# Test procedury:
# second(pair)
#   zwraca drugi element pary pair  

sub ::build()
{
    insert_left("A", aa);
    insert_left("B", bb);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

X => Y
{
}

sub Y::default()
{
    is("pietruszka", second(pair("gruszka", "pietruszka")));
    is(-234, second(pair(3, -234)));
    $childP := find("A", aa);
    $childR := find("B", bb);
    is($childR, second(pair($childP, $childR)));
    is("pietruszka", second(pair($childP, "pietruszka")));
}
