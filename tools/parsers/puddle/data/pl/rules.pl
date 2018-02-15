# IT : Scentralizowane usługi w zakresie depozytów , bezpiecznego przechowywania i administracji mogą być świadczone jedynie przez Bank Włoch w odniesieniu do rządowych papierów wartościowych lub przez Monte Titoli SpA w odniesieniu do akcji , papierów wartościowych zaświadczających uczestnictwo kapitałowe i innych obligacji będących przedmiotem obrotu na rynku regulowanym .
# 36. RO : Zakładanie i działalność towarzystw ubezpieczeniowych i reasekuracyjnych są uzależnione od pozwolenia wydawanego przez organ nadzoru działalności ubezpieczeniowej i reasekuracyjnej .
# Pośrednictwo w zakresie zawierania umów ubezpieczeniowych pomiędzy stroną trzecią a towarzystwem ubezpieczeniowym może być świadczone przez osoby fizyczne lub prawne zamieszkałe w Republice Słowackiej na rzecz towarzystwa ubezpieczeniowego posiadającego licencję Organu Nadzoru Ubezpieczeń. Umowa pośrednictwa mająca na celu zawarcie umowy ubezpieczeniowej przez stronę trzecią z towarzystwem ubezpieczeniowym może być zawarta przez krajowe lub zagraniczne towarzystwo ubezpieczeniowe , jedynie w przypadku przyznania licencji przez Organ Nadzoru Ubezpieczeń. Środki finansowe specjalnych funduszy ubezpieczeniowych licencjonowanych podmiotów ubezpieczeniowych pochodzące z ubezpieczenia lub reasekuracji posiadaczy polisy mających status rezydenta lub zarejestrowaną siedzibę w Republice Słowackiej muszą zostać złożone w banku rezydującym w Republice Słowackiej i nie mogą zostać przekazane za granicę.
# 1. W drodze odstępstwa od wymagań kwalifikacji przewidzianych w art. 3 ust. 1 dyrektywy 66/401/EWG , art. 3 ust. 1 dyrektywy 66/402/EWG , art. 3 ust. 1 dyrektywy 2002/54/WE , art. 3 ust. 1 dyrektywy 2002/56/WE i art. 3 ust. 1 dyrektywy 2002/57/WE państwa członkowskie mogą postanowić , że materiał siewny odmiany chronionej może być wprowadzony do obrotu , jeśli spełnia on wymogi ust. 2 , 3 i 4 tych artykułów .
# [ 4 ] Oprócz kwoty kapitału , Bank Słowenii , rozważając wydanie nieograniczonej lub ograniczonej licencji bankowej , uwzględnia również następujące wytyczne ( zarówno w odniesieniu do krajowych jak i zagranicznych wnioskodawców ) : - preferencje krajowo-ekonomiczne dla niektórych działań bankowych ; - istniejącą gęstość sieci bankowej w Republice Słowenii ; - rzeczywistą realizację działań banku w porównaniu z działaniami określonymi w istniejącej licencji. ( Uwaga : przepis ten zostanie zniesiony z chwilą przyjęcia nowej ustawy o bankowości ) .
# [ 5 ] W przypadku tradycyjnych zasobów własnych ( opłaty rolne , opłaty wyrównawcze od cukru , cła ) należy wskazać kwoty netto , tzn. kwoty brutto po odliczeniu 25% na poczet kosztów poboru .
# Państwa członkowskie mogą stosować ograniczenia wskazane na tej liście jedynie w odniesieniu do bezpośredniego prowadzenia działalności przez meksykańską jednostkę handlową lub do świadczenia usług transgranicznych z Meksyku ; wobec powyższego , państwo członkowskie nie może stosować tych ograniczeń , w tym ograniczeń dotyczących prowadzenia działalności , do meksykańskich filii założonych w innym państwie członkowskim Wspólnoty , chyba że te ograniczenia mogą również być stosowane do przedsiębiorstw lub obywateli innych państw członkowskich zgodnie z prawem wspólnotowym .



#> któremu NP
Rule "który"
Match: [pos~"apron" && base~"któr.*"];
#Right: [pos!~"subst"];
Eval:  group(NP, 1);

#> jakich NP
Rule "jaki"
Match: [pos~"apron" && base~"jaki.*"];
#Right: [pos!~"subst"];
Eval:  group(NP, 1);

##KJ: "brutto" is treated as an adjective of arbitrary grammatical features in order to be unified with a preceding noun in arbitrary case, gender and number
#> brutto adj
Rule "brutto"
Match: [base~"brutto"];
Eval:  add("adj:*:*:*:pos", "brutto",1);

#> netto adj
Rule "netto"
Match: [base~"netto"];
Eval:  add("adj:*:*:*:pos", "netto",1);

##KJ: "tzn." should be parsed as conjunction (same as "i"); So, "tzn. kot" should be parsed as CNP but it is NOT!
#> tzn. conj
Rule "tzn."
Match: [base~"tzn\."];
Eval:  add("conj", "tzn\.",1);

Rule "lit."
Match: [base~"lit\."];
Eval:  delete(pos!~"subst", 1);
       group(NP, 1);

#Rule "dłużej niż"
#Match: [pos~"adv" && degree~"comp"] [base~"niż"];
#Eval:  word(1, PS:comp, base);
#
#Rule "dłuższy niż"
#Match: ([pos~"adj" && degree~"comp"]|[base~"inny"]) [base~"niż"];
#Eval:  word(1.2, P);

Rule "poza"
Match: [orth~"poza"];
Eval:  delete(pos!~"prep",1);

###### Non word items #####

#> Jassem NE
#> Kowal subst
Rule "NE1.1: Naive named entity - starts with a capital letter and consists of several unknown items"
#Match: [pos~"ign" && orth~"\p{Lu}{1}[\p{L}{1}\'\-\/\.\d]*"] [pos~"ign"]*;
#Match: [pos~"ign" && orth~"[[:Lu:]][[[:L*:]]\'\-\/\.\d]*"] [pos~"ign"]*;
Match: [pos~"ign" && orth~"\p{Lu}[\p{L}\.\d\'\-\/]*"] [pos~"ign"]*;
Eval:  group(NE, 1);

#> "Jassem" NE
#> "Kowal" subst
Rule "NE1.2: Naive named entity - surrounded by quotes and consists of unknown items"
Left:  [base~"\""];
Match: [pos~"ign"]+ ;
Right: [base~"\""];
Eval:  group(NE, 2);

##KJ: There must be something wrong with regexps. The rule used to work but no longer does.
#> 2. stycznia 1965 r.
#> 2. stycznia 1965 r
Rule "DATE1: Date with month names and optional 'r.' (eg. 13 listopada 2008 r.)"
Match: [orth~"\d?\d"] [orth~"\."]? [base~"styczeń|luty|marzec|kwieceń|maj|czerwiec|lipiec|sierpień|wrzesień|październik|listopad|grudzień"] ([base~"\d\d\d\d"] ([orth~"r\.?"] | [orth~"roku"])?)?;
Eval:  group(DATE, 3);

##KJ: Tokenizer should be corrected. It returns type X instead of DATE
Rule "DATE2: Simple numerical date (eg. 13.11.2008)"
Match: [base~"\d?\d\.\d?\d\.(\d\d)?\d\d"];
Eval:  group(DATE, 1);

##KJ: Tokenizer should be corrected. It returns type X instead of DATE
Rule "TIME1: Simple time (e.g. 12:01)"
Match: [pos~"ign" && base~"\d?\d[\.\:]\d\d"];
Eval:  group(TIME, 1);

##KJ: There must be something wrong with regexps. The rule works for numbers that have 2 or more digits. Does not work for 1-digit numbers.
#>34 %
Rule "NUM1: All tokens that begin with a numeral, followed by space and %"
Match: [orth~"\d?\d"] [orth~"%"]?;
Eval:  group(NUM, 1);

##KJ: The rule does not work because group must consist of at least two tokens. "add" should be used instead of "group" but this is not compiled by the parser
#>XIX NUM
Rule "NUM2: Roman numbers from I to X with captial letters"
Match: [orth~"(I|II|III|IV|V|VI|VII|VIII|IX|X)"];
Eval:  group(NUM, 1);

##KJ: Works for numerals longer than 2 digits
#>( 54 ) NUM
Rule "NUM4.1: Numerical tokens surrounded by braces"
Match: [base~"\("] [type=NUM] [base~"\)"]; 
Eval:  group(NUM, 2);

##KJ: Works for numerals longer than 2 digits
#>[ 54 ]
#>[5]
Rule "NUM4.2: Numerical tokens surrounded by squared parentheses"
Match: [base~"\["] [type=NUM] [base~"\]"];
Eval:  group(NUM, 2);

##KJ: 1) I suggest to remove this rule
#>( a )
Rule "NUM3.1: Single letters surrounded by ')'"
#Match: [base~"\("] [base~"\p{L*}"] [base~"\)"];
Match: [base~"\("] [base~"\p{L}"] [base~"\)"];
Eval:  group(NUM, 2);

##KJ: 1) I suggest to remove this rule
Rule "NUM3.2: Single letters followed by ')'"
#Match: [base~"\p{L*}"] ns [base~"\)"];
Match: [base~"\p{L}"] [base~"\)"];
Eval:  group(NUM, 1);

##KJ: The rule does not work
#> 5 albo 12
Rule "NUM5: Conjunction of numerical phrases"
Match: [type=NUM] (([pos~"conj"]|[base~","]) [type=NUM])+ ;
Eval:  group(NUM, 1);

#> "Jassem"
Rule "NE2: Named entity surrounded by quotes"
Match: [base~"\""] [type=NE] [base~"\""];
Eval:  group(NE, 2);

##KJ: The rule does not work
#> ( Jassem )
Rule "NE3: Named entity surrounded by parenthesis"
#Match: [base~"\("] ([type=NE]|[orth~"\p{Lu}+"]) [base~"\)"];
Match: [base~"\("] ([type=NE]|[orth~"p\{Lu}+"]) [base~"\)"];
Eval:  group(NE, 2);

##KJ: The rule does not work
#> Jassem albo Jaworski
Rule "NE4: Conjunction of named entities"
Match: [type=NE] ([pos~"conj"]? [type=NE])+ ;
Eval:  group(NE, 1);

###### adjective rules #####

#> oczekiwany
#> wyglądający
#> piękny 
Rule "AP1: At least 1 adjective"
Match: ([pos~"adj" | pos~"ppas" | pos~"pact"])+;
Eval:  unify(case gender number, 1, 2);
       group(AP, 1);

#> długo oczekiwany
#> pięknie wyglądający
#> zawsze piękny 
Rule "AP2: Adverb + at least 1 adjective"
Match: [pos~"adv"]* [type=AP];
Eval:  delete(pos!~"adv", 1);
       group(AP, 2);

Rule "AP3.1: Conjuction + adjective phrase"
Match: [pos~"conj"] [type=AP];
Eval:  delete(pos!~"conj", 1);
       group(CAP, 2);

#Rule "AP3.2: Comma + adjective phrase"
#Match: [base~","] [type=AP];
#Eval:  group(CAP, 2);

Rule "AP4: Adjective phrase + conjunctional adjective phrase"
Match: [type=AP] [type=CAP]+;
Eval:  unify(case gender number, 1, 2);
       group(AP, 1);

####### nominal rules #####

#>młot
Rule "NP1.1: Single noun"
Match: [pos~"subst"] ;
Eval:  delete(pos!~"subst", 1);
       group(NP, 1);

#>czekanie
#>bawienie się
Rule "NP1.2: Single gerund"
Match: [pos~"ger"] [pos~"pron" && base~"się"]?;
Eval:  delete(pos!~"ger", 1);
       delete(pos!~"pron", 2);
       group(NP, 1);

#>żeliwny młot kowalski	   
Rule "NP2.1: Adjective phrase + noun + adjective phrase"
Match: [type=AP] [pos~"subst"] [type=AP];
Eval:  unify(case gender number, 1, 2, 3);
       delete(pos!~"subst", 2);
       group(NP, 2);

#>młot kowalski
Rule "NP2.2: Noun + adjective phrase"
Match: [pos~"subst"] [type=AP];
Eval:  unify(case gender number, 1, 2);
       delete(pos!~"subst", 1);
       group(NP, 1);

#> żeliwny młot
Rule "NP2.3: Adjective phrase + noun"
Match: [type=AP] [pos~"subst"];
Eval:  unify(case gender number, 1, 2);
       delete(pos!~"subst", 2);
       group(NP, 2);

#>Naiwne łudzenie się spokojne
Rule "NP2.4: Adjective phrase + gerund + adjective phrase"
Match: [type=AP] [pos~"ger"] [pos~"pron" && base~"się"]? [type=AP];
Eval:  unify(case gender number, 1, 2, 4);
       delete(pos!~"ger", 2);
       group(NP, 2);

#>łudzenie się spokojne
Rule "NP2.5: Gerund + adjective phrase"
Match: [pos~"ger"] [pos~"pron" && base~"się"]? [type=AP];
Eval:  unify(case gender number, 1, 3);
       delete(pos!~"ger", 1);
       group(NP, 1);

#>naiwne łudzenie się
Rule "NP2.6: Adjective phrase + gerund"
Match: [type=AP] [pos~"ger"] [pos~"pron" && base~"się"]?;
Eval:  unify(case gender number, 1, 2);
       delete(pos!~"ger", 2);
       group(NP, 2);

#>mój żeliwny młot kowalski	   
Rule "NP3: Possessive pronoun + noun phrase"
Match: [pos~"pron"] [type=NP && head=[pos!~"pron"]];
Eval:  unify(case gender number, 1, 2);
       delete(pos!~"pron",1);
       group(NP, 2);

##KJ: The rule does not work
#>cztery pory roku	   
Rule "NP4.1: Cardinal number + noun phrase"
Match: [pos~"num"] [type=NP && head=[pos!~"pron"]];
Eval:  unify(case gender, 1, 2);
       delete(pos!~"num",1);
       group(NP, 2);

##KJ: The rule does not work
#>przybysze dwaj
Rule "NP4.2: Noun phrase + cardinal number"
Match: [type=NP && head=[pos!~"pron"]] [pos~"num"];
Eval:  unify(case gender, 1, 2);
       delete(pos!~"num",2);
       group(NP, 1);

#>one
#>im   
Rule "NP6: Pronoun as noun phrase"
Match: [pos~"ppron3"];
Eval:  delete(pos!~"pron", 1);
      group(NP, 1);

#Rule "NP7: Single adjective as noun phrase"
#Match: [pos~~"adj"];
#Eval:  group(NP, 1);

###### Combining tokens and phrases #####

Rule "NP+NUM1.1: Numerical phrase as apposition (after noun phrase)"
Match: [type=NP] [type=NUM]+ [type=AP];
Eval:   unify(case gender number, 1, 3);
        group(NP, 1);

Rule "NP+NUM1.2: Numerical phrase as apposition (after noun phrase)"
Match: [type=NP] [type=NUM]+;
Eval:  group(NP, 1);

Rule "NP+NUM1.3: Numerical phrase as apposition (before noun phrase)"
Left:  [pos~".*"];
Match: [type=NUM] [type=NP];
Eval:  group(NP, 3);

Rule "NP+NE1: Named entity as apposition (after noun phrase)"
Match: [type=NP] [type=NE]+;
Eval:  group(NP, 1);

Rule "PR+DATE1: Date as prepositional phrase (eg. do 3 marca 2004)"
Match: [pos~"adv"]? [pos~"prep"] [type=DATE];
Eval:  group(PP, 3);

Rule "PR+NUM1: Numerical phrase as prepositional phrase (eg. około 30 %)"
Match: [pos~"adv"]? [pos~"prep"] [type=NUM];
Eval:  group(PP, 3);

Rule "NP+DATE1: Date as apposition (after noun phrase)"
Match: [type=NP] [type=DATE];
Eval:  group(NP, 1);

###### Higher order noun phrases #####

#KJ: Rule NP8 does not work (according to something wrong in puddle)
#> Pory roku
Rule "NP8: Noun phrase + genitive attribute"
Match: [type=NP && head=[pos!~"pron"]] [type=NP && case~"gen"]+;
Eval:  delete(case!~"gen", 2);
       group(NP, 1);

#>i kot
Rule "NP9.1: Conjunction + noun phrase"
Match: [pos~"conj"] [type=NP];
Eval:  delete(conj-type!~"phrase", 1);
       group(CNP, 2);

#>,kot
Rule "NP9.2: Comma + noun phrase"
Match: [base~","] [type=NP];
Eval:  group(CNP, 2);

#> pies i kot
Rule "NP10: Noun phrase + conjunctional noun phrase"
Match: [type=NP] [type=CNP]+;
Eval:  unify(case, 1, 2);
       group(NP, 1);

####### Higher order noun phrases 2 #####

#KJ: Temporarily, preposition 'do' does not work (discarded by lemmatization)
#> Kosz na śmieci
#> Miska na jedzenie dla psa
Rule "NP11: Noun phrase (no pronoun) + prepositional attribute"
Match: [type=NP && head=[pos!~"pron"]] [type=PP]+;
Eval:  group(NP, 1); # lemmatize(1)

#> " Miska dla psa"
Rule "NP12.1: Noun phrase surrounded by quotes"
Match: [base~"\""] [type=NP] [base~"\""];
Eval:  group(NP, 2); # lemmatize(2)

#> (Pies i kot)
Rule "NP12.2: Noun phrase surrounded by parenthesis"
Match: [base~"\("] [type=NP] [base~"\)"];
Eval:  group(NP, 2);
	   
###### Prepositional phrases #####
##KJ: Removed optional adverbs
##KJ: Changed the head of the group
#> na śmieci
Rule "PP1.1: Prepositions + noun phrase"
Match: [pos~"prep"] [type=NP];
Eval:  unify(case, 1, 2);
       delete(pos!~"prep", 1);
       group(PP, 1);

##KJ: Removed optional adverbs
##KJ: Changed the head of the group
#> Dla Jassema
Rule "PP1.2: Named entity as prepositional phrase (eg. dla McCaina)"
Match: [pos~"prep"] [type=NE];
Eval:  delete(pos!~"prep", 2);
       group(PP, 1);

#> i dla kota
Rule "PP2.1: Conjunction + prepositional phrase"
Match: [pos~"conj"] [type=PP];
Eval:  delete(pos!~"conj", 1);
       group(CPP, 2);

#> , dla kota
Rule "PP2.2: Comma + prepositional phrase"
Match: [base~","] [type=PP];
Eval:  group(CPP, 2);

#> dla psa, dla kota i dla wnuczki
Rule "PP3: Prepositional phrase + conjunctional prepositional phrase"
Match: [type=PP] [type=CPP]+;
Eval:  group(PP, 1);

##KJ: The rule is not clear
##KJ: It is worth to check the word "trochę" (lamerlemma creates lots of discarded alternatives)
#> trochę zbyt długi
Rule "PP5: Preposition + left-over adjective phrase"
Match: [pos~"adv"]? [pos~"prep"] [type=AP];
Eval:  unify(case, 2, 3);
       group(PP, 3);

##KJ: This rule is not clear to me	   
Rule "PP6: Preposition + left-over numeral"
Match: [pos~"adv"]? [pos~"prep"] [pos~"num"];
Eval:  unify(case, 2, 3);
       group(PP, 3);

###### verbal rules 1 ######

Rule "VP1.1: Verb phrase with predicative"
Match: [pos~"adv"]* [base~"być|zostać" && pos~"verb"] ([pos~"adv"]*|[type=PP]?) ([pos~"adj"]|[pos~"ppas|pact"]|[type=AP]|[type=IP]);
Eval:         delete(pos!~"adv", 1);
       group(VP, 2);

Rule "VP1.2: Verb phrase with predicative"
Match: ([pos~"adj"]|[pos~"ppas|pact"]|[type=AP]|[type=IP]) [pos~"adv"]* [base~"być|zostać"];
Right: ([type=NP]|[type=SNP]);
Eval:  unify(case gender number, 1, 4);
       delete(case!~"nom", 1);
       delete(pos!~"adv", 2);
       group(VP, 3);

Rule "VP1.3: Verb phrase with predicative"
Match: ([pos~"adj"]|[pos~"ppas|pact"]|[type=AP]|[type=IP]) [pos~"adv"]* [base~"być|zostać"]; # TODO: lapie bądź jako przysłówek i sie sypie. to zachowanie parsera do poprawy!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Eval:         delete(case!~"nom", 1);
       delete(pos!~"adv", 2);
       group(VP, 3);

###### glue rules #####

Rule "IP1: Participle with nominal phrases"
Match: [pos~"adv"]? [pos~"ppas|pact|pcon|pant"] [pos~"pron" && base~"się"]? (([type=NP]|[type=PP]|[pos~"adv" && base!~"nie"])+)?;
Eval:         delete(pos!~"ppas|pact|pcon|pant", 2);
      delete(pos!~"adv", 1);
       delete(case~"nom", 4);
      group(IP, 2);

#Rule "IP1: imiesłow czynny lub bierny + fraza rzeczownikowy / fraza przyimkowa"
#Left:  [pos!~"BZK"];
#Match: [pos~"adv"]? [pos~"ppas|pact"] [pos~"ZRs"]? ([type=NP]|[type=PP]|[pos~"adv" && base!~"nie"])+;
#Eval:  group(IP, 3);
#       delete(pos!~"ICZ|IBR",3);
#       delete(pos!~"adv", 2);
#       delete(case~"nom", 5);

Rule "IP2.1: Conjunction + participle phrase"
Match: [pos~"conj"] [type=IP];
Eval:  group(CIP, 2);

#Rule "IP2.2: Comma + participle phrase"
#Match: [base~","] [type=IP];
# Eval:  group(CIP, 2);

Rule "NP10: Noun phrase + partciple phrase"
Match: [type=NP && head=[pos!~"pron"]] ([type=IP]|[type=CIP])+;
Eval:         unify(case gender number, 1, 2);
group(NP, 1);

#Rule "NP11.1: Cast numerical phrases into noun phrases"
#Match: ([type=NUM]);
#Eval:  group(NP, 1);

Rule "NP11.2: Cast named entities into noun phrases"
Match: ([type=NE]);
Eval:  group(NP, 1);

##### verbal rules 2 #####

Rule "VP1.4: Complex verb phrase with predicative"
Match: [pos~"adv"]* ([pos~"verb"]|[pos~"winien"]|[pos~"pred"]) [pos~"adv"]* [base~"być|zostać" && pos~"verb"] [pos~"adv"]* ([pos~"adj"]|[pos~"ppas|pact"]|[type=AP]|[type=IP]);
Eval:  group(VP, 2);
       #delete(case!~"nom", 6);

Rule "VP2: General verbal phrase"
Match: [pos~"adv" && base!~"może"]* ([pos~"verb"]|[pos~"winien"]|[pos~"pred"]) [pos~"adv"]* [base~"się"]? (([pos~"adv"]*|[type=PP]?) ([pos~"verb"]|[pos~"winien"]|[pos~"pred"]) [pos~"adv"]* [base~"się"]?)*;
Eval:  group(VP, 2);
autodelete;

##### Glue rules 2 ######

#KJ: De-activated the rule
#Rule "G1: Series of non-verbal phrases"
#Match: ([type=NP && head=[pos!~"pron"]] | [type=CNP] | [type=NE] | [type=PP] | [type=CPP] | [type=IP]) ([type=NP && head=[pos!~"pron"]] | [type=CNP]| [type=NE]  | [type=PP] | [type=CPP] | [type=IP])+;
#Eval: group(SNP, 1);

##### Sentence rules #####

Rule "S1.1: Normal sentence (obligatory noun group before verb phrase)"
Match: ([pos~"conj"]|[pos~"adv"])* ([type=NP]|[type=SNP]|[type=SPP]|[type=PP]|[type=IP]|[type=CIP])+ [type=VP] ([type=NP]|[type=SNP]|[type=SPP]|[type=PP]|[type=IP]|[type=CIP])*;
Eval:  group(S, 3);

Rule "S1.2: Normal sentence (obligatory noun group after verb phrase)"
Match: ([pos~"conj"]|[pos~"adv"])* ([type=NP]|[type=SNP]|[type=SPP]|[type=PP]|[type=IP]|[type=CIP])* [type=VP] [pos~"adv"]* ([type=NP]|[type=SNP]|[type=SPP]|[type=PP]|[type=IP]|[type=CIP]|[pos~"adv"])+ [pos~"adv"]*;
Eval:  group(S, 3);

Rule "S1.3: Normal sentence (no noun groups)"
Match: ([pos~"conj"]|[pos~"adv"])* [type=VP];
Eval:  group(S, 2);
