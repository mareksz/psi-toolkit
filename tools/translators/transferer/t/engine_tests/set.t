# Test procedury:
# set(attr, val)
#   Ustawia wartość atrybutu attr na val. W zasadzie set("X", v) jest równoważne X := v, ale set może się przydać, jeśli nazwa atrybutu jest generowana dynamicznie.

sub ::build()
{
    insert_left("A", aa);
}

A:aa => P:pp
{
    $attrybut := "Moj_attr";
    set($attrybut, "numer1");
    is("numer1", this()->Moj_attr);

    # ponowna zmiana wartośc atrybutu
    set($attrybut, "numer2");
    is("numer2", this()->Moj_attr);
}

X => Y
{
}

sub Y::default()
{
    # set zwraca wartość ustawionego atrybutu
    is(set("Majshdj", "jdskf"), "jdskf");
}
