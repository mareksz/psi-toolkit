# Test procedury:
# surface()
#   domyślna procedura infleksji - zwraca Equiv lub konkatenacje surface() dla wszystkich podwęzłów

#   zwraca kategorię liścia, o ile Equiv nie jest ustawione

sub ::build()
{
    $childA := insert_left("A",aa);

    $childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);    
}

A:aa => P:pp
{
    this()->Equiv := "domyslna";
    is(surface(), "domyslna");
}

B:bb => R:rr
{
    is(surface(), "R");
}

D:dd => T:tt
{
    Equiv := "infleksji";
    is(surface(), "infleksji");
}

X => Y
{
}

sub Y::default()
{
    is(surface(), null());
}
