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


$BASEDir = $VBFANALYSISPKG;
$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$JETAlgorithm     = $User_Preferences{"JETAlgorithm"} ;
$INPUTSAVEPath    = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;

print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "JETAlgorithm = "     .$JETAlgorithm."\n" ;
print "INPUTSAVEPath = "    .$INPUTSAVEPath."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;






#####################################################
# PG prepare the array containing the root files list
#####################################################


open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
  chomp($_);
  
  ($sample,$dummy,$color,$linestyle,$dataFlag,$mH,$crossSection,$dummy,$dummy) = split(" ") ;
  $subsample = substr($sample,0,1);
  if($subsample eq "#")
  {
      next;
  }

  print("\n\n\n************** Sample: ".$sample." **************\n") ;  
  

  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("ls -l ".$INPUTSAVEPath.$sample." | grep root | grep Preselection | awk '{print \$9}' > ".$LISTOFFiles."\n") ;
  
  $command = "rm -rf ".$OUTPUTSAVEPath."/".$sample;
  system($command);

  $command = "mkdir ".$OUTPUTSAVEPath."/".$sample;
  system($command);
  
  $command = "hadd -f ".$OUTPUTSAVEPath."/".$sample."/".$OUTPUTFILEName."_all_".$JETAlgorithm.".root";
  open (LISTOFFiles2,$LISTOFFiles) ;
  while (<LISTOFFiles2>)
  {
    chomp($_);
    $command2 = " ".$INPUTSAVEPath."/".$sample."/".$_;
    $command = $command.$command2;
  }
  
  system($command);
  
}  



###############
# rm list files
###############

open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
  chomp($_);

  ($sample,$dummy,$color,$linestyle,$dataFlag,$mH,$crossSection,$dummy,$dummy) = split(" ") ;
  $subsample = substr($sample,0,1);
  if($subsample eq "#")
  {
      next;
  }
  
  $LISTOFFiles = "./list_".$sample.".txt" ;
  $command = "rm ".$LISTOFFiles;
  system($command);
}
