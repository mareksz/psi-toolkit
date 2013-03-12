# Test operatora:
# &
#   Zwraca listę powstałą w wyniku połączenia list podanych jako argumenty. Argument pusty traktuje jak listę pustą, argument niebędący listą — jak listę jednoelementową.

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
    # przynajmniej jedna wartość musi być != null()
    ok("gruszka" & "pietruszka");
    ok(3 & -234);
    $childP := find("P", pp);
    $childR := find("R", rr);
    ok($childP & $childR);
    ok($childP & "pietruszka");
    ok(null() & "pietruszka");
}
