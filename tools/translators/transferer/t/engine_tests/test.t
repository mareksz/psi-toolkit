# Test maszyny reguł transferu

sub ::build()
{
    insert_left("A",e);
}

A => B
{
    
}

:e => :f
{

}

X => Y
{
    ok(1);
    
    ok(find_left("B",f));
}

sub Y::default()
{
    is(2+2, 4);
}

