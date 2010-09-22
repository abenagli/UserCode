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
  my $JOBDir = $BASEDir."/run".$RUNId."_evt".$EVTId."_CRAB";
  
  my $command = "mkdir ".$JOBDir;
  system($command);
  
  
  
  # CMSSW cfg file
  my $CMSSWCfgTemplate = $BASEDir."/eventIdFilter_template_cfg.py";
  my $CMSSWCfgFile = $JOBDir."/eventIdFilter_cfg.py";
  
  system ("cat ".$CMSSWCfgTemplate."   | sed -e s%RUNID%".$RUNId.
                                   "%g | sed -e s%LSID%".$LSId.
                                   "%g | sed -e s%EVTID%".$EVTId.
	                           "%g > ".$CMSSWCfgFile);



  # JSON file
  my $JSONTemplate = $BASEDir."/my_JSON_template.txt";
  my $JSONFile = $JOBDir."/my_JSON.txt";
  
  system ("cat ".$JSONTemplate."   | sed -e s%RUNID%".$RUNId.
                               "%g | sed -e s%LSID%".$LSId.
                               "%g | sed -e s%EVTID%".$EVTId.
	                       "%g > ".$JSONFile);


  # CRAB cfg file
  my $CRABTemplate = $BASEDir."/crab_DATA_template.cfg";
  my $CRABCfgFile = $JOBDir."/crab_DATA.cfg";
  
  system ("cat ".$CRABTemplate."   | sed -e s%RUNID%".$RUNId.
                               "%g | sed -e s%LSID%".$LSId.
                               "%g | sed -e s%EVTID%".$EVTId.
                               "%g | sed -e s%DATASETPATH%".$DATASETPath.
                               "%g | sed -e s%CMSSWCFGFILE%".$CMSSWCfgFile.
                               "%g | sed -e s%JSONFILE%".$JSONFile.
                               "%g | sed -e s%WORKINGDIR%".$JOBDir."/"."CRAB".
	                       "%g > ".$CRABCfgFile);

  
  
  # Submit jobs
  $command =  "crab -create -submit -cfg ".$CRABCfgFile;
  system($command."\n");
}
