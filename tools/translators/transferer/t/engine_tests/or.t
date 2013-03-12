# Test operatora:
# or
#   Alternatywa logiczna.


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    ok(1 or 1 or 0);
    is(1, (0 or 0) or (0 or 1));
}
