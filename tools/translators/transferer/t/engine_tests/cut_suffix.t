# Test procedury:
# cut_suffix(s, len)
#   zwraca napis s z uciętymi len znakami (od końca), uwaga: len może być podane zarówno jako liczba (5), jak i jako napis ("5")

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("Lśniący ", cut_suffix("Lśniący lamparcik", 9));
    is("Jurny ", cut_suffix("Jurny  Jarząbekżęó", 12));
    is("Ubuntu", cut_suffix("Ubuntu", 0));
    $napis := "Ubuntu";
    is($napis, cut_suffix("Ubuntu", 0));
}
