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


$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$JETAlgorithm     = $User_Preferences{"JETAlgorithm"} ;
$INPUTSAVEPath    = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;

print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "JETAlgorithm = "     .$JETAlgorithm."\n" ;
print "INPUTSAVEPath = "    .$INPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;






#####################################################
# PG prepare the array containing the root files list
#####################################################


open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
  chomp($_);
  $sample = $_;
  print("Sample: ".$sample."\n") ;  
  

  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("rfdir ".$INPUTSAVEPath.$sample." | grep root | grep VBFHiggsToWWTolnujjPreselection | awk '{print \$9}' > ".$LISTOFFiles."\n") ;
  
  
  
  system("mkdir /tmp/abenagli/".$sample);
  open (LISTOFFiles,$LISTOFFiles) ;
  while (<LISTOFFiles>)
  {
    chomp($_);
    $command = "rfcp ".$INPUTSAVEPath.$sample."/".$_." /tmp/abenagli/".$sample."/\n";
    #print($command);
    system($command);
    
  }

  system("mkdir /tmp/abenagli/hadd");
  system("mkdir /tmp/abenagli/hadd/".$sample);
  $command = "hadd -f /tmp/abenagli/hadd/".$sample."/".$OUTPUTFILEName."_all_".$JETAlgorithm.".root";
  open (LISTOFFiles2,$LISTOFFiles) ;
  while (<LISTOFFiles2>)
  {
    chomp($_);
    $command2 = " /tmp/abenagli/".$sample."/".$_;
    $command = $command.$command2;
  }
  #print($command);
  system($command);
  
}  
