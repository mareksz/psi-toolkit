# Test operatora:
# :=
#   Operator przypisania. Po prawej stronie musi znajdować się zmienna lub atrbybut. Wartością przypisania jest przypisana wartość.
#   $X := 10   --->   10

sub ::build()
{
    $childA := insert_left("A", aa);
    $childA.insert_left("B", bb);
}

A:aa => P:pp
{
    $childR := find("R", rr);
    is($childR.label(), rr);
    is($childR.cat(), "R");
}

B:bb => R:rr
{
}

X => Y
{
}

sub Y::default()
{
    $licznik := 12;
    is(12, $licznik);
}
