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
$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$INPUTSAVEPath    = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;

$LISTOFSamples  = $BASEDir."/".$LISTOFSamples;
$JOBCfgTemplate = $BASEDir."/".$JOBCfgTemplate;


print "BASEDir = "          .$BASEDir."\n" ;
print "JETAlgorithm = "     .$JETAlgorithm."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "INPUTSAVEPath = "    .$INPUTSAVEPath."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;






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
  $sample = $_;
  print("Sample: ".$sample."\n") ;  
  system ("mkdir ".$sample."_".$JETAlgorithm."\n") ;
  
  $command = "rm -rf /grid_mnt/data__DATA/data.polcms/cms/abenagli/tmp/".$sample;
  system($command);

  $command = "mkdir /grid_mnt/data__DATA/data.polcms/cms/abenagli/tmp/".$sample;
  system($command);
  
  
  
    
  
  
  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("rfdir ".$INPUTSAVEPath.$sample." | grep VBFPreselection | awk '{print \$9}' > ".$LISTOFFiles."\n") ;
  
  
  
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
                                   "%g | sed -e s%JOBIT%".$jobIt.
                                   "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName.
                                   "%g | sed -e s%OUTPUTROOTFILEPATH%".$OUTPUTSAVEPath."/".$sample."/".
                                   "%g > ".$tempo1) ;
    
    
    
    $it = 0;
    $JOBLISTOFFiles = $jobDir."/list_".$sample.".txt";
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
        $command = "rfcp ".$INPUTSAVEPath."/".$sample."/".$file." /grid_mnt/data__DATA/data.polcms/cms/abenagli/tmp/".$sample;
        system ("echo ".$command." >> ".$tempBjob);

	print JOBLISTOFFiles "/grid_mnt/data__DATA/data.polcms/cms/abenagli/tmp/".$sample."/".$file."\n";
      }
      ++$it;
    }
    
    
    
    $tempo2 = "./tempo2" ;    
    system ("cat ".$tempo1." | sed -e s%INPUTFILELIST%".$JOBLISTOFFiles."%g > ".$tempo2) ;
    
    
    $JOBCfgFile = $jobDir."/selections_cfg.py" ;
    system ("mv ".$tempo2." ".$JOBCfgFile) ;
    system ("rm ./tempo*") ;
    
    
    
    
    
    
    ######################
    # make job files
    ######################    
    
    $command = "cd ".$BASEDir ;
    system ("echo ".$command." >> ".$tempBjob) ;

    $command = "source ./scripts/setup.csh" ;
    system ("echo ".$command." >> ".$tempBjob) ;
    
    $command = "cd ".$jobDir ;
    system ("echo ".$command." >> ".$tempBjob) ;
    
    $command = "mkdir ".$OUTPUTSAVEPath."/".$sample;
    system ("echo ".$command." >> ".$tempBjob) ;
    
    $command = "Collisions7TeVPreselection.exe ".$JOBCfgFile ;
    system ("echo ".$command." >> ".$tempBjob) ;
    
    
    
    ############
    # submit job
    ############
    
    print SAMPLEJOBLISTFILE "echo \"*************************".$sample."***************************\"\n";
    print SAMPLEJOBLISTFILE "source ".$tempBjob."\n";
    print SAMPLEJOBLISTFILE "echo \"**************************************************\"\n";
    print SAMPLEJOBLISTFILE "echo \"**************************************************\"\n";
    
    #$command = "qsub -V -q production -d ".$jobDir." ".$tempBjob."\n" ;  
    #print ($command);
    #system ($command);
    
    print "\n" ;
  }

  system ("rm ".$LISTOFFiles) ;
}  
