# Test operatora:
# -
#   Jednoargumentowy operator negacji arytmetycznej
#   - 1000 ---> -1000

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is(2, 5 + (-3));
}
