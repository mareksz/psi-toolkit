# Test operatora:
# +
#   Dla liczb - operator dodawania, dla napisów - konkatenacji.
#   2+2  --->  4
#   "2"+"2"  --->  "22"
#   "will"+"be"  ---> "willbe"


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is(5, 2 + 3);
    is("napis", "na" + "pis");
    $czesc1 := "Karmelkowy";
    $czesc2 := koliberek;
    is("Karmelkowy koliberek", $czesc1 + " " + $czesc2);
    is("2jupi", 2 + jupi);
    is(jupi2, jupi + 2);
}
