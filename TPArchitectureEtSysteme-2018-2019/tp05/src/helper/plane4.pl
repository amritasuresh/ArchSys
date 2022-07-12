open FD,"high.csv";
while (<FD>)
{
	tr/\015\012//d;
	$h{hex $_} = "";
}
close FD;

$c = 0;
for ($i = 0; $i < 0x300; $i++)
{
	if (defined($h{$i})) { $r = ++$c } else { $r = 0; }
	printf "%2d, ",$r;
	print "\n" if $i % 16 == 15;
}

open FD,"characters.csv";
<FD>;
while (<FD>)
{
	tr/\015\012//d;
	@f = split(/;/);
	$id = shift @f;
	$ch{hex $id} = join(";",@f);
}
close FD;

$textlen = 0;
@txt = ();

open FD,"high.csv";
while (<FD>)
{
	tr/\015\012//d;
	$p = $_;
	$h = 256*hex $p;
	$p =~ s/^0*//;
	$p = "00".$p if length($p) == 0;
	$p = "0".$p if length($p) == 1;
	printf "\t/* block %d (%sxx) */\n\t\t",++$b,$p;
	for ($i = 0; $i <= 255; $i++)
	{
		unless (defined($ch{$h+$i}))
		{
			print "     0,     ";
			print "\n" if ($i % 4 == 3);
			print "\t\t" if ($i % 4 == 3 && $i != 255);
			next;
		}

		$f = $ch{$h+$i};
		($ccc,$fce,$dec,$cpt,undef) = split(/;/,$f);
		$len = 0; $pos = 0;
		$decbit = 0;
		$decbit = 1 if $cpt;
		pushtext($dec) if $dec;
		pushtext($cpt) if $cpt;
		$r = ($ccc << 24) + ($fce << 23) + ($decbit << 22)
				+ ($len << 16) + $pos;
		printf "0x%08x, ",$r;
		print "\n" if ($i % 4 == 3);
		print "\t\t" if ($i % 4 == 3 && $i != 255);
	}
	print "\n";
}
close FD;

$txt = join(", ",map { sprintf "0x%04x",$_ } @txt);
$txt =~ s/(.{79}) /$1X/g;
$txt =~ tr/X/\012/;
print $txt;

exit 0;

sub pushtext {
	my $txt = shift;
	my @cps = split(/ /,$txt);
	$len = $#cps+1;
	$pos = $textlen;
	for (@cps) { push_utf16($_); }
}

sub push_utf16
{
	my $cp = hex shift;
	
	if ($cp < 0x10000) { push @txt,$cp; $textlen++; return; }

	push @txt, (0xd800 + (($cp >> 10)-0x40));
	push @txt, (0xdc00 + ($cp & 0x3ff));
	$textlen += 2;
}
