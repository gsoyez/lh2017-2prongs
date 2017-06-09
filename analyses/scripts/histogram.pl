#!/usr/bin/perl -w
#
# script to histogram STDIN (taking it as an ntuple), assuming all
# elements have weight 1.
#
$usage = "
 Usage:
   
  histogram.pl [options] lo hi binsize

 Options:
    
   -2d    do 2d histogram (need lo, hi, binsize for 2nd dim too)

   -w     weighted (each entry has two values: position in hist & weight)
          [does not yet work for 2d]

   -norm  normalise the result
";

$norm=0;
$do2d="";
$file="";
$weighted=0;
$cmdline = "$0 ".join(" ",@ARGV);
while ($#ARGV >= 0 && ($ARGV[0] =~ /^-[a-z]/i || $ARGV[0] eq "-2d")) {
   $arg = shift @ARGV;
   if ($arg eq '-norm') {$norm = 1;}
   elsif ($arg eq '-2d') {$do2d = 1;}
   elsif ($arg eq '-weight' || $arg eq "-w") {$weighted = 1;}
   elsif ($arg eq '-f') {$file = shift @ARGV;}
   else {die "unrecognized argument $arg";}
}
if (($do2d && $#ARGV != 5) || (!$do2d && $#ARGV != 2)) {
  print " *** ERROR: incorrect set of arguments!\n $usage";
  exit;
}

$nentries=0;
print "# $cmdline\n";
if ($do2d) {

  # 2d histogram 
  $min1  = $ARGV[0];
  $max1  = $ARGV[1];
  $step1 = $ARGV[2];
  $min2  = $ARGV[3];
  $max2  = $ARGV[4];
  $step2 = $ARGV[5];
  
  $nbins1 = int(($max1-$min1)/$step1);
  $step1 = ($max1-$min1)/$nbins1;
  $nbins2 = int(($max2-$min2)/$step2);
  $step2 = ($max2-$min2)/$nbins2;
  
  @arr = ();
  for ($i1 = 0; $i1 < $nbins1; $i1++) {
    for ($i2 = 0; $i2 < $nbins2; $i2++) {
      $arr[$i1][$i2] = 0;
    }
  }
  
  while (($number1,$number2) = split(" ",<STDIN>)) {
    $bin1 = &floor(($number1-$min1)/$step1);
    $bin2 = &floor(($number2-$min2)/$step2);
    if ($bin1 >= 0 && $bin1 < $nbins1 && $bin2 >= 0 && $bin2 < $nbins2) {$arr[$bin1][$bin2] += 1;}
    $nentries++;
  }
  
  for ($i1 = 0; $i1 < $nbins1; $i1++) {
    for ($i2 = 0; $i2 < $nbins2; $i2++) {
      print 
	  $min1+$i1*$step1, " ", $min1+($i1+0.5)*$step1, " ", $min1+($i1+1.0)*$step1, " ",
	  $min2+$i2*$step2, " ", $min2+($i2+0.5)*$step2, " ", $min2+($i2+1.0)*$step2, " ",
	  $arr[$i1][$i2], "\n";
    }
    print "\n";
  }
  print STDERR "N entries = $nentries\n";

} else {
  # 1d histogram 
  $min  = $ARGV[0];
  $max  = $ARGV[1];
  $step = $ARGV[2];
  
  $nbins = int(($max-$min)/$step);
  $step = ($max-$min)/$nbins;
  
  @arr = ();
  @totalweight = ();

  $ncols=0;
  while ($line = <STDIN>) {
    if ($line =~ /^#/) {print $line; next;}
    @numbers = split(" ",$line);
    # figure out how many things we're histogramming
    # and do the initialisation
    if (!$ncols) {
      $ncols = $#numbers + 1;
      # with weighted histogramming, we have one column for 
      # histogram position and a second column for the event
      # weight
      if ($weighted) {$ncols /= 2};
      for ($col = 0; $col < $ncols; $col++) {
	for ($i = 0; $i < $nbins; $i++) {$arr[$col][$i] = 0;}
	$sum[$col]=0;
	$sumsqr[$col]=0;
	$totalweight[$col]=0;
      }
    }
    # then do the binning 
    for ($col = 0; $col < $ncols; $col++) {
      my $weight;
      if ($weighted) {
	$number = $numbers[2*$col];
	$weight = $numbers[2*$col+1];
      } else {
	$number = $numbers[$col];
	$weight = 1;
      }
      $sum[$col]    += $number * $weight;
      $sumsqr[$col] += $number*$number * $weight;
      $totalweight[$col] += $weight;
      $bin = &floor(($number-$min)/$step);
      if ($bin >= 0 && $bin < $nbins) {$arr[$col][$bin] += $weight;}
    }
    $nentries++;
  }
  
  for ($col = 0; $col < $ncols; $col++) {
    if ($norm) {$norm[$col] = 1.0/$totalweight[$col]} 
    else       {$norm[$col] = 1.0}
  }

  for ($i = 0; $i < $nbins; $i++) {
    print $min+$i*$step, " ", $min+($i+0.5)*$step, " ", $min+($i+1.0)*$step, " ";
    for ($col = 0; $col < $ncols; $col++) {
      #print STDERR "$ncols $col $i $arr[$col][$i]\n";
      print $norm[$col]*$arr[$col][$i], " ";
    }
    print "\n";
  }
  for ($col = 0; $col < $ncols; $col++) {
    $av = $sum[$col]/$totalweight[$col];
    $stddev = sqrt($sumsqr[$col]/$totalweight[$col] - $av**2);
    # error calculation is now quite right?
    print STDERR "N entries = $nentries, average = $av +- ",$stddev/sqrt($nentries)," std.dev = $stddev +-",$stddev/sqrt($nentries),"\n";
    print "# N entries = $nentries, average = $av +- ",$stddev/sqrt($nentries)," std.dev = $stddev +-",$stddev/sqrt($nentries),"\n";
  }
}


sub floor($) {
  (my $val) = @_;
  if ($val >= 0.0) {return int($val);}
  else             {return int($val-1.0);}
}
