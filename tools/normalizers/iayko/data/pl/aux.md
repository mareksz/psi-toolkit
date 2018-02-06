# Auxilliary grammar file
## All bytes

    export Any = Optimize[
        "[1]" |   "[2]" |   "[3]" |   "[4]" |   "[5]" |   "[6]" |   "[7]" |   "[8]" |   "[9]" |  "[10]" |
        "[11]" |  "[12]" |  "[13]" |  "[14]" |  "[15]" |  "[16]" |  "[17]" |  "[18]" |  "[19]" |  "[20]" |
        "[21]" |  "[22]" |  "[23]" |  "[24]" |  "[25]" |  "[26]" |  "[27]" |  "[28]" |  "[29]" |  "[30]" |
        "[31]" |  "[32]" |  "[33]" |  "[34]" |  "[35]" |  "[36]" |  "[37]" |  "[38]" |  "[39]" |  "[40]" |
        "[41]" |  "[42]" |  "[43]" |  "[44]" |  "[45]" |  "[46]" |  "[47]" |  "[48]" |  "[49]" |  "[50]" |
        "[51]" |  "[52]" |  "[53]" |  "[54]" |  "[55]" |  "[56]" |  "[57]" |  "[58]" |  "[59]" |  "[60]" |
        "[61]" |  "[62]" |  "[63]" |  "[64]" |  "[65]" |  "[66]" |  "[67]" |  "[68]" |  "[69]" |  "[70]" |
        "[71]" |  "[72]" |  "[73]" |  "[74]" |  "[75]" |  "[76]" |  "[77]" |  "[78]" |  "[79]" |  "[80]" |
        "[81]" |  "[82]" |  "[83]" |  "[84]" |  "[85]" |  "[86]" |  "[87]" |  "[88]" |  "[89]" |  "[90]" |
        "[91]" |  "[92]" |  "[93]" |  "[94]" |  "[95]" |  "[96]" |  "[97]" |  "[98]" |  "[99]" | "[100]" |
        "[101]" | "[102]" | "[103]" | "[104]" | "[105]" | "[106]" | "[107]" | "[108]" | "[109]" | "[110]" |
        "[111]" | "[112]" | "[113]" | "[114]" | "[115]" | "[116]" | "[117]" | "[118]" | "[119]" | "[120]" |
        "[121]" | "[122]" | "[123]" | "[124]" | "[125]" | "[126]" | "[127]" | "[128]" | "[129]" | "[130]" |
        "[131]" | "[132]" | "[133]" | "[134]" | "[135]" | "[136]" | "[137]" | "[138]" | "[139]" | "[140]" |
        "[141]" | "[142]" | "[143]" | "[144]" | "[145]" | "[146]" | "[147]" | "[148]" | "[149]" | "[150]" |
        "[151]" | "[152]" | "[153]" | "[154]" | "[155]" | "[156]" | "[157]" | "[158]" | "[159]" | "[160]" |
        "[161]" | "[162]" | "[163]" | "[164]" | "[165]" | "[166]" | "[167]" | "[168]" | "[169]" | "[170]" |
        "[171]" | "[172]" | "[173]" | "[174]" | "[175]" | "[176]" | "[177]" | "[178]" | "[179]" | "[180]" |
        "[181]" | "[182]" | "[183]" | "[184]" | "[185]" | "[186]" | "[187]" | "[188]" | "[189]" | "[190]" |
        "[191]" | "[192]" | "[193]" | "[194]" | "[195]" | "[196]" | "[197]" | "[198]" | "[199]" | "[200]" |
        "[201]" | "[202]" | "[203]" | "[204]" | "[205]" | "[206]" | "[207]" | "[208]" | "[209]" | "[210]" |
        "[211]" | "[212]" | "[213]" | "[214]" | "[215]" | "[216]" | "[217]" | "[218]" | "[219]" | "[220]" |
        "[221]" | "[222]" | "[223]" | "[224]" | "[225]" | "[226]" | "[227]" | "[228]" | "[229]" | "[230]" |
        "[231]" | "[232]" | "[233]" | "[234]" | "[235]" | "[236]" | "[237]" | "[238]" | "[239]" | "[240]" |
        "[241]" | "[242]" | "[243]" | "[244]" | "[245]" | "[246]" | "[247]" | "[248]" | "[249]" | "[250]" |
        "[251]" | "[252]" | "[253]" | "[254]" | "[255]"  
    ];

## White spaces
    Space = Optimize[
    " " | "\t" | "\n" | "\r" 
    ];

    NotSpace = Optimize[Any - Space];

## Punctuation marks
    Punct = Optimize[
    "!" | "\"" | "#" | "$" | "%" | "&" | "'" | "(" | ")" | "*" | "+" | "," |
    "-" | "." | "/" | ":" | ";" | "<" | "=" | ">" | "?" | "@" | "\[" | "\\" |
    "\]" | "^" | "_" | "`" | "{" | "|" | "}" | "~"
    ];

## Character Rules
_Character Rules_ are responsible for listing all characters that may appear in Polish texts:
historical and temporary

    Lower = "a" | "ą".utf8 | "á" | "b" | "c" | "ć" | "d" | "e" | "é" | "ę" | "f" | "g" | "h" | "i" | "j" | "k" |
            "l"    |    "ł" | "m" | "n" | "ń" | "o" | "ó" | "p" | "q" | "r" | "s" | "ś" | "t" | "u" |
            "v" | "w" | "x" | "y" | "z" | "ź" | "ż" ;

    Upper =  "A" | "Ą" | "Á" | "B" | "C" | "Ć" |"D" | "E" | "É" |"Ę" |"F" | "G" | "H" | "I" | "J" | "K" |
               "L" | "Ł" |" M" | "N" | "Ń" | "O" |"Ó" | "P" | "Q" | "R" | "S"| "Ś" | "T" | "U" | "V" |
               "W" | "X" | "Y" | "Z"| "Ź" | "Ż" ;

    Letter = Lower | Upper ;

    LowerVowel = "a" | "ą" | "á" | "e" | "é" | "ę" | "i" | "o" | "ó" | "u" | "y" ;

    UpperVowel = "A" | "Ą" | "Á" | "E" | "É" | "Ę" | "I" | "O" | "Ó" | "U" | "Y" ;

    Vowel = LowerVowel | UpperVowel ;

    LowerConsonant = Lower - LowerVowel ;

    UpperConsonant = Upper - UpperVowel ;

    Consonant = LowerConsonant | UpperConsonant ;

    AnyString = Letter* ;

    NonEmptyString = Letter+ ;

    NonLetter = Any - Letter;

    Beginning = "[BOS]" | Space | Punct ;

    End = "[EOS]" | Space | Punct ;

    AnyPrefix = Beginning AnyString ;

## Palatalization issues
The below automaton list consonants that are followed by the "j" character, if palatelized.

    Followed_by_j =   "c" | "s" | Vowel "z" |
                      "C" | "S" | Vowel "Z" ;

The below automaton list characters that are followed by the "i" character, if palatelized.

    Followed_by_i =  "d" | "f" | "g" | "h" | "k" | "l" | "m" | "n" | "p" | "r" | "t" | "w" |
                     "D" | "F" | "G" | "H" | "K" | "L" | "M" | "N" | "P" | "R" | "T" | "W" ;

## Capitalization issue
The rules of the type "Ll" deal with capitalization issue (lower, upper character) for standard letters.
The rules of the type "Caca" deal with capitalization issue for acute letters (e. g. "ć").
The rules of the type "Anan" deal with capitalization issue for nasal vowels (e. g. "ą").
The rules of the type "Lclc" deal with capitalization issue for other Polish diacritic letters (e. g. "ż").

    Aa = ( "A" | "a" ) ;
    Anan = ( "Ą" | "ą" ) ;
    Aaaa = ( "Á" | "á" ) ;
    Bb = ( "B" | "b" ) ;
    Cc = ( "C" | "c" ) ;
    Caca = ( "Ć" | "ć" ) ;
    Dd = ( "D" | "d" ) ;
    Ee = ( "E" | "e" ) ;
    Enen = ( "Ę" | "ę" ) ;
    Ff = ( "F" | "f" ) ;
    Gg = ( "G" | "g" ) ;
    Hh = ( "H" | "h" ) ;
    Ii = ( "I" | "i" ) ;
    Jj = ( "J" | "j" ) ;
    Kk = ( "K" | "k" ) ;
    Ll = ( "L" | "l" ) ;
    Lclc = ( "Ł" | "ł" ) ;
    Mm = ( "M" | "m" ) ;
    Nn = ( "N" | "n" ) ;
    Nana = ( "Ń" | "ń" ) ;
    Oo = ( "O" | "o" ) ;
    Oaoa = ( "Ó" | "ó" ) ;
    Pp = ( "P" | "p" ) ;
    Qq = ( "Q" | "q" ) ;
    Rr = ( "R" | "r" ) ;
    Ss = ( "S" | "s" ) ;
    Sasa = ( "Ś" | "ś" ) ;
    Tt = ( "T" | "t" ) ;
    Uu = ( "U" | "u" ) ;
    Vv = ( "V" | "v" ) ;
    Ww = ( "W" | "w" ) ;
    Xx = ( "X" | "x" ) ;
    Zz = ( "Z" | "z" ) ;
    Zaza = ( "Ź" | "ź" ) ;
    Zczc = ( "Ż" | "ż" ) ;
