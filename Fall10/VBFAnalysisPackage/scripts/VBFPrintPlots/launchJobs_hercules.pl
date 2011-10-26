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

$SELECTIONSCfgTemplate = $BASEDir."/".$SELECTIONSCfgTemplate;

print "BASEDir = "          .$BASEDir."\n" ;
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
  
  ($mH) = split(" ");
  
  print("Higgs mass = ".$mH."\n");
  $sampleDir = $OUTPUTSaveDir.$sample."/";
  
  
  $selectionsCfgFile = "./selections_".$mH.".cfg";
  system("cat ".$SELECTIONSCfgTemplate."   | sed -e s%HIGGSMASS%".$mH.
                                       "%g > ".$selectionsCfgFile);
  
  
  $sampleJobFile = "./job_".$mH.".sh";
  open(SAMPLEJOBFILE, ">", $sampleJobFile);
  
  $command = "cd ".$BASEDir;
  print SAMPLEJOBFILE $command."\n";
  
  $command = "source ./scripts/setup.sh";
  print SAMPLEJOBFILE $command."\n";
  
  $command = "cd ".$BASEDir."/scripts/VBFPrintPlots/";
  print SAMPLEJOBFILE $command."\n";
  
  $command = "unbuffer ".$EXEName." ".$BASEDir."/scripts/VBFPrintPlots/".$selectionsCfgFile." > out_".$mH.".txt";
  print SAMPLEJOBFILE $command."\n";
  
  
  
  print SAMPLEJOBLISTFILE "echo \"qsub -V -q production -d ./ ".$sampleJobFile."\"\n";
  print SAMPLEJOBLISTFILE "qsub -V -q production -d ./ ".$sampleJobFile."\n";
  
}
