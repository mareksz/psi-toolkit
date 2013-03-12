# Test procedury:
# nounderscores(s)
#   Zwraca napis s, w którym ewentualne podkreślniki zastąpiono spacjami.


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is("          ", nounderscores("_____     "));
    is("@$#%^&$# ść", nounderscores("@$#%^&$#_ść"));
    is("bez podkreślników", nounderscores("bez podkreślników"));
}
