# Test procedury:
# insert_before(cat,label)
#   Przed bieżącym węzłem wstawia (w węźle-rodzicu) siostrzany węzeł o danej kategorii i etykiecie.

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
    insert_before("M", "etykietka");
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
    $childT := find("T", tt);

    ok(!$childS->precedes($childM));
    ok($childM->precedes($childS));
    ok($childM->precedes($childT));
}
