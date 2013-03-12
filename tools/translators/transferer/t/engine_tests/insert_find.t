# Test funkcji: 
# insert_left(cat,label)
	# na początku wstawia podwęzeł o danej kategorii i etykiecie
# insert_right(cat,label)
	# na końcu wstawia podwęzeł o danej kategorii i etykiecie
# find_right(cat,label), find_left(cat,label), find(cat,label)
	# daje podwezęł o danej kategorii i etykiecie lub wartość pustą, jeśli nie znaleziono takiego podwęzła


sub ::build()
{
	insert_left("F", ff);
	$childA := insert_left("A",aa);
	
	$childB := $childA->insert_left("B", bb);
	$childD := $childA->insert_left("D", dd);
	$childA->insert_right("E", ee);

	$childB->insert_left("C", cc);
	$childD->insert_left("A", aa);
}

A:aa => P:pp
{
}

B:bb => R:rr
{
}

C:cc => S:ss
{
	ok(!find("P", pp));			# 2. 	--istnieje w drzewie, nie jest potomkiem wezla
}

D:dd => T:tt
{
	ok(find("P", pp));			# 1.	--jest bezposrednim potomkiem (drugi wezel o tej samej nazwie)
}

E:ee => U:uu
{
}

F:ff => W:ww
{
}

X => Y
{
	# sprawdzenie dla wezla X 
	  ok(find_left("P", pp));		# 2. P	--jest bezposrednim potomkiem
	  ok(!find_left("T", tt));		# 3. T	--nie jest bezposrednim potomkiem
	  

	# sprawdzenie dla wezla P
	  $child1 := find_left("P", pp);
	  
	  ok($child1->find_left("R", rr));	# 4. R	--jest bezposrednim potomkiem
	  ok($child1->find_left("U", uu));	# 5. U 	--jest bezposrednim potomkiem
	  ok(!$child1->find_left("S", ss));	# 6. S 	--nie jest bezposrednim potomkiem
	  ok(!$child1->find("W", ww));	  	# 7. W	--nie jest bezposrednim potomkiem
	  ok(!find_left("S", plum));		# 8. S z inna etykieta	--nie istnieje w drzewie, ma inna etykiete
}

