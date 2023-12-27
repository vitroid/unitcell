#!/usr/bin/env perl

$mw=18.0;
$n=12;
$dens=1.0;


$x=2.42;
$y=2.42;
$z=2.46;
$vol=$mw*$n/(6.022e23*1e-24*$x*$y*$z);
$vol=exp((1/3)*log($vol));
$x*=$vol;
$y*=$vol;
$z*=$vol;

print "\@BOX3\n";
print "$x $y $z\n";
print "\@AR3A\n";
print "12\n";
$x=$vol*0.175012;
$y=$vol*0.678203;
$z=$vol*1.092083;
print "$x $y $z\n";
$x=$vol*0.661741;
$y=$vol*1.171998;
$z=$vol*-0.141727;
print "$x $y $z\n";
$x=$vol*1.154621;
$y=$vol*1.658145;
$z=$vol*1.082531;
print "$x $y $z\n";
$x=$vol*-0.054974;
$y=$vol*-0.038142;
$z=$vol*0.471574;
print "$x $y $z\n";
$x=$vol*1.871795;
$y=$vol*1.888202;
$z=$vol*1.701086;
print "$x $y $z\n";
$x=$vol*0.661795;
$y=$vol*0.191634;
$z=$vol*-0.147263;
print "$x $y $z\n";
$x=$vol*1.154810;
$y=$vol*0.677872;
$z=$vol*1.088271;
print "$x $y $z\n";
$x=$vol*1.642144;
$y=$vol*1.171824;
$z=$vol*-0.137704;
print "$x $y $z\n";
$x=$vol*1.384722;
$y=$vol*-0.037855;
$z=$vol*0.467355;
print "$x $y $z\n";
$x=$vol*1.871779;
$y=$vol*0.448143;
$z=$vol*1.708231;
print "$x $y $z\n";
$x=$vol*-0.055209;
$y=$vol*1.402043;
$z=$vol*0.478055;
print "$x $y $z\n";
$x=$vol*0.431764;
$y=$vol*1.887935;
$z=$vol*-0.762492;
print "$x $y $z\n";