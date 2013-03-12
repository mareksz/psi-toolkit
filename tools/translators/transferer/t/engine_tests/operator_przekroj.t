# Testy operatora:
# ^
#   Przekrój dziedzin semantyki
#   hum ^ any   --->   any
#   "-hum" ^ con   --->   con

sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is(act ^ action, action);
    is(group ^ object ^ organization, organization);
    is("living_thing" ^ flower, flower);
    is("living_thing" ^ "substance", NULL);
}
