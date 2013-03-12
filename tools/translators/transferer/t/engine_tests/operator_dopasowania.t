# Test funkcji:
# =~
#   Dopasowanie do wyrażenia regularnego. Prawym argumentem musi być wyrażenie regularne. Wartością wyrażenie jest 1 wgdy napis po lewej stronie został dopasowany do wyrażenia regularnego. Efektem ubocznym wyrażenia jest przypisanie zmiennym $1-$9 fragmentów napisów, które zostały dopasowane do części wyrażenia regularnego ujętych w nawiasy okrągłe.
#   "warszawa" =~ /zawa$/   --->    1



sub ::build()
{
}

X => Y
{
}

sub Y::default()
{
    ok("lśniący lamparcik" =~ /((l)śn(i)ący) \2.*\3k/);
    is($1, lśniący);
    is(1, "karmelkowy koliberek" =~ /karmelkowy/);
    is(0, "Hoża Hawajka" =~ /hoża hawajka/);
    $napis := "Jurny Jarząbek";
    ok($napis =~ /Jurny /);
}
