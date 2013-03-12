# Test procedury:
# replace_with(node)
#   Wycina bieżący węzeł z nadrzędnego drzewa i w jego miejsce wkleja węzeł node.

sub ::build()
{
    $childA := insert_left("A", aa);
    $childB := insert_right("B", bb);
    $childA->insert_left("D", dd);
    $childE := $childA->insert_right("E", ee);
    $childE->insert_left("F", ff);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

D:dd => S:ss
{
}

E:ee => T:tt
{
}

F:ff => W:ww
{
}

X => Y
{
}

sub P::default()
{
    $childT := find("T", tt);
    
    replace_with($childT);
    #is("tt", get("T"));
    
}

sub Y::default()
{
    $childT := find("T", tt);
    $childT2 := $childT->find("T", tt);
    $childW := $childT->find("W", ww);

    ok(!undefined($childT));
    # przy replace wstawiany węzeł zostaje wycięty, a nie powielony
    #ok(!undefined($childT2));

    ok(!undefined($childW));
    is(null(), find("P", pp));
}
