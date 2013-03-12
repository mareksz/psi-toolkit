# Test funkcji:
# insert_after(cat,label)
#   Za bieżącym węzłem wstawia (w węźle-rodzicu) siostrzany węzeł o danej kategorii i etykiecie.

sub ::build()
{
    $childA := insert_left("A", aa);
    $childB := insert_right("B", bb);
    $childA->insert_left("D", dd);
    $childA->insert_right("E", ee);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

D:dd => S:ss
{
    insert_after("M", "etykietka");
}

E:ee => T:tt
{
}

X => Y
{
}

sub P::default()
{
    $childS := find("S", ss);
    $childM := find("M", "etykietka");
    ok($childS->precedes($childM));
    ok(!$childM->precedes($childS));

    $childT := find("T", tt);
    ok($childM->precedes($childT));
    
    # nie można podawać warości NULL
    #$childNull := find("We", we);
    #ok(!$childM->precedes($childNull));
}
