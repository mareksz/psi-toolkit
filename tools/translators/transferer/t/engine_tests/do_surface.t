# Test procedury:
# do_surface()
#   uruchamia generowanie postaci powierzchniowej

# wygenerowanie postaci powierzchniowej
# węzły drzewa po stronie docelowej są przeglądane począwszy od korzenia (top-down). Dla danego węzła wywoływana jest funkcja surface. Jeśli funkcja surface zwróci wartość pustą, będą przeglądane węzły-dzieci danego węzła, w przeciwnym razie węzły-dzieci nie będą przeglądane i wartość zwrócona przez surface zostanie potraktowana jako postać powierzchniowa odpowiadająca węzłowi.

# Domyślnie surface działa w następujący sposób:
#    * jeśli węzeł ma ustawiony atrybut Equiv, zwracana jest wartość tego atrybutu
#    * jeśli węzeł nie ma dzieci (jest liściem) zwracana jest kategoria węzła z usuniętymi ewentualnymi apostrofami
#    * w przeciwnym razie zwracana jest wartość pusta



sub ::build()
{
    $childA := insert_left("A", aa);
    $childB := insert_right("B", bb);
    $childA->insert_left("D", dd);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

D:dd => S:ss
{
}

X => Y
{
}

sub Y::default()
{
    is(this(), do_surface());
}

sub P::default()
{
    is(this(), do_surface());
    # is("P:wartEQUIV", Equiv); # rzeczywiście surface() jest wywoływana
}

sub R::default()
{
    is(this(), do_surface());
}

sub P::surface()
{
    set("Equiv", "P:wartEQUIV");
}
