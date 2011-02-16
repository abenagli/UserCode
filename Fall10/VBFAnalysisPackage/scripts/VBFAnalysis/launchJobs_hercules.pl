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
$JETAlgorithm          = $User_Preferences{"JETAlgorithm"};
$JETType               = $User_Preferences{"JETType"};
$SAMPLESListFile       = $User_Preferences{"SAMPLESListFile"};
$SAMPLESDir            = $User_Preferences{"SAMPLESDir"};
$INPUTFILEName         = $User_Preferences{"INPUTFILEName"};
$OUTPUTSaveDir         = $User_Preferences{"OUTPUTSaveDir"};

$SELECTIONSCfgTemplate = $BASEDir."/".$SELECTIONSCfgTemplate;
$SAMPLESListFile       = $BASEDir."/".$SAMPLESListFile;
$OUTPUTSaveDir         = $BASEDir."/".$OUTPUTSaveDir;

print "BASEDir = "          .$BASEDir."\n" ;
print "SELECTIONSCfgTemplate = ".$SELECTIONSCfgTemplate."\n";
print "EXEName = ".$EXEName."\n";
print "JETAlgorithm = ".$JETAlgorithm."\n";
print "JETType = ".$JETType."\n";
print "SAMPLESListFile = ".$SAMPLESListFile."\n";
print "SAMPLESDir = ".$SAMPLESDir."\n";
print "INPUTFILEName = ".$INPUTFILEName."\n";
print "OUTPUTSaveDir = ".$OUTPUTSaveDir."\n";





$command = "mkdir ".$OUTPUTSaveDir;
print($command."\n");
system($command);


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
  
  ($sample,$sampleName,$dataFlag,$mH,$scale,$crossSection,$jetalgo) = split(" ") ;
  $nullSample = "";
  if($sample eq $nullSample)
  {
    next;
  }
  
  print("sample = ".$sample."   name = ".$sampleName."   crossSection = ".$crossSection."\n");
  $sampleDir = $OUTPUTSaveDir.$sample."/";
  
  $command = "rm -rf ".$sampleDir;
  print($command."\n");
  system($command);
  
  $command = "mkdir ".$sampleDir;
  print($command."\n");
  system($command);
  
  
  
  

  $inputFileList = $sampleDir."/list.txt";  
  open(INPUTFILELIST, ">", $inputFileList);
  
  
  print INPUTFILELIST $SAMPLESDir.$sample."/".$INPUTFILEName."_all_".$JETAlgorithm.".root\n";
  #print("inputFileList = ".$inputFileList."\n");
  
  
  $selectionsCfgFile = $sampleDir."/selections.cfg" ;
  #print("selectionsCfgFile = ".$selectionsCfgFile."\n");
  system("cat ".$SELECTIONSCfgTemplate."   | sed -e s%INPUTFILELIST%".$inputFileList.
                                       "%g | sed -e s%BASEDIR%".$BASEDir.
                                       "%g | sed -e s%OUTPUTSAVEPATH%".$sampleDir.
                                       "%g | sed -e s%JETALGORITHM%".$JETAlgorithm.
                                       "%g | sed -e s%JETTYPE%".$JETType.
                                       "%g | sed -e s%MH%".$mH.
                                       "%g | sed -e s%CROSSECTION%".$crossSection.
                                       "%g | sed -e s%DATAFLAG%".$dataFlag.
                                       "%g | sed -e s%MASS%".$mH.
                                       "%g > ".$selectionsCfgFile);
  
  
  
  
  
  
  $sampleJobFile = $sampleDir."/job.sh";
  open(SAMPLEJOBFILE, ">", $sampleJobFile);
   
  $command = "cd ".$BASEDir;
  print SAMPLEJOBFILE $command."\n";
  
  $command = "source ./scripts/setup.sh";
  print SAMPLEJOBFILE $command."\n";
    
  $command = "cd ".$sampleDir;
  print SAMPLEJOBFILE $command."\n";

  $command = "unbuffer ".$EXEName." ./selections.cfg >> ".$sampleDir."/out.txt";
  print SAMPLEJOBFILE $command."\n";
  
  
  
  $command = "chmod 755 ".$sampleJobFile;
  #print($command."\n\n\n");
  system($command);
  
  print SAMPLEJOBLISTFILE "qsub -V -d ".$sampleDir." -q production ".$sampleJobFile."\n";
  
  ++$type;
}
