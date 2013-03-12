# Test funkcji:
# default()
#    domyślna procedura modyfikacji (nic nie robi)


sub ::build()
{
    $childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);
	$childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

D:dd => S:ss
{
}

X => Y
{
}

sub Y::default()
{
    ok(1);
    ok(!0);
}

