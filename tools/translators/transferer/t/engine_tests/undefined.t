# Test procedury:
# undefined(v)
#   Sprawdza, czy v jest niezdefiniowane.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    $zmienna := "chrząszcz";
    ok(!undefined($zmienna));
    $zmienna := null();
    ok(undefined($zmienna));
    ok(undefined($gfchjnk));
    ok(!undefined("lskjdlaksdla"));
}
