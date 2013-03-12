# Test funkcji: 
# substring(s,beg,len)
	# napis podnapis napisu s począwszy od beg o długości len;
	# beg może być ujemne - wówczas początek podnapisu jest liczony od końca (np. substring("warszawa",-3,2) = "aw"),
	# przy braku trzeciego argumentu podnapis jest brany do końca (np. substring("warszawa", 3, ) = "szawa").

sub ::build()
{
}

X => Y
{
	
}

sub Y::default()
{
	# prosty przyklad
	is("napis", substring("napis 1", 0, 5));
	is("napis", substring("nowy napis", 5, ));
	is("napis", substring("nowy napis", -5, 5));
	is("napis", substring("nowy napis", -5, ));

	# sprawdzenie dla polskich znakow
	is(" węzeł jest usuwany", substring("usuwa pierwszy podwęzeł o kategorii cat_or_node i etykiecie label, argument cat_or_node może być po prostu węzłem (a nie kategorią) - wówczas ten węzeł jest usuwany", -19, 19));
	
	# sprawdzenie dla pustego ciagu znakow
	is("", substring("nowy napis!@#$%^&*()...", 23, ));
	is("", substring("nowy napis", 10, 10));

	# sprawdzenie dla dlugiego ciagu znakow
	is("through the woods, and two of them fell into a deep pit. When the other frogs saw how deep the pit was, they told the two frogs that they were as good as", substring("A group of frogs were traveling through the woods, and two of them fell into a deep pit. When the other frogs saw how deep the pit was, they told the two frogs that they were as good as dead. The two frogs ignored the comments and tried to jump up out of the pit with all their might. The other frogs kept telling them to stop, that they were as good as dead. Finally, one of the frogs took heed to what the other frogs were saying and gave up. He fell down and died. The other frog continued to jump as hard as he could. Once again, the crowd of frogs yelled at him to stop the pain and just die. He jumped even harder and finally made it out. When he got out, the other frogs said, Did you not hear us? The frog explained to them that he was deaf. He thought they were encouraging him the entire time. This story teaches two lessons: 1. There is power of life and death in the tongue. An encouraging word to someone who is down can lift them up and help them make it through the day. 2. A destructive word to someone who is down can be what it takes to kill them. Be careful of what you say. Speak life to those who cross your path. The power of words... it is sometimes hard to understand that an encouraging word can go such a long way. Anyone can speak words that tend to rob another of the spirit to continue in difficult times. Special is the individual who will take the time to encourage another.", 32, 153));
}



