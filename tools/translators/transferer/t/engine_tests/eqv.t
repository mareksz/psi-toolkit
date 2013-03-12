# Test funkcji:
# eqv()
#    daje węzeł-odpowiednik


# Test maszyny reguł transferu

sub ::build()
{
	$childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);

	$childB->insert_left("C", cc);
}

A:aa => P:pp
{
    # 3. sprawdzenie czy kategoria węzła-odpowiednika jest poprawnie określona
    is("A", eqv()->cat());
    is("aa", eqv()->label());
}

B:bb => R:rr
{
    # 2.
    is(@, eqv());
}

C:cc => S:ss
{
    # 1.
    $wezelY := eqv();
    is(@, $wezelY);
}

D:dd => T:tt
{
}

X => Y
{
}





