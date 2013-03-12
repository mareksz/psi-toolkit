# Test procedury:
# pair(first, second)
#   Zwraca parę złożoną z first i second.

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
    ok(pair("gruszka", "pietruszka"));
    ok(pair(3, -234));
    $childP := find("P", pp);
    $childR := find("R", rr);
    ok(pair($childP, $childR));
    ok(pair($childP, "pietruszka"));
}
