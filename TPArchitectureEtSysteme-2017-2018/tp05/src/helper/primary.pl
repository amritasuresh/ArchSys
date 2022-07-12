$BUCKS = 0x1000;
$ACCS = 64;

for ($i = 0; $i < $BUCKS; $i++)
{
	$hpop[$i] = "0" x $ACCS; $hc[$i] = 0; $k{$i} = ""; $dpl[$i] = 0;
}

open FD,"primary.csv";
while (<FD>)
{
	tr/\015\012//d;
	($id,$first,$last) = split(/,/);
	$high = hex $first & ($BUCKS-1);
	$low = hex $last & ($ACCS-1);
	$hc[$high]++;
	substr($hpop[$high],$low,1) = "1";
	#print "%05x",($high<<8)+$low;
}
close FD;

@idx = sort { $hc[$b] <=> $hc[$a] } keys %k;

#for $i (@idx)
#	{ printf "%03x => %s (%d)\n",$i,$hpop[$i],$hc[$i]; }

$pop = ("0" x $ACCS);
$len = $ACCS;

for $i (@idx)
{
	last if $hc[$i] == 0;
	for ($j = 0; $j <= $len-$ACCS; $j++)
	{
		next unless fits($i,$j);
		$dpl[$i] = $j;
		#printf "%03x >> %d (pop = %d)\n",$i,$j,$hc[$i];
		fit($i);
		$newlen = $j + 2*$ACCS;
		if ($newlen > $len)
		{
			$len = $newlen;
			$pop = substr($pop . ("0" x $ACCS),0,$len);
		}
		last;
	}
}

sub fits
{
	my ($idx,$d) = @_;
	my @f = split(//,$hpop[$idx]);
	my @g = split(//,substr($pop,$d,$ACCS));
	for (my $k = 0; $k < $ACCS; $k++)
	{
		next if $f[$k] eq "0";
		next if $g[$k] eq "0";
		return 0;
	}
	return 1;
}

$len -= $ACCS;
$pop = substr($pop,0,$len);
#print $pop,"\n";

#for ($i = 0; $i < $BUCKS; $i++)
#	{ printf "%03x => %d (pop = %d)\n",$i,$dpl[$i],$hc[$i]; }

print "ushort dpl[] = {\n";
for ($i = 0; $i < $BUCKS; $i++)
{
	printf "%4d,",$dpl[$i];
	print "\n" if $i % 16 == 15;
}

print "};\n\n";

for ($i = 0; $i < $len; $i++)
{
	$id[$i] = 0;
	$first[$i] = 0;
	$last[$i] = 0;
	$used[$i] = 0;
}

open FD,"primary.csv";
while (<FD>)
{
	tr/\015\012//d;
	($id,$first,$last) = split(/,/);
	$high = hex $first & ($BUCKS-1);
	$low = hex $last & ($ACCS-1);
	$idx = $dpl[$high] + $low;
	$id[$idx] = $id;
	$first[$idx] = $first;
	$last[$idx] = $last;
	die if $used[$idx];
	$used[$idx] = 1;
}
close FD;

print "uint primid[] = {\n";
for ($i = 0; $i < $len; $i++)
{
	printf "0x%05x,",hex $id[$i];
	print "\n" if $i % 10 == 9;
}
print "};\n\nuint primfst[] = {\n";
for ($i = 0; $i < $len; $i++)
{
	printf "0x%05x,",hex $first[$i];
	print "\n" if $i % 10 == 9;
}
print "};\n\nuint primlst[] = {\n";
for ($i = 0; $i < $len; $i++)
{
	printf "0x%05x,",hex $last[$i];
	print "\n" if $i % 10 == 9;
}
print "};\n\n";

sub fit
{
	my ($idx) = @_;
	my @f = split(//,$hpop[$idx]);
	for (my $k = 0; $k < $ACCS; $k++)
	{
		next if $f[$k] eq "0";
		substr($pop,$dpl[$i]+$k,1) = "1";
	}
}
