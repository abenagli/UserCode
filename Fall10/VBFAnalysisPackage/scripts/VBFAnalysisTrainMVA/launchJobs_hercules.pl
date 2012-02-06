#!/usr/bin/perl


use Env;

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#PG lettura dei parametri da cfg file
#PG --------------------------------
print "reading ".$ARGV[0]."\n" ;

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
  }


$BASEDir               = $VBFANALYSISPKG;
$SELECTIONSCfgTemplate = $User_Preferences{"SELECTIONSCfgTemplate"};
$EXEName               = $User_Preferences{"EXEName"};
$SAMPLESListFile       = $User_Preferences{"SAMPLESListFile"};
$OUTDir                = $User_Preferences{"OUTDir"};

$SELECTIONSCfgTemplate = $BASEDir."/".$SELECTIONSCfgTemplate;

print "BASEDir = "          .$BASEDir."\n" ;
print "OUTDir  = "          .$OUTDir."\n" ;
print "SELECTIONSCfgTemplate = ".$SELECTIONSCfgTemplate."\n";
print "EXEName = ".$EXEName."\n";
print "SAMPLESListFile = ".$SAMPLESListFile."\n";






$sampleJobListFile = "./lancia.sh";
open(SAMPLEJOBLISTFILE, ">", $sampleJobListFile); 



#------------------
# loop over samples

print("\n");
open(SAMPLESListFile, $SAMPLESListFile) ;

$type = 0;
while(<SAMPLESListFile>)
{
  chomp;
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  
  ($mH,$mHcutMin,$mHcutMax) = split(" ") ;

  print("Higgs mass = ".$mH."\n");
  $sampleDir = $OUTPUTSaveDir.$sample."/";
  
  
  $selectionsCfgFile = "./selections_".$mH.".cfg";
  system("cat ".$SELECTIONSCfgTemplate."   | sed -e s%HIGGSMASS%".$mH.
                                       "%g | sed -e s%OUTDIR%".$OUTDir.
                                       "%g | sed -e s%HMASSMIN%".$mHcutMin.
                                       "%g | sed -e s%HMASSMAX%".$mHcutMax.
                                       "%g > ".$selectionsCfgFile);
                                         
  $sampleJobFile = "./job_".$mH.".sh";
  open(SAMPLEJOBFILE, ">", $sampleJobFile);

  $command = "mkdir ".$OUTDir."/TMVA/H".$mH;
  unless (-e $OUTDir."/TMVA/H".$mH) {
    system($command."\n");
  }
  
  $command = "cd ".$BASEDir;
  print SAMPLEJOBFILE $command."\n";
  
  $command = "source ./scripts/setup.sh";
  print SAMPLEJOBFILE $command."\n";
  
  $command = "cd ".$BASEDir."/scripts/VBFAnalysisTrainMVA/";
  print SAMPLEJOBFILE $command."\n";
  
  $command = "unbuffer ".$EXEName." ".$BASEDir."/scripts/VBFAnalysisTrainMVA/".$selectionsCfgFile." > out_".$mH.".txt";
  print SAMPLEJOBFILE $command."\n";
  
  
  
  print SAMPLEJOBLISTFILE "echo \"qsub -V -q longcms -d ./ ".$sampleJobFile."\"\n";
  print SAMPLEJOBLISTFILE "qsub -V -q longcms -d ./ ".$sampleJobFile."\n";
  
}
