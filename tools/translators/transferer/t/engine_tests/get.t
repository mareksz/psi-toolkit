# Test procedury:
# get(attr)
#   Zwraca wartość atrybutu o nazwie attr. W zasadzie get("X")to to samo, co X , ale get może się przydać, jeśli dynamicznie tworzymy nazwę atrybutu (np. get("Prep" + $number))


sub ::build()
{
    insert_left("A", aa);
}

A:aa => P:pp
{
    $attr := "moj_";
    $ybut := "attr";
    set($attr + $ybut, "tajny");
    is("tajny", get($attr + $ybut));
    is("tajny", get("moj_" + $ybut));
    set("", "");
    is("", get(""));
}

X => Y
{
}
