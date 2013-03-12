# Test procedury:
# strip_quotes(s)
#   Zwraca s z usuniętymi początkowymi i końcowymi apostrofami (jeśli tam w ogóle są).

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("usunięte początkowe i końcowe apostrofy", strip_quotes('usunięte początkowe i końcowe apostrofy'));
    is("usunięte początkowe i końcowe apostrofy", strip_quotes("'usunięte początkowe i końcowe apostrofy'"));
    #is("usunięte początkowe i końcowe apostrofy", strip_quotes("'usunięte początkowe i końcowe apostrofy"));
    #is("usunięte początkowe i końcowe apostrofy", strip_quotes("usunięte początkowe i końcowe apostrofy'"));
}
