# Test procedury:
# ordnum2pltext(s)
#   Dla danego napisu s reprezentującego liczbę (zapisaną cyframi arabskimi) zwraca parę, której pierwszy element to polski wyraz lub fraza odpowiadająca liczbie s, zaś drugim elementem jest opis sposobu odmiany wyrazu/frazy. Uwaga! Oczekiwanym typem argumentu jest napis, nie — liczba.

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("drugi", first(ordnum2pltext("2")));
    is("milion stutysięczny", first(ordnum2pltext("1100000")));
    is("dwa tysiące dziesiąty", first(ordnum2pltext("2010")));
    is("trzydziesty trzeci", first(ordnum2pltext("33")));
    is(second(ordnum2pltext("81")), second(ordnum2pltext("33")));

    is("piąty", first(ordnum2pltext("5")));
}
