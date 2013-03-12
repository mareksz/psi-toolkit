# Test procedury:
# ucfirst(s)
#   Zwraca napis s z powiększoną pierwszą literą.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    $napis := klsdjlkjflskdlfjanbdkfnb;
    is("Klsdjlkjflskdlfjanbdkfnb", ucfirst($napis));
    is("Pierwsza litera", ucfirst("pierwsza litera"));
    is("DCGFNKJ", ucfirst("DCGFNKJ"));
    is('pierwsza litera', ucfirst('pierwsza litera'));
    is("8pierwsza litera", ucfirst("8pierwsza litera"));
}
