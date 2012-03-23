
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



$INPUTDir         = $User_Preferences{"INPUTDir"};
$xWidth           = $User_Preferences{"xWidth"};
$flavour          = $User_Preferences{"flavour"};
$step             = $User_Preferences{"step"};
$additionalCuts   = $User_Preferences{"additionalCuts"};
$onSidebands      = $User_Preferences{"onSidebands"};
$analysisMethod   = $User_Preferences{"analysisMethod"};
$fitFunction      = $User_Preferences{"fitFunction"};
$combineMethod    = $User_Preferences{"combineMethod"};
$combineTechnique = $User_Preferences{"combineTechnique"};



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
      $plotName = "limitPlot_signal_step".$step."_binWidth".$xWidth."_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_signal_step".$step."_binWidth".$xWidth."_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
  else
  {
    $outputDir = $INPUTDir."/combine_signal/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_signal_step".$step."_".$additionalCuts."_binWidth".$xWidth."_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_signal_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_signal_step".$step."_".$additionalCuts."_binWidth".$xWidth."_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
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
      $plotName = "limitPlot_sidebands_step".$step."_binWidth".$xWidth."_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_sidebands_step".$step."_binWidth".$xWidth."_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
  else
  {
    $outputDir = $INPUTDir."/combine_sidebands/binWidth".$xWidth."/step".$step."_".$additionalCuts."/";
    
    if( $analysisMethod eq "sidebands" )
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_sideband_step".$step."_".$additionalCuts."_binWidth".$xWidth."_".$analysisMethod."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
    else
    {
      $jobDir = $outputDir."limitPlot_sidebands_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
      $plotName = "limitPlot_sideband_step".$step."_".$additionalCuts."_binWidth".$xWidth."_".$analysisMethod."_".$fitFunction."_".$flavour."_".$combineMethod."_".$combineTechnique."/";
    }
  }
}



chop($plotName);
#print ">>> jobDir: ".$jobDir."\n";


if( 1 == 1 )
{
  $outFile_masses   = $jobDir."/results_masses.txt";
  $outFile_observed = $jobDir."/results_observed.txt";
  $outFile_2s_low   = $jobDir."/results_2s_low.txt";
  $outFile_1s_low   = $jobDir."/results_1s_low.txt";
  $outFile_median   = $jobDir."/results_median.txt";
  $outFile_1s_hig   = $jobDir."/results_1s_hig.txt";
  $outFile_2s_hig   = $jobDir."/results_2s_hig.txt";
  
  open OUTFILE_MASSES,   ">", $outFile_masses;
  open OUTFILE_OBSERVED, ">", $outFile_observed;
  open OUTFILE_2S_LOW,   ">", $outFile_2s_low;
  open OUTFILE_1S_LOW,   ">", $outFile_1s_low;
  open OUTFILE_MEDIAN,   ">", $outFile_median;
  open OUTFILE_1S_HIG,   ">", $outFile_1s_hig;
  open OUTFILE_2S_HIG,   ">", $outFile_2s_hig;
  
  
  
  for($massIt = 0; $massIt < $nMasses; ++$massIt)
  {
    $mass = $masses[$massIt];
    $limit_observed;
    $limit_2s_low;
    $limit_1s_low;
    $limit_median;
    $limit_1s_hig;
    $limit_2s_hig;
    
    $outFile = $jobDir."out_H".$mass.".txt";
    open OUTFILE, "<", $outFile;
    while(<OUTFILE>)
    {
      $line = $_;
      @words = split(' ',$line);
      
      
      
      # observed
      if( ($words[0] eq "Observed") && ($words[1] eq "Limit:") )
      {
        $limit_observed = $words[4];
        if( ($limit_observed <= 0.1) || ($limit_observed >= 20.) )
        { 
          $limit_observed = 1.;
        }
      }
      
      # 2 sigma low
      if( ($words[0] eq "Expected") && ($words[1] eq "2.5%:") )
      {
        $limit_2s_low = $words[4];
        if( ($limit_2s_low <= 0.1) || ($limit_2s_low >= 20.) )
        { 
          $limit_2s_low = 1.;
        }
      }
      
      # 1 sigma low
      if( ($words[0] eq "Expected") && ($words[1] eq "16.0%:") )
      {
        $limit_1s_low = $words[4];
        if( ($limit_1s_low <= 0.1) || ($limit_1s_low >= 20.) )
        { 
          $limit_1s_low = 1.;
        }
      }
      
      # median expected
      if( ($words[0] eq "Expected") && ($words[1] eq "50.0%:") )
      {
        $limit_median = $words[4];
        if( ($limit_median <= 0.1) || ($limit_median >= 20.) )
        { 
          $limit_median = 1.;
        }
      }
      
      # 1 sigma high
      if( ($words[0] eq "Expected") && ($words[1] eq "84.0%:") )
      {
        $limit_1s_hig = $words[4];
        if( ($limit_1s_hig <= 0.1) || ($limit_1s_hig >= 20.) )
        { 
          $limit_1s_hig = 1.;
        }
      }
      
      # 2 sigma high
      if( ($words[0] eq "Expected") && ($words[1] eq "97.5%:") )
      {
        $limit_2s_hig = $words[4];
        if( ($limit_2s_hig <= 0.1) || ($limit_2s_hig >= 20.) )
        { 
          $limit_2s_hig = 1.;
        }
      }
    }
    
    print OUTFILE_MASSES   $mass."\n";
    print OUTFILE_OBSERVED $limit_observed."\n";
    print OUTFILE_2S_LOW   $limit_2s_low."\n";
    print OUTFILE_1S_LOW   $limit_1s_low."\n";
    print OUTFILE_MEDIAN   $limit_median."\n";
    print OUTFILE_1S_HIG   $limit_1s_hig."\n";
    print OUTFILE_2S_HIG   $limit_2s_hig."\n";
  }
}



$drawObserved = "1";

if( @ARGV >= 2 )
{
  $drawObserved = $ARGV[1];
}

$command = "drawExclusionPlot.exe ".$jobDir." ".$plotName." ".$drawObserved;

for($iPar = 0; $iPar < @ARGV-2; ++$iPar)
{
  $command = $command." ".$ARGV[$iPar+2];
}

print($command."\n");
system($command);
