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



if( @ARGV != 3 )
{
  print(">>> combineDatacards.pl::usage   perl combineDatacards.pl   params.CFG   flavour   charge\n");
  exit;
}

$flavour = $ARGV[1];
$charge  = $ARGV[2];

if( !($flavour eq "e_mu") )
{
  $flavour = "_".$flavour;
}

if( $charge eq "pm" )
{
  $charge = "";
}
if ( !($charge eq "pm") && !($charge eq "p_m") )
{
  $charge = "_".$charge;
}

#@masses = (250,260,270,280,290,300,310,320,330,340,350,360,370,380,400,410,420,430,440,450,460,470,480,490,500,510,520,530,540,550,560,570,580,590,600);
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
  
  
  
  if( ($flavour eq "e_mu") && !($charge eq "p_m" ) )
  {
    $datacard_e    = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e".$charge.".txt";
    $datacard_mu   = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_mu".$charge.".txt";
    $datacard_e_mu = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e_mu".$charge.".txt";
    
    $combineCommand = "combineCards.py ".$datacard_e." ".$datacard_mu." > ".$datacard_e_mu."\n";
    print JOBFILE $combineCommand."\n";
  }
  
  
  
  if( !($flavour eq "e_mu") && ($charge eq "p_m") )
  {
    $datacard_p   = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass.$flavour."_p.txt";
    $datacard_m   = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass.$flavour."_m.txt";
    $datacard_p_m = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass.$flavour."_p_m.txt";
    
    $combineCommand = "combineCards.py ".$datacard_p." ".$datacard_m." > ".$datacard_p_m."\n";
    print JOBFILE $combineCommand."\n";
  }
  
  
  
  if( ($flavour eq "e_mu") && ($charge eq "p_m") )
  {
    $datacard_e_p      = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e_p.txt";
    $datacard_e_m      = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e_m.txt";
    $datacard_mu_p     = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_mu_p.txt";
    $datacard_mu_m     = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_mu_m.txt";
    $datacard_e_mu_p_m = $outputDir."/datacard_".$analysisMethod."_".$fitFunction."_".$combineTechnique."_".$mass."_e_mu_p_m.txt";
    
    $combineCommand = "combineCards.py ".$datacard_e_p." ".$datacard_e_m." ".$datacard_mu_p." ".$datacard_mu_m." > ".$datacard_e_mu_p_m."\n";
    print JOBFILE $combineCommand."\n";
  }
}
