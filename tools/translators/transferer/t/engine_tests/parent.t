# Test procedury:
# parent()
#   Zwraca węzeł-rodzic bieżącego węzła (lub wartość pustą, jeśli węzeł jest korzeniem).

sub ::build()
{
    $childA := insert_left("A", aa);
    insert_right("B", bb);
    $childA->insert_left("C", cc);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

C:cc => S:ss
{
}

X => Y
{
}

sub Y::default()
{
    is(parent(), null());
    $childP := find("P", pp);
    # dla P rodzicem jest Y
    is(this(), $childP->parent());
}
