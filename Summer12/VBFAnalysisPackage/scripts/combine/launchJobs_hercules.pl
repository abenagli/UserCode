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
$nToys            = $User_Preferences{"nToys"};
$toyParent        = $User_Preferences{"toyParent"};



if( @ARGV != 3 )
{
  print(">>> launchJobs.pl::usage   perl launchJobs.pl   params.CFG   flavour   charge\n");
  exit;
}

$flavour = $ARGV[1];
$charge  = $ARGV[2];

if( !($charge eq "pm") )
{
  $charge = "_".$charge;
}
else
{
  $charge = "";
}

print "CMSSWDir = ".$CMSSWDir."\n";
print "INPUTDir = ".$INPUTDir."\n";
print "step = ".$step."\n";
print "xWidth = ".$xWidth."\n";
print "flavour = ".$flavour."\n";
print "charge = ".$charge."\n";



#@masses = (250,260,270,280,290,300,310,320,330,340,350,360,370,380,400,410,420,430,440,450,460,470,480,490,500,510,520,530,540,550,560,570,580,590,600);
#@masses = (300,400,450,500,600);
@masses = (250,300,350,400,450,500,550,600);
$nMasses = @masses;



#########################################################################
### define input dir

$combineFolder = "combine_signal/";
$fileName      = "limitPlot_signal_";
if( $onSidebands == 1 )
{
  $combineFolder = "combine_sidebands/";
  $fileName      = "limitPlot_sidebands_";
}

if( $additionalCuts eq "none" )
{
  $additionalCuts = "";
}
else
{
  $additionalCuts = "_".$additionalCuts;
}
  
$outputDir = $INPUTDir."/".$combineFolder."/binWidth".$xWidth."/step".$step.$additionalCuts."/";






#########################################################################
### loop on toys

for($toyIt = 0; $toyIt < $nToys+1; ++$toyIt)
{
  $toyLabel = "";
  $toyLabel2 = "";
  if( $toyIt > 0 )
  {
    $toyLabel = "_toy".$toyIt;
    $toyLabel2 = "_".$toyParent."_toy";
  }
  
  
  
  if( $analysisMethod eq "sidebands" )
  {
    $jobDir = $outputDir.$fileName.$analysisMethod."_".$flavour.$charge."_".$combineMethod."_".$combineTechnique.$toyLabel2."/";
  }
  else
  {
    $jobDir = $outputDir.$fileName.$analysisMethod."_".$fitFunction."_".$flavour.$charge."_".$combineMethod."_".$combineTechnique.$toyLabel2."/";
  }
  
  
   
  if( (-e $jobDir) && ($toyIt < 2) )
  {
    $command = "rm -rf ".$jobDir;
    system($command);
  }
  
  if( $toyIt < 2 )
  {
    $command = "mkdir ".$jobDir;
    system($command);
  }
  
  $command = "cd ".$jobDir;
  system($command);
  
  
  
  if( $toyIt > 0 )
  {
    $jobDir = $jobDir."/toy".$toyIt."/";
    if( -e $jobDir )
    {
      $command = "rm -rf ".$jobDir;
      system($command);
    }
    
    $command = "mkdir ".$jobDir;
    system($command);
    
    $command = "cd ".$jobDir;
    system($command);
  }
  
  
  
  
  
  
  for($massIt = 0; $massIt < $nMasses; ++$massIt)
  {
    $mass = $masses[$massIt];
    
    $jobFile = $jobDir."job_H".$mass.".sh";
    $outFile = $jobDir."out_H".$mass.".txt";
    
    if( $analysisMethod eq "sidebands")
    {
      $datacard ="datacard_".$analysisMethod."_".$combineTechnique."_".$mass."_".$flavour.$charge.$toyLabel2.".txt";
      $shapes = "shapes_".$analysisMethod."_".$mass."_".$flavour.$charge.$toyLabel2.".root";
    }
    else
    {
      $datacard ="datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_".$flavour.$charge.$toyLabel2.".txt";
      $shapes = "shapes_".$analysisMethod."_".$fitFunction."_".$mass."_".$flavour.$charge.$toyLabel2.".root";
    }
    
    open(JOBFILE, ">", $jobFile);
    
    print JOBFILE "source /gwpool/initcms/slc5_64-cmssw.sh\n";
    print JOBFILE "cd ".$CMSSWDir."\n";
    print JOBFILE "eval `scramv1 runt -sh`\n";
    print JOBFILE "cd ".$jobDir."\n";
    if( !($flavour eq "e_mu") && !($charge eq "p_m") )
    {
      print JOBFILE "cp ".$outputDir."/".$shapes." ./\n";
    }
    if( $toyIt == 0 )
    {
      print JOBFILE "cp ".$outputDir."/".$datacard." ./\n";
    }
    else
    {
      print JOBFILE "cat ".$outputDir."/".$datacard." | sed -e s%TOYPARENT%".$toyParent."%g | sed -e s%TOYIT%".$toyIt."%g > ./".$datacard."\n";
    }
    
    $command = "chmod a+x ".$jobFile;
    system($command);
    
    
    
    $combineCommand = "unbuffer combine -M ".$combineMethod." ".$combineOptions." -n HWWlvjj -m ".$mass." -d ".$datacard." >> ".$outFile."\n";
    print JOBFILE $combineCommand."\n";
    
    
    
    $submit = "";
    if( $toyIt == 0 )
    {
      $submit = "qsub -V -d ".$jobDir." -k n -q shortcms ".$jobFile;
      #$submit = "sh ".$jobFile." \&";
      print ">>>>>>>>> submit: ".$submit."\n";
    }
    else
    {
      $submit = "qsub -V -k n -q longcms ".$jobFile;
    }
    system($submit);
    system("sleep 0.10");
  }
}
