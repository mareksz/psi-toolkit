# Test funkcji:
# delete(cat_or_node,label)
	# usuwa pierwszy podwęzeł o kategorii cat_or_node i etykiecie label, argument cat_or_node może być po prostu węzłem (a nie kategorią) - wówczas ten węzeł jest usuwany


sub ::build()
{
	$childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);
	$childD := $childA->insert_left("D", dd);
	$childA->insert_right("E", ee);

	$childB->insert_left("C", cc);
	$childD->insert_left("A", aa);

insert_left("A",aa);
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

D:dd => T:tt
{
}

E:ee => U:uu
{
}


X => Y
{
	# usuwanie jednego z potomnych wezlow korzenia - istnieja dwa wezly o tej samej etykiecie - jak sa wybierane?
	ok(delete("P", pp));
	#ok(find("P", pp));

	# usuwanie R (z calym poddrzewem)
	$childP := find("P", pp);
	ok($childP->delete("R", rr));

	# usuwanie wierzcholka, ktorego nie ma
	ok(!$childP->delete("R", rr));

	# usuwanie wierzcholka o blednie zapisanej etykiecie
	ok(!delete("P", ppZBledem));

}
