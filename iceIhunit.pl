#!/usr/bin/env perl

$mw=18.0;
$n=8;
$dens=1.0;
if($#ARGV==0){
    $dens=$ARGV[0];
}

$x=sqrt(8/3);
$y=sqrt(8);
$z=8/3;
$vol=$mw*$n/($dens*6.022e23*1e-24*$x*$y*$z);
$vol=exp((1/3)*log($vol));
print STDERR "$vol O-O length\n";
$x*=$vol;
$y*=$vol;
$z*=$vol;

print "\@BOX3\n$x $y $z\n";
print "\@AR3A\n$n\n";
($a,$b,$c)=(1/4,1/6,3/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(3/4,2/6,5/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(1/4,1/6,13/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(3/4,2/6,11/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(3/4,4/6,3/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(1/4,5/6,5/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(3/4,4/6,13/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
($a,$b,$c)=(1/4,5/6,11/16);
print $a*$x." ".$b*$y." ".$c*$z."\n";
