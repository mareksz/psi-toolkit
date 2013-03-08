#!/usr/bin/perl

$id = $ARGV[0] || 'i2morpho';

for$P(0..10)
{
    for$r(0..5)
    {
	for$l(0..2)
	{
	    for$o(0..3)
	    {
		for$p(0..7)
		{
		    $M->[$P][$r][$l][$o][$p] = 'X';
		}
	    }
	}
    }
}

$P = -1;

while(<STDIN>)
{
    chomp;

    if(/^\s*\#(.*)$/)
    {
	++$P;
	$kom[$P] = $1;
    }
    elsif(/^(\d)(\d)(\d)(\d):\s*(.*\S)\s*$/)
    {
	$r = $1;
	$l = $2;
	$o = $3;
	$p = $4;
	$m = $5;
	
	$M->[$P][$r][$l][$o][$p] = $m;	

	@vr = ($r);
	@vr = (0..5) if $r == 0;
	@vl = ($l);
	@vl = (0..2) if $l == 0;
	@vo = ($o);
	@vo = (0..3) if $o == 0;
	@vp = ($p);
	@vp = (0..7) if $p == 0;

	for$ir(@vr)
	{
	    for$il(@vl)
	    {
		for$io(@vo)
		{
		    for$ip(@vp)
		    {
			$M->[$P][$ir][$il][$io][$ip] = $m;	
		    }
		}
	    }
	}
    }
}

$maxp = $P;

print "static const int ", uc($id), "_MAX = ", $maxp, ";\n";
print "static const char* ${id}[",$maxp+1,"][6][3][4][8] = {\n";

for$P(0..$maxp)
{
    print "// $kom[$P]\n";
    print "{\n";

    for$r(0..5)
    {
	print "\t{\n";

	for$l(0..2)
	{
	    print "\t\t{\n";

	    for$o(0..3)
	    {
		print "\t\t\t{";

		for$p(0..7)
		{
		    print '"',$M->[$P][$r][$l][$o][$p],'"';
		    print ", " if $p < 7;
		}

		print "}";
		print "," if $o < 3;

		print "\n";
	    }

	    print "\t\t}";
	    print "," if $l < 2;
	    print "\n";
	}

	print "\t}";
	print "," if $r < 5;
	print "\n";
    }
    
    print "}";
    print "," if $P < $maxp;
    print "\n";  
}

print "}\n";

