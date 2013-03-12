# Test procedury:
# get_option(opt)
#   Zwraca wartość opcji opt. Opcje ustawiane są na zewnątrz interpretatora reguł, nie można ich ustawiać za pomocą reguł.

sub ::build()
{
    $childA := insert_left("A", aa);
    $childB := insert_right("B", bb);
    $childB := $childA->insert_left("B", bb);
    $childA->insert_right("C", cc);
    $childB->insert_left("A", aa);
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


X => Y
{
}

sub Y::default()
{
    ok(undefined(get_option(opt)));
}
