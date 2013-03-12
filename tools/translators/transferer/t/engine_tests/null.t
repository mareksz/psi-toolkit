# Test procedury:
# null()
#   Zwraca wartość pustą.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is($bzdura, null());
    is(parent(), null());
}
