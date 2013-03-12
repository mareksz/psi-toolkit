# Test procedury:
# suffix(s, len)
#   Zwraca końcówkę napisu s o długości len, czyli to samo, co substring(s,-len,). 

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("długości len...", suffix("Zwraca koncówkę napisu s o długości len...", 15));
    is("ługości len...'", suffix("'Zwraca koncówkę napisu s o długości len...'", 15));
    is("", suffix("Zwraca koncówkę napisu s o długości len...", 0));
    is("napisu", suffix("napisu", 20));
}

