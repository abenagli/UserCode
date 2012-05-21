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



$CMSSWDir         = $User_Preferences{"CMSSWDir"};
$INPUTDir         = $User_Preferences{"INPUTDir"};
$xWidth           = $User_Preferences{"xWidth"};
$step             = $User_Preferences{"step"};
$additionalCuts   = $User_Preferences{"additionalCuts"};
$onSidebands      = $User_Preferences{"onSidebands"};
$analysisMethod   = $User_Preferences{"analysisMethod"};
$fitFunction      = $User_Preferences{"fitFunction"};
$combineMethod    = $User_Preferences{"combineMethod"};
$combineOptions   = $User_Preferences{"combineOptions"};
$combineTechnique = $User_Preferences{"combineTechnique"};

print "CMSSWDir = ".$CMSSWDir."\n";
print "INPUTDir = ".$INPUTDir."\n";
print "step = ".$step."\n";
print "xWidth = ".$xWidth."\n";



@masses = (250,300,350,400,450,500,550,600);
$nMasses = @masses;



if( $onSidebands == 0 )
{
  if( $additionalCuts eq "none" )
  {
    $outputDir = $INPUTDir."/combine_signal/binWidth".$xWidth."/step".$step."/";
  }
  else
  {
    $outputDir = $INPUTDir."/combine_signal/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
  }
}

else
{
  if( $additionalCuts eq "none" )
  {
    $outputDir = $INPUTDir."/combine_sidebands/binWidth".$xWidth."/step".$step."/";
  }
  else
  {
    $outputDir = $INPUTDir."/combine_sidebands/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
  }
}






$jobFile = "./combineDatacards.sh";
open(JOBFILE, ">", $jobFile);

print JOBFILE "source /gwpool/initcms/slc5_64-cmssw.csh\n";
print JOBFILE "cd ".$CMSSWDir."\n";
print JOBFILE "eval `scramv1 runt -csh`\n";
print JOBFILE "cd -\n";



for($massIt = 0; $massIt < $nMasses; ++$massIt)
{
  $mass = $masses[$massIt];
  
  $datacard_e    = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e.txt";
  $datacard_mu   = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_mu.txt";
  $datacard_e_mu = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e_mu.txt";
  
  
  $combineCommand = "combineCards.py ".$datacard_e." ".$datacard_mu." > ".$datacard_e_mu."\n";
  print JOBFILE $combineCommand."\n";
}
