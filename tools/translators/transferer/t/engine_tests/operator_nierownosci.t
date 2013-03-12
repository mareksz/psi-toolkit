# Test operatora:
# !=
#   Operator nierówności
#   2 != 2  ---> ~


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
    ok($childP1 != this());
}

sub Y::default()
{
    ok(11 != 10);
    $napis := "Test operatora nierówności.";
    is(1, $napis != "Musi być różne" + ". I jest.");
    is(1, ala + " " + ma + " " + kota != "Ala ma kota");
}
