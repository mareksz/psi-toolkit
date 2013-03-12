# Test funkcji:
# change_category(newc)
	# zmienia kategorię bieżącego węzła na newc

sub ::build()
{
	$child1 := insert_left("A",aa);
	insert_left("D", dd);
	$child2 := $child1->insert_left("B", bb);
	$child3 := $child1->insert_left("C", cc);
}

A:aa => P:pp
{
	change_category("Zmiana1");
	is("Zmiana1", cat());
}

B:bb => R:rr
{
	change_category("liga mistrzów");
	is("liga mistrzów", cat());

	# czy mozna to tak wywolac???
	#is("liga mistrzów", change_category("liga mistrzów"));
}

C:cc => S:ss
{
}

D:dd => T:tt
{
}

X => Y
{
}


