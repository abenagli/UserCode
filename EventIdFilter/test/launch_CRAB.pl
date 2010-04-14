#!/usr/bin/perl

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


$LOCALDir         = $User_Preferences{"LOCALDir"} ;
$CMSSWCfgTemplate = $User_Preferences{"CMSSWCfgTemplate"} ;
$CRABCfgTemplate  = $User_Preferences{"CRABCfgTemplate"} ;
$RUNId            = $User_Preferences{"RUNId"} ;
$EVId             = $User_Preferences{"EVId"} ;

print "LOCALDir = ".$LOCALDir."\n" ;
print "CMSSWCfgTemplate = " .$CMSSWCfgTemplate."\n" ;
print "CRABCfgTemplate = " .$CRABCfgTemplate."\n" ;
print "RUNId = ".$RUNId."\n" ;
print "EVId = ".$EVId."\n" ;






$jobDir = $LOCALDir."/run".$RUNId."_ev".$EVId ;
$command = "mkdir ".$jobDir ;
system($command) ;

$CMSSWCfgFile = $jobDir."/"."eventIdFilter_cfg.py" ;
system ("cat ".$CMSSWCfgTemplate."   | sed -e s%RUNID%".$RUNId.
                                 "%g | sed -e s%EVID%".$EVId.
                                 "%g > ".$CMSSWCfgFile) ;

$CRABCfgFile = $jobDir."/"."crab.cfg" ;
system ("cat ".$CRABCfgTemplate."   | sed -e s%RUNID%".$RUNId.
                                "%g | sed -e s%JOBDIR%".$jobDir.
                                "%g > ".$CRABCfgFile) ;


$command = "crab -create -submit -cfg ".$CRABCfgFile ;
print($command."\n") ; 
system($command."\n") ; 
