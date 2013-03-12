# Test procedury:
# first(pair)
#   zwraca pierwszy element pary pair 

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
    is("gruszka", first(pair("gruszka", "pietruszka")));
    is(3, first(pair(3, -234)));
    $childP := find("A", aa);
    $childR := find("B", bb);
    is($childP, first(pair($childP, $childR)));
    is($childP, first(pair($childP, "pietruszka")));
}
