# Test procedury:
# tree_representation()
#   Zwraca tekstową reprezentację drzewa.

sub ::build()
{
    $childA := insert_left("A", aa);
    $childB := insert_right("B", bb);
    $childB := $childA->insert_left("B", bb);
    $childA->insert_right("C", cc);
    $childB->insert_left("A", aa);
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
    is("Y[P[R[P()],S()],R()]", tree_representation());
}
