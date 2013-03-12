# Test procedury:
# warning(sth)
#   wyświetla sth na strumieniu diagnostycznym (linię zaczyna gwiazdką) 


sub ::build()
{
    $childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{
    info();
}

B:bb => R:rr
{
    info();
}

D:dd => S:ss
{
    info();
}

X => Y
{
    info();
}

sub Y::info()
{
    warning("wezel kategorii Y");
}
sub ::info()
{
    warning("wezel kategorii != Y");
}

