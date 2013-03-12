# Test operatora:
# <
#   Lewy argument jest podzbiorem (w sensie dziedzin semantyki) prawego argumentu.
#   "human" < "animate" --->  1
#   "-animate" < "-human"  ---> 1
#   "con" <  "con"   ---> 1
#   "number" <  "human"  ---> ~


sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    is(1, flower < living_thing);
    is(1, "-living_thing" < "-flower");
    is(1, flower < flower);
    is(0, living_thing < flower);
    is(1, "river" < body_of_water);
}
