# Test procedury:
# is_upper(s)
#   Sprawdza, czy pierwsza litera napisu s jest wielką literą.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    ok(is_upper("Tak"));
    ok(!is_upper("nie"));
    ok(!is_upper("8nie"));
    ok(!is_upper("   nie"));
}
