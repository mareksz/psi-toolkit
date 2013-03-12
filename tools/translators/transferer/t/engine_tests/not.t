# Test operatora:
# not
#   Jednoargumentowy operator negacji logicznej
#   not 0  --->  1


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    ok(1);
    ok(not not 1);
    ok(not( not 1));
    is(0, not not not 1);
}
