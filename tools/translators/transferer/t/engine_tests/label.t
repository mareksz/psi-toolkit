# Test funkcji:
# label()
#    daje etykietę węzła

sub ::build()
{
	$childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);
	$childD := $childA->insert_left("D", dd);
	$childA->insert_right("E", ee);

	$childB->insert_left("C", cc);
	$childD->insert_left("A", aa);

    insert_left("A",aa);
}

A:aa => P:pp
{
    # 1, 2, 5. węzeł o tej kategorii jest wstawiony do drzewa więcej niż jeden raz (3 razy). Dlatego test wykonany jest więcej niż jeden raz (3 razy).
    is(pp, label());
}

B:bb => R:rr
{
    # 4
    is(bb, @->label());
}

C:cc => S:ss
{
    # 3.
    is(ss, label());
}

D:dd => T:tt
{
}

E:ee => U:uu
{
}

:xx => :yy
{
}

X => Y
{
    # 6.
    $childP := find("P", pp);
    is(pp, $childP->label());
}
