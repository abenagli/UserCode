#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

use Env;

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

$BASEDir          = $VBFANALYSISPKG;
$JETAlgorithm     = $User_Preferences{"JETAlgorithm"} ;
$JETType          = $User_Preferences{"JETType"} ;
$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$EXEName          = $User_Preferences{"EXEName"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$INPUTSAVEPath    = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;

$JOBCfgTemplate = $BASEDir."/".$JOBCfgTemplate;


print "BASEDir = "          .$BASEDir."\n" ;
print "JETAlgorithm = "     .$JETAlgorithm."\n" ;
print "JETType = "          .$JETType."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "EXEName = "          .$EXEName."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "INPUTSAVEPath = "    .$INPUTSAVEPath."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n\n" ;






$sampleJobListFile = "./lancia.sh";
open(SAMPLEJOBLISTFILE, ">", $sampleJobListFile);


#####################################################
# PG prepare the array containing the root files list
#####################################################


open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
  system("cd ".$BASEDir."\n");
  
  chomp($_);
  
  ($sample,$dummy,$color,$linestyle,$fillstyle,$dataFlag,$mH,$crossSection,$dummy,$dummy) = split(" ") ;
  $subsample = substr($sample,0,1);
  if($subsample eq "#")
  {
    next;
  }
  
  print("Sample: ".$sample."\n") ;  
  system ("mkdir ".$sample."_".$JETAlgorithm."\n") ;
  
  
  
    
  
  
  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("ls -l ".$INPUTSAVEPath.$sample." | grep root | awk '{print \$9}' > ".$LISTOFFiles."\n") ;
  
  
  
  $totNumber = 0;
  $jobNumber = 0;
  
  open (LISTOFFiles,$LISTOFFiles) ;
  while (<LISTOFFiles>)
  {
    #print "File = ".$_;
    ++$totNumber;
  }
  
  $jobNumber = int($totNumber/$JOBModulo);
  if( $totNumber%$JOBModulo != 0)
  {
    $jobNumber = $jobNumber+1;
  }
  
  print "NumberOfJobs = ".$jobNumber."\n";
  
  
  
  
  
  
  ################
  # loop over jobs 
  ################
  
  for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
  { 
    $currDir = `pwd` ;
    chomp ($currDir) ;
    
    $jobDir = $currDir."/".$sample."_".$JETAlgorithm."/JOB_".$jobIt ;
    system ("mkdir ".$jobDir." \n") ;
    
    $tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
    $command = "touch ".$tempBjob ;
    system ($command) ;
    $command = "chmod 777 ".$tempBjob ;
    system ($command) ;
    


    
    
    $tempo1 = "./tempo1" ;
    system ("cat ".$JOBCfgTemplate."   | sed -e s%JETALGORITHM%".$JETAlgorithm.
                                   "%g | sed -e s%JETTYPE%".$JETType.
                                   "%g | sed -e s%JOBIT%".$jobIt.
                                   "%g | sed -e s%MASS%".$mH.
                                   "%g | sed -e s%DATAFLAG%".$dataFlag.
                                   "%g | sed -e s%CROSSECTION%".$crossSection.
                                   "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName.
                                   "%g | sed -e s%OUTPUTROOTFILEPATH%".$OUTPUTSAVEPath."/".$sample."/".
                                   "%g > ".$tempo1) ;
    
    
    
    
    
    
    ######################
    # make job files
    ######################    
    
    open (SAMPLEJOBFILE, ">", $tempBjob) or die "Can't open file ".$tempBjob;
    $command = "cd ".$BASEDir ;
    print SAMPLEJOBFILE $command."\n";

    $command = "source ./scripts/setup.sh" ;
    print SAMPLEJOBFILE $command."\n";
    
    $command = "cd ".$jobDir ;
    print SAMPLEJOBFILE $command."\n";
    
    $command = "mkdir ".$OUTPUTSAVEPath."/".$sample;
    print SAMPLEJOBFILE $command."\n";
    
    #$command = "mkdir /data_CMS/cms/abenagli/tmp/".$sample."_JOB".$jobIt."/";
    #print SAMPLEJOBFILE $command."\n";
    
    #$command = "chmod 777 /data_CMS/cms/abenagli/tmp/".$sample."_JOB".$jobIt."/";
    #print SAMPLEJOBFILE $command."\n";
    
    
    
    
    
    $it = 0;
    $JOBLISTOFFiles = $jobDir."/list.txt";
    open (JOBLISTOFFiles, ">", $JOBLISTOFFiles) or die "Can't open file ".$JOBLISTOFFiles;

    open (LISTOFFiles2,$LISTOFFiles) ;
    while (<LISTOFFiles2>)
    {
      chomp; 
      s/#.*//;                # no comments
      s/^\s+//;               # no leading white
      s/\s+$//;               # no trailing white
      $file = $_ ;
      
      if( ($it >= ($jobIt - 1)*$JOBModulo) && ($it < ($jobIt)*$JOBModulo) )
      { 
        #$command = "rfcp ".$INPUTSAVEPath."/".$sample."/".$file." /data_CMS/cms/abenagli/tmp/".$sample."_JOB".$jobIt."/";
        #print SAMPLEJOBFILE $command."\n";
	
        print JOBLISTOFFiles $INPUTSAVEPath."/".$sample."/".$file."\n";
        #print JOBLISTOFFiles "root://polgrid4.in2p3.fr/".$INPUTSAVEPath."/".$sample."/".$file."\n";
      }
      ++$it;
    }
    
    
    
    $tempo2 = "./tempo2" ;    
    system ("cat ".$tempo1." | sed -e s%INPUTFILELIST%".$JOBLISTOFFiles."%g > ".$tempo2) ;
    
    
    $JOBCfgFile = $jobDir."/selections_cfg.py" ;
    system ("mv ".$tempo2." ".$JOBCfgFile) ;
    system ("rm ./tempo*") ;
    
    
    
    
    
    
    $command = "unbuffer ".$EXEName." ".$JOBCfgFile." >> ".$jobDir."/out.txt" ;
    print SAMPLEJOBFILE $command."\n";    
    
    
    
    ############
    # submit job
    ############
    
    $command = "echo \"qsub -V -q production -d ".$jobDir." ".$tempBjob."\"\n" ;
    print SAMPLEJOBLISTFILE $command."\n";
    
    $command = "qsub -V -q production -d ".$jobDir." ".$tempBjob."\n" ; 
    print SAMPLEJOBLISTFILE $command."\n";
    
    #print "\n" ;
  }

  system ("rm ".$LISTOFFiles) ;
}  
