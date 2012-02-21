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
$flavour          = $User_Preferences{"flavour"};
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
print "flavour = ".$flavour."\n";



@masses = (250,300,350,400,450,500,550,600);
$nMasses = @masses;



if( $onSidebands == 0 )
{
  if( $additionalCuts eq "none" )
  {
    $outputDir = $INPUTDir."/combine_signal/binWidth".$xWidth."/step".$step."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
  else
  {
    $outputDir = $INPUTDir."/combine_signal/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
}

else
{
  if( $additionalCuts eq "none" )
  {
    $outputDir = $INPUTDir."/combine_sidebands/binWidth".$xWidth."/step".$step."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
  else
  {
    $outputDir = $INPUTDir."/combine_sidebands/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
}



if( -e $jobDir )
{
  $command = "rm -rf ".$jobDir;
  system($command);
}

$command = "mkdir ".$jobDir;
system($command);

$command = "cd ".$jobDir;
system($command);



for($massIt = 0; $massIt < $nMasses; ++$massIt)
{
  $mass = $masses[$massIt];
  
  $jobFile = $jobDir."job_H".$mass.".sh";
  $outFile = $jobDir."out_H".$mass.".txt";
  
  if( $analysisMethod eq "sidebands")
  {
    $datacard ="datacard_".$analysisMethod."_".$combineTechnique."_".$mass."_".$flavour.".txt";
    $shapes = "shapes_".$analysisMethod."_".$mass."_".$flavour.".root";
  }
  else
  {
    $datacard ="datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_".$flavour.".txt";
    $shapes = "shapes_".$analysisMethod."_".$fitFunction."_".$mass."_".$flavour.".root";
  }
  
  open(JOBFILE, ">", $jobFile);
  
  print JOBFILE "source /gwpool/initcms/slc5_64-cmssw.sh\n";
  print JOBFILE "cd ".$CMSSWDir."\n";
  print JOBFILE "eval `scramv1 runt -sh`\n";
  print JOBFILE "cd ".$jobDir."\n";
  print JOBFILE "cp ".$outputDir."/".$datacard." ./\n";
  print JOBFILE "cp ".$outputDir."/".$shapes." ./\n";
  
  $combineCommand = "unbuffer combine -M ".$combineMethod." ".$combineOptions." -n HWWlvjj -m ".$mass." -d ".$datacard." >> ".$outFile."\n";
  print JOBFILE $combineCommand."\n";
  
  
  
  $submit = "qsub -V -d ".$jobDir." -k n -q shortcms ".$jobFile;
  print ">>>>>>>>> submit: ".$submit."\n";
  system($submit);
}
