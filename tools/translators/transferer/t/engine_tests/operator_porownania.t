# Test operatora:
# =
#   Porównanie


sub ::build()
{
    insert_left("A", aa);
}

A:aa => B:bb
{
}

X => Y
{
    $childP1 := eqv();
    $childP2 := @;
    ok($childP1 = $childP2);
}

sub Y::default()
{
    ok(11 = 11);
    $napis := "kj hkajsdh kjash dkjas hdk";
    is(1, $napis = "kj hkajsdh kjash dkjas " + "hdk");
    is(1, ala + " " + ma + " " + kota = "ala ma kota");
    is(0, ala + " " + "W" = "ala w");
}
