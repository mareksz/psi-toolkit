# Test procedury:
# change_label(newl)
#    zmienia etykietę bieżącego węzła na newl, zwraca tenże bieżący węzeł


sub ::build()
{
    $childA := insert_left("A",aa);

	$childB := $childA->insert_left("B", bb);
	$childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{
    # ważna kolejność wykonywania porównania (najpierw zmiana, później sprawdzenie wartości, nie odwrotnie)!
    is(change_label("nowa"), this());
}

B:bb => R:rr
{
    is(change_label("etykietka")->label(), "etykietka");
    is(change_label("")->label(), "");
}

D:dd => S:ss
{
}

X => Y
{
}
