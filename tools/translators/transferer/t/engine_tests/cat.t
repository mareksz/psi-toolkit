# Test funkcji:
# cat()
	# daje kategorię węzła

sub ::build()
{
	$child1 := insert_left("A",aa);
	
	$child2 := $child1->insert_left("B", bb);
	$child3 := $child1->insert_left("C", cc);
}

A:aa => P:pp
{
	is("P", cat());
}

B:bb => R:rr
{
	is("R", cat());
}

C:cc => S:ss
{
	is("S", cat());
}


X => Y
{
	is("Y", cat());
	
	# sprawdzenie nazwy dla odszukanego wierzcholka
	$childP := find_left("P", pp);
	is("P", $childP->cat());
}


