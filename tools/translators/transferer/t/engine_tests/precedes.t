# Test funkcji:
# precedes(node)
#    sprawdza czy węzeł poprzedza węzeł node (z punktu widzenia węzła rodzicielskiego) - ma sens tylko dla wyrażeń w konstrucjach %Typ [...]

sub ::build()
{
    $childA := insert_left("A",aa);
    
    $childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{  
    $childR := find("R", rr);
    is("R", $childR->cat());
    $childS := find("S", ss);
    is("S", $childS->cat());
    ok($childS->precedes($childR));
    ok(not($childR->precedes($childS))); 
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
