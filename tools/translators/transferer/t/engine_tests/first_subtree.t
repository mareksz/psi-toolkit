# Test procedury:
# first_subtree()
#   zwraca pierwszy (od lewej) podwęzeł danego węzła


sub ::build()
{
    $childA := insert_left("A",aa);

    $childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);
    $childA->insert_right("E", ee);

    $childE->insert_left("C", cc);
    $childD->insert_left("A", aa);
    $childE->insert_right("F", ff);
    
}

A:aa => P:pp
{
    $childT := find("T", tt);
    is($childT, first_subtree());
}

B:bb => R:rr
{
}

C:cc => S:ss
{
}

D:dd => T:tt
{
}

E:ee => U:uu
{
    $childC := find_right("S", ss);
    is($childC, first_subtree());
}

X => Y
{
}
