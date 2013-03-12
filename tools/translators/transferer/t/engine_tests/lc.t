# Test procedury:
# lc(s)
#   Zwraca napis s sprowadzony do małych liter.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("tekst różnie pisany", lc("TekSt RÓżNiE pisaNy"));
    $napis := "TekSt RÓżNiE pisaNy";
    is("tekst różnie pisany", lc($napis));
    ok("tekst różnie pisany jest" != lc($napis));
}
