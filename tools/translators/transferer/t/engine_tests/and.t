# Test operatora:
# and
#   Operator koniunkcji.
#   1 == 1 and 2 == 1   --->   ~
#   2 == 2 and 3 == 2 or "bar" = "bar"   --->   1  


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    ok(1 and 1 and 1);
    is(0, 0 and 1 and 1 and 1);
    is(1, 1 and 1 and 1 and 1);
}
