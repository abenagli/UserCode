#!/usr/bin/perl

use Env;
use Cwd;
use strict;
use warnings;



# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#PG lettura dei parametri da cfg file
#PG --------------------------------
print "reading ".$ARGV[0]."\n" ;

open(USERCONFIG,$ARGV[0]);
while (<USERCONFIG>)
{
  chomp; 
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  
  my $DATASETPath;
  my $RUNId;
  my $LSId;
  my $EVTId;
  ($DATASETPath,$RUNId,$LSId,$EVTId) = split(" ");
  
  print("\n\n\n");
  print("-------------------------------------------------------------------------\n");
  print("****** RUN: ".$RUNId." ****** LS: ".$LSId." ****** EVT: ".$EVTId." ******\n");
  print("-------------------------------------------------------------------------\n");
  
  
  
  my $BASEDir = getcwd;
  my $JOBDir = $BASEDir."/run".$RUNId."_evt".$EVTId."_POLUI";
  
  my $command = "mkdir ".$JOBDir;
  system($command);
  
  
  
  # query the DBS
  my $fileList = $JOBDir."/fileList.txt";
  my $listOfFiles = "";
  $command = "dbs search --query=\"find file where run=".$RUNId." and lumi = ".$LSId." and dataset=".$DATASETPath."\" | grep root > ".$fileList;
  system($command);
  
  
  
  # make the file list
  open(FILELIST,$fileList);
  while(<FILELIST>)
  {
    chomp;
    my $file = $_ ;
    print("file = ".$file."\n");
    $listOfFiles = $listOfFiles."APICE".$file."APICE,";
  }
  chop($listOfFiles);
  
  
  
  # CMSSW cfg file
  my $CMSSWCfgTemplate = $BASEDir."/eventIdFilter_template_cfg.py";
  my $CMSSWCfgFile = $JOBDir."/eventIdFilter_cfg.py";
  
  system ("cat ".$CMSSWCfgTemplate."   | sed -e s%RUNID%".$RUNId.
                                   "%g | sed -e s%LSID%".$LSId.
                                   "%g | sed -e s%EVTID%".$EVTId.
                                   "%g | sed -e s%JOBDIR%".$JOBDir.
                                   "%g | sed -e s%LISTOFFILES%".$listOfFiles.
                                   "%g | sed -e s%APICE%\\'%g > ".$CMSSWCfgFile);



  # Submit jobs
  $command =  "cmsRun ".$CMSSWCfgFile;
  system($command."\n");
}
