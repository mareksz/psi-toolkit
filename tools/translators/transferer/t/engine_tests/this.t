# Test funkcji:
# this()
#    bieżący węzeł


sub ::build()
{
    $childA := insert_left("A",aa);

    $childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{  
    # jak porównać węzły. Prawdopodobnie porównanie kategorii i etykiety węzła nie wystarcza.
    is("P", this()->cat());
    is("pp", this()->label());
}

B:bb => R:rr
{
}

D:dd => S:ss
{
}

X => Y
{
}
