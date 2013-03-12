# Test operatora:
# ->
#   Odwołanie się do atrybutu węzła lub wywołanie procedury na rzecz węzła.
#   $X->Num
#       atrybut Num węzła wskazywanego przez zmienną $X
#   @->doSomething("a", 1)
#       wywołanie procedury doSomething na rzecz węzła-odpowiednika biężącego węzła


sub ::build()
{
    $childA := insert_left("A",aa);
    $childB := $childA->insert_left("B", bb);
    $childD := $childA->insert_left("D", dd);
}

A:aa => P:pp
{
    is(pp, this()->label());
    is(change_category("Nowa kat.")->cat(), "Nowa kat.");

    this()->L := 12;
    this()->P := mian;
    is(this()->L, 12);
}

B:bb => R:rr
{
    is(@->cat(), "B");
}

D:dd => T:tt
{
}

X => Y
{
    $childP := find("Nowa kat.", pp);

    $childR := $childP.find("R", rr);
    is($childR, this()->find("Nowa kat.", pp)->delete("R", rr));
}
