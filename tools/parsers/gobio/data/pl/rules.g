
#!!! TRACE special.s


#



#!!! TRACE czasownik.g

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#

#
#
#
#
#
#
#

#
#
#
#
#

#
#
#
#
#
#
#
#
#
#

#
#
#

#
#
#
#
#
#
#
#
#
#
#
#
#
dcz = czasownik*   %FC[czasownik]%

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#

#
#
dcz = kpszdp* %FC[kpszdp]%
    Typ = modalny,
    C := teraźniejszy,
    Podmiot := brak,
    Przyim := brak,
    # ponieważ dla frazy czasownikowej (w przeciwieństwie do przysłówkowej)
    # oczekujemy dopełnienia bezokolicznikowego na pozycji Dop3 musimy
    # poprzestawiać dopełnienia
    (Dop2 != bk;
     Dop3 := Dop2,
     Oblig3 := Oblig2,
     {Dop2 := false},
     {Oblig2 := false},
     {Przyim2 := false}),
    A := any,
    setscore 2

#
dcz = zznie czasownik* %FC[zznie,czasownik]%
    Neg = false,
    Neg := 1,
    setscore 3

#
#
usię = zzsiebie* %FR[zzsiebie]%
    Przyim := brak

#
usię = przyimek zzsiebie*   %FPR[przyimek,zzsiebie]%
    przyimek.P == zzsiebie.P,
    Przyim := przyimek.Typ

#
#
#
#
#
#
#
#

dczsię0 = dcz*

dczsię = dczsię0*

#
#
#

dczsię = dczsię0* usię %dczsię0[+usię]%
    SobieP == usię.P,
    SobiePrzyim = usię.Przyim,
    SobieP := wypełn,
    setscore 3


#
#
#
dczsię = dcz* 'się'   %dcz[+'się':czynne]%
    (Podmiot = 1 ; Podmiot = oblig ;
     (Podmiot = potencjalny, C = bezokolicznik)),
    Zwrotność = false,
    O = 3, L = 1, R == n,
    Podmiot := brak,
    S ^ hum,
    Zwrotność := wypełn

#
#
#
#
dczsię = dcz* 'się' %dcz[+'się':bierne]%
    (Podmiot = 1 ; Podmiot = oblig ;
     (Podmiot = potencjalny, C = bezokolicznik)),
    Zwrotność = false,
    (Wyp1 = false, Przyim1 = brak, (Dop1 = dop ; Dop1 = bier),
     Wyp1 := 1,
     S := Sem1;
     Wyp2 = false, Przyim2 = brak, (Dop2 = dop ; Dop2 = bier),
     Wyp2 := 1,
         S := Sem2;
     Wyp3 = false, Przyim3 = brak, (Dop3 = dop ; Dop3 = bier),
     Wyp3 := 1,
         S := Sem3),
    Zwrotność := wypełn


#

pkcz = dczsię*

#
#
#
#
#
pkcz = zznie pkcz* %pkcz%
    Hk,
    Length = 1,
    Neg = false,
    Neg := 1,
    setscore 4


#
#
#
#
#
#
#
#

#
#
pkcz = pkcz* 'się' %pkcz[+'się']%
    (Length = 1 ; Length = 2),
    Zwrotność = 1,
    Zwrotność := wypełn,
    setscore 2

#
pkcz = pkcz*  czasownik_być  %pkcz[+czasownik_być:posiłk]%
    pkcz.Typ = modalny,
    pkcz.C = teraźniejszy,
    czasownik_być.L == 1,
    czasownik_być.R == n,
    czasownik_być.O == 3,
    C := czasownik_być.C

#


#
#
#
#
#
#
#
#
#
#
#
#
#
#
pkcz = pkcz* czasownik_być
    Neg = false,
    A :== ndk,
    czasownik_być.C = przyszły,
    C = bezokolicznik,
        C := przyszły,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    O :== czasownik_być.O,
    (Podmiot = potencjalny, Podmiot := 1;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)


#
#
pkcz = pkcz* zznie czasownik_być
    (Neg = false ; Neg = oczekiwana),
    A :== ndk,
    czasownik_być.C = przyszły,
    C = bezokolicznik,
    C := przyszły,
    R := czasownik_być.R,
    L := czasownik_być.L,
    O := czasownik_być.O,
    Neg := 1,
    (Podmiot = potencjalny, Podmiot := 1;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    setscore 3

#

#
#

#

#

pkcz = pkcz* '$zostać'
    Neg = false,
    A :== dk,
    (P == mian ; (P == miejsc, '$zostać'.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := '$zostać'.C,
    O := '$zostać'.O,
    R :== '$zostać'.R,
    L :== '$zostać'.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := '$zostać'.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)

#
pkcz = pkcz* zznie '$zostać'
    Neg = false,
    A :== dk,
    (P == mian ; (P == miejsc, '$zostać'.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := '$zostać'.C,
    O := '$zostać'.O,
    R :== '$zostać'.R,
    L :== '$zostać'.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := '$zostać'.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    Neg := 1

#

#
pkcz = pkcz* czasownik_być
    Neg = false,
    A :== any,
    (P == mian ; (P == miejsc, czasownik_być.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := czasownik_być.C,
    O := czasownik_być.O,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := czasownik_być.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)

#
pkcz = pkcz* zznie czasownik_być
    Neg = false,
    A :== any,
    (P == mian ; (P == miejsc, czasownik_być.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := czasownik_być.C,
    O := czasownik_być.O,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := czasownik_być.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    Neg := 1, setscore 3

#
#
kcz0 = czasownik_być kcz0*  %kcz0[czasownik_być:posiłk+]%
    kcz0.Typ = modalny,
    kcz0.C = teraźniejszy,
    (czasownik_być.L == 1,
     czasownik_być.R == n,
     czasownik_być.O == 3 ; R = mż),
    C := czasownik_być.C

#


#
#



#
#



#
#
#


#
pkcz = pkcz* uszfrz
        (pkcz != kcz0; C != odsłownik;
         uszfrz.Core = "on"; uszfrz.Core = "ona"; uszfrz.Core = "ono";
     uszfrz.Core = "oni"; uszfrz.Core = "one"
),  # po lewej stronie odsłownika nie dołączamy
    (( Wyp1 = false,
        uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
        Wyp1 := 1,
        (uszfrz.O != 3; L1 :== uszfrz.L, R1 :== uszfrz.R),
       (Neg = false ; Dop1 != bier ; Przyim1 != brak),
        (pkcz = pkcz;
         (Ps1 != podczasownik1, Ps1 != podczasownik2, Ps1 != podczasownik3,
          Ps1 != podpodczasownik1, Ps1 != podpodczasownik2, Ps1 != podpodczasownik3);
         Przyim1 != brak;
         1
    ),
          %pkcz[+uszfrz:dop1]%);
     ( Wyp2 = false,
        uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
        Wyp2 := 1,
        (uszfrz.O != 3; L2 :== uszfrz.L, R2 :== uszfrz.R),
       (Neg = false ; Dop2 != bier ; Przyim2 != brak),
        (pkcz = pkcz;
         (Ps2 != podczasownik1, Ps2 != podczasownik2, Ps2 != podczasownik3,
          Ps2 != podpodczasownik1, Ps2 != podpodczasownik2, Ps2 != podpodczasownik3);
         Przyim2 != brak;
         1
    ),
          %pkcz[+uszfrz:dop2]%);
     ( Wyp3 = false,
        uszfrz.P == Dop3, uszfrz.Przyim == Przyim3,
        Wyp3 := 1,
        (uszfrz.O != 3; L3 :== uszfrz.L, R3 :== uszfrz.R),
       (Neg = false ; Dop3 != bier ; Przyim3 != brak),
        (pkcz = pkcz;
         (Ps3 != podczasownik1, Ps3 != podczasownik2, Ps3 != podczasownik3,
          Ps3 != podpodczasownik1, Ps3 != podpodczasownik2, Ps3 != podpodczasownik3);
         Przyim3 != brak;
         1
    ),
          %pkcz[+uszfrz:dop3]%)),
    {uszfrz.Neg = 1, Neg = false, Neg := oczekiwana},
    setscore 2,
    {uszfrz.Wyp1 = 1, setscore 1},
    {uszfrz.Wyp2 = 1, setscore 1}

pkcz = pkcz* hak
    Hk := Hk & hak.Hk,
    Nr = hak.Nr,
    setscore 9

#
#
#
#
pkcz = pkcz* krzlicz
    (C = przeszły ; C = teraźniejszy ; C = przyszły ;
     C = bezokolicznik ; C = przypuszczający ; C = bezosobnik),
    (( Wyp1 = false,
      krzlicz.P == Dop1, Przyim1 != brak,
          Sem1 := krzlicz.S ^ pkcz.Sem1,
          Wyp1 := 1,
      L1 :== krzlicz.L, R1 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim1, OczekiwanyP := Dop1,
      %pkcz[+FPR:dop1[krzlicz]]%);
     ( Wyp2 = false,
      krzlicz.P == Dop2, Przyim2 != brak,
          Sem2 := krzlicz.S ^ pkcz.Sem2,
          Wyp2 := 1,
      L2 :== krzlicz.L, R2 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim2, OczekiwanyP := Dop2,
      %pkcz[+FPR:dop2[krzlicz]]%);
     ( Wyp3 = false,
      krzlicz.P == Dop3, Przyim3 != brak,
          Sem3 := krzlicz.S ^ pkcz.Sem3,
          Wyp3 := 1,
      L3 :== krzlicz.L, R3 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim3, OczekiwanyP := Dop3,
      %pkcz[+FPR:dop3[krzlicz]]%))

#
#
pkcz = pkcz* szfrz
        (pkcz != kcz0; C != odsłownik),
    {Neg = false, Neg := oczekiwana},
    (szfrz.P == dop ; szfrz.P == bier),
    (( Wyp1 = false,
      Dop1 == bier, Przyim1 = brak,
      Sem1 := szfrz.S ^ pkcz.Sem1,
      Wyp1 := 1,
      (szfrz.O != 3 ; L1 :== szfrz.L, R1 :== szfrz.R),
      %pkcz[+szfrz:dop1]%);
    ( Wyp2 = false,
      Dop2 == bier, Przyim2 = brak,
      Sem2 := szfrz.S ^ pkcz.Sem2,
      Wyp2 := 1,
      (szfrz.O != 3 ; L2 :== szfrz.L, R2 :== szfrz.R),
      %pkcz[+szfrz:dop2]%);
    ( Wyp3 = false,
      Dop3 == bier, Przyim3 = brak,
      Sem3 := szfrz.S ^ pkcz.Sem3,
      Wyp3 := 1,
      (szfrz.O != 3 ; L3 :== szfrz.L, R3 :== szfrz.R),
      %pkcz[+szfrz:dop3]%)),
    setscore 2,
    {szfrz.Wyp1 = 1, setscore 1},
    {szfrz.Wyp2 = 1, setscore 1}


#
#
#
#
#
pkcz = pkcz* uszfprzym  %pkcz[+uszfprzym:dop1]%
    Wyp1 = false,
    ((uszfprzym.P = mian, Dop1 = prz_mian);
     (uszfprzym.P = dop, Dop1 = prz_dop);
     (uszfprzym.P = cel, Dop1 = prz_cel);
     (uszfprzym.P = bier, Dop1 = prz_bier);
     (uszfprzym.P = narz, Dop1 = prz_narz);
     (uszfprzym.P = miejsc, Dop1 = prz_miejsc)),
        uszfprzym.Przyim == Przyim1,
        ((Dop2 == bier ; Dop2 == dop), Przyim2 = brak,
     L2 :== uszfprzym.L, R2 :== uszfprzym.R ;
     (Dop3 == bier ; Dop3 == dop), Przyim3 = brak,
         L3 :== uszfprzym.L, R3 :== uszfprzym.R ;
     (Dop2 != bier, Dop2 != dop ; Przyim2 != brak),
     (Dop3 != bier, Dop3 != dop ; Przyim3 != brak),
     # @todo odsłownik ma R pusty (w przeciwieństwie do bezokolicznika)
     (C = odsłownik ; R :== uszfprzym.R, L :== uszfprzym.L)
        ),
        Wyp1 := 1,
    {uszfprzym.Wyp1 = 1, setscore 1},
    {uszfprzym.Wyp2 = 1, setscore 1}

#
#
pkcz = pkcz* szfokol
        (pkcz != kcz0; C != odsłownik),
    szfokol.Typ = odprzym,
    (Wyp1 = false, Dop1 == av, Wyp1 := 1, %pkcz[+szfokol:dop1]%;
         Wyp2 = false, Dop2 == av, Wyp2 := 1, %pkcz[+szfokol:dop2]%;
     Wyp3 = false, Dop3 == av, Wyp3 := 1, %pkcz[+szfokol:dop3]%),
     {szfokol.Wyp1 = 1, setscore 1},
     {szfokol.Wyp2 = 1, setscore 1}


#
pkcz = pkcz* szfokol
        (pkcz != kcz0; C != odsłownik),
    szfokol.S = loc,
    ( Wyp1 = false, Dop1 == lc, Wyp1 := 1, %pkcz[+szfokol:dop1]%;
          Wyp2 = false, Dop2 == lc, Wyp2 := 1, %pkcz[+szfokol:dop2]%;
      Wyp3 = false, Dop3 == lc, Wyp3 := 1, %pkcz[+szfokol:dop3]%),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1},
    {szfokol.Neg = 1, Neg = false, Neg := oczekiwana}

#
#
#
#
#
#
#
#
pkcz = pkcz* szfokol  %pkcz[+szfokol:okol]%
    (''  = '+' ;
         Wyp3 = false ;
         (Dop3 != bk, Dop3 != odsł_mian, Dop3 != odsł_dop, Dop3 != odsł_cel,
          Dop3 != odsł_bier, Dop3 != odsł_narz, Dop3 != odsł_miejsc )),
    ('+' = '+' ;
     (C != odsłownik,
      #C != imiesłów_bierny,
      C != imiesłów_czynny ; szfokol.Length = 1,
          C != imiesłów_współczesny,
          C != imiesłów_uprzedni)),
    (szfokol.Typ != odprzym;
         ((Wyp1; Dop1 != av),
      (Wyp2; Dop2 != av),
      (Wyp3; Dop3 != av))),
    (szfokol.S != loc;
         ((Wyp1; Dop1 != lc),
      (Wyp2; Dop2 != lc),
      (Wyp3; Dop3 != lc))),
    (szfokol.Przyim = false ;
     (Wyp1; Przyim1 != szfokol.Przyim; Dop1 != szfokol.P),
     (Wyp2; Przyim2 != szfokol.Przyim; Dop2 != szfokol.P),
     (Wyp3; Przyim3 != szfokol.Przyim; Dop3 != szfokol.P)),
        (szfokol.Lu = false ; pkcz.L == szfokol.Lu),
    (szfokol.Ru = false ; pkcz.R == szfokol.Ru),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1},
    {szfokol.Neg = 1, Neg = false, Neg := oczekiwana}
    # żeby FPR się przyklejała raczej do czasownika
    , setscore 1

#
#
pkcz = pkcz* przecinek zdanie_wtrącone przecinek

#
#
pkcz = pkcz* przecinek szfokol przecinek


#
#
pkcz = pkcz* spójnik  %pkcz[+spójnik:inkorporacyjny]%
       spójnik.Ps1 = 1, spójnik.Ps3 = 3,
#
#
#
#
#
    setscore 1


#
pkcz = pkcz* szfrz %pkcz[+szfrz:podmiot]%
#
#
    ( C = przeszły; C = przyszły; C = teraźniejszy; C = przypuszczający; C = nieustalony),
    ((Podmiot = 1; Podmiot = oblig), Podmiot := wypełn;
         Podmiot = potencjalny, Podmiot := potencjalny_wypełn),
    szfrz.P == mian,
    ( L :== szfrz.L,
          R :== szfrz.R,
          ( O :== szfrz.O; O :== szfrz.O1);
      L :== szfrz.L1,
      R :== szfrz.R1,
      O :== szfrz.O1 ),
    S := szfrz.S ^ pkcz.S,
    {szfrz.Neg = 1, Neg = false, Neg := oczekiwana}

#
#
#
#
pkcz = pkcz* uodsłownik %pkcz[+uodsłownik:dop2]%
    Wyp2 = false,
    ((uodsłownik.P = mian, Dop2 = odsł_mian);
     (uodsłownik.P = dop, Dop2 = odsł_dop);
     (uodsłownik.P = cel, Dop2 = odsł_cel);
     (uodsłownik.P = bier, Dop2 = odsł_bier);
     (uodsłownik.P = narz, Dop2 = odsł_narz);
     (uodsłownik.P = miejsc, Dop2 = odsł_miejsc);
     (uodsłownik.P = dop, Dop2 = odsł_bier, Przyim2 = brak,
          (Neg = false, Neg := oczekiwana ; Neg != false))),
        uodsłownik.Przyim == Przyim2,
    Wyp2 := 1, setscore 4

#

kcz0 = pkcz* przecinek? zkcz %pkcz[+przecinek,+zkcz]%
    (zkcz.C = imiesłów_współczesny ; zkcz.C = imiesłów_uprzedni),
    (C = przeszły ; C = teraźniejszy ; C = przyszły ; C = bezosobnik ;
         C = przypuszczający)

#

korelat = 'to'        %KORELAT['to']%
    Typ := to
    , Block := 1

korelat = 'o' 'to'    %KORELAT['o','to']%
    Typ := o_to
    , Block := 1

korelat = 'na' 'to' %KORELAT['na','to']%
    Typ := na_to
    , Block := 1

korelat = 'za' 'to' %KORELAT['za','to']%
    Typ := za_to
    , Block := 1

korelat = 'na' 'tym' %KORELAT['na','tym']%
    Typ := na_tym
    , Block := 1

korelat = 'nad' 'tym' %KORELAT['nad','tym']%
    Typ := nad_tym
    , Block := 1

korelat = 'o' 'tym' %KORELAT['o','tym']%
    Typ := o_tym
    , Block := 1

korelat = 'w' 'tym' %KORELAT['w','tym']%
    Typ := w_tym
    , Block := 1

korelat = 'tego' %KORELAT['tego']%
    Typ := tego
    , Block := 1

korelat = 'z' 'tego' %KORELAT['z','tego']%
    Typ := z_tego
    , Block := 1

korelat = 'do' 'tego' %KORELAT['do','tego']%
    Typ := do_tego
    , Block := 1

korelat = 'co' 'do' 'tego' %KORELAT['co','do','tego']%
    Typ := co_do_tego
    , Block := 1

korelat = 'temu' %KORELAT['temu']%
    Typ := temu
    , Block := 1


#

uzdanieże = zdanieże* %zdanieże%
    Typ := brak

uzdanieże = korelat zdanieże*   %zdanieże[korelat+]%
    Typ := korelat.Typ, setscore 2,
    Block := 1

#
kcz0 = pkcz* uzdanieże %pkcz[+uzdanieże:dop3]%
    Wyp3 = false, Dop3 = że, Wyp3 := 1, uzdanieże.Typ = Przyim3,
    (uzdanieże.Typ = brak, setscore 2; setscore 3)

#

uzdanieby = zdanieby*  %zdanieby%
    Typ := brak

uzdanieby = korelat zdanieby* %zdanieby[korelat+]%
    Typ := korelat.Typ, setscore 2,
    Block := 1

#
kcz0 = pkcz* uzdanieby %pkcz[+uzdanieby:dop3]%
     Wyp3 = false,
     Dop3 = by,
     Wyp3 := 1,
     uzdanieby.Typ = Przyim3,
         setscore 3

#

uzdaniejak = zdaniejak* %zdaniejak%
    Typ := brak

uzdaniejak = korelat zdaniejak* %zdaniejak[korelat+]%
    Typ := korelat.Typ, setscore 2,
    Block := 1

kcz0 = pkcz* uzdaniejak %pkcz[+uzdaniejak:dop3]%
    Wyp3 = false, Dop3 = jak, Wyp3 := 1, uzdaniejak.Typ = Przyim3,
    setscore 3

#

#

upytanie = przecinek? pytanie*  %ZOB[przecinek,pytanie]%
    Typ := brak

#
#
upytanie = korelat przecinek? pytanie*  %ZOB[korelat,przecinek,pytanie]%
    Block := 1


kcz0 = pkcz* upytanie %pkcz[+upytanie:dop3]%
    Wyp3 = false, Dop3 = ob, Wyp3 := 1, upytanie.Typ = Przyim3,
    setscore 4

#

#
#
#
pkcz = pkcz* kcz %pkcz[+kcz:dop3]%
    kcz.C = bezokolicznik,
    (kcz.Hk = 255 ; kcz.Hk = false),
    Wyp3 = false, Dop3 = bk, Wyp3 := 1, setscore 1,
    (kcz.Podmiot = potencjalny_wypełn, Podmiot = 1, O == 3, Podmiot := wypełn ;
         kcz.Podmiot = brak, Podmiot = brak;
     kcz.Podmiot = potencjalny),
    {kcz.Neg = oczekiwana, pkcz.Neg != 1, Neg := oczekiwana},
    {kcz.Neg = 1, Neg := 1},
#
#
      { Dop1 = false,
    ( kcz.Dop1, kcz.Wyp1 = false, Wyp1 := false, Dop1 := kcz.Dop1,
      Przyim1 := kcz.Przyim1,
          Sem1 := kcz.Sem1,
      {Oblig1 := kcz.Oblig1},
      L1 := kcz.L1, R1 := kcz.R1,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3) ; Ps1 := podczasownik1}
        ;
      kcz.Dop2, kcz.Wyp2 = false, {Wyp1 := false}, Dop1 := kcz.Dop2,
      Przyim1 := kcz.Przyim2,
          Sem1 := kcz.Sem2,
          {Oblig1 := kcz.Oblig2},
      L1 := kcz.L2, R1 := kcz.R2,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3); Ps1 := podczasownik2}
    ;
      kcz.Dop3, kcz.Wyp3 = false, {Wyp1 := false}, Dop1 := kcz.Dop3,
      Przyim1 := kcz.Przyim3,
          Sem1 := kcz.Sem3,
          {Oblig1 := kcz.Oblig3},
      L1 := kcz.L3, R1 := kcz.R3,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3); Ps1 := podczasownik3}
    )
       },
      { Dop2 = false,
    ( kcz.Dop1, kcz.Wyp1 = false, {Wyp2 := false}, Dop2 := kcz.Dop1,
      Przyim2 := kcz.Przyim1,
          Sem2 := kcz.Sem1,
          {Oblig2 := kcz.Oblig1},
      L2 := kcz.L1, R2 := kcz.R1,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3); Ps2 := podczasownik1}
        ;
      kcz.Dop2, kcz.Wyp2 = false, {Wyp2 := false}, Dop2 := kcz.Dop2,
      Przyim2 := kcz.Przyim2,
          Sem2 := kcz.Sem2,
          {Oblig2 := kcz.Oblig2},
      L2 := kcz.L2, R2 := kcz.R2,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3); Ps2 := podczasownik2}
    ;
      kcz.Dop3, kcz.Wyp3 = false, {Wyp2 := false}, Dop2 := kcz.Dop3,
      Przyim2 := kcz.Przyim3,
          Sem2 := kcz.Sem3,
          {Oblig2 := kcz.Oblig3},
      L2 := kcz.L3, R2 := kcz.R3,
          {(kcz.Ps1 = podczasownik1, Ps1 := podpodczasownik1;
        kcz.Ps1 = podczasownik2, Ps1 := podpodczasownik2;
        kcz.Ps1 = podczasownik3, Ps1 := podpodczasownik3); Ps2 := podczasownik3}
    )
       },
#
    {kcz.Zwrotność != false, kcz.Zwrotność != wypełn, Zwrotność = false,
         Zwrotność := kcz.Zwrotność},
    {OczekiwanyPrzyim := kcz.OczekiwanyPrzyim,
         OczekiwanyP := kcz.OczekiwanyP}

#

#
#

#
#
#

kcz0 = pkcz*

#
kcz0 = 'się' kcz0* %kcz0['się'+]%
    Zwrotność = 1,
    Zwrotność := wypełn,
    setscore 2

#
kcz0 = 'się' kcz0* %kcz0['się':czynne+]%
    (Podmiot = 1 ; Podmiot = oblig ;
     (Podmiot = potencjalny, C = bezokolicznik)),
    Zwrotność = false,
    O = 3, L = 1, R == n,
    Podmiot := brak,
    Zwrotność := wypełn

#
#
#
#
#
#
kcz0 = 'się' kcz0* %kcz0['się':bierne+]%
    (Podmiot = 1 ; Podmiot = oblig ;
     (Podmiot = potencjalny, C = bezokolicznik)),
    Zwrotność = false,
    (Wyp1 = false, Przyim1 = brak, (Dop1 = dop ; Dop1 = bier),
     Wyp1 := 1,
     S := Sem1;
     Wyp2 = false, Przyim2 = brak, (Dop2 = dop ; Dop2 = bier),
     Wyp2 := 1,
     S := Sem2;
     Wyp3 = false, Przyim3 = brak, (Dop3 = dop ; Dop3 = bier),
     Wyp3 := 1,
     S := Sem3),
    Zwrotność := wypełn

#
#
#

#
kcz0 = czasownik_być kcz0*  %kcz0[czasownik_być:posiłk+]%
    Neg = false,
    czasownik_być.C = przyszły,
    A :== ndk,
    C = przeszły,
    C := przyszły,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    O := czasownik_być.O,
    # "będę robił" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż)

#
kcz0 = zznie czasownik_być kcz0* %kcz0[zznie+,czasownik_być:posiłk+]%
    (Neg = false ; Neg = oczekiwana),
    A :== ndk,
    czasownik_być.C = przyszły,
    C = przeszły,
    C := przyszły,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    O := czasownik_być.O,
    Neg := 1,
    # "nie będę robił" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    setscore 3

#
#
#
kcz0 = czasownik_być kcz0*
    Neg = false,
    A :== ndk,
    czasownik_być.C = przyszły,
    C = bezokolicznik,
        C := przyszły,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    O :== czasownik_być.O,
    (Podmiot = potencjalny, Podmiot := 1;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)

#
kcz0 = zznie czasownik_być kcz0*
    (Neg = false ; Neg = oczekiwana),
    A :== ndk,
    czasownik_być.C = przyszły,
    C = bezokolicznik,
    C := przyszły,
    R := czasownik_być.R,
    L := czasownik_być.L,
    O := czasownik_być.O,
    Neg := 1,
    (Podmiot = potencjalny, Podmiot := 1;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    setscore 3


#

#
kcz0 = '$zostać' kcz0*
    Neg = false,
    A :== dk,
    (P == mian ; (P == miejsc, '$zostać'.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := '$zostać'.C,
    O := '$zostać'.O,
    R :== '$zostać'.R,
    L :== '$zostać'.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := '$zostać'.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)

#
kcz0 = zznie '$zostać' kcz0*
    Neg = false,
    A :== dk,
    (P == mian ; (P == miejsc, '$zostać'.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := '$zostać'.C,
    O := '$zostać'.O,
    R :== '$zostać'.R,
    L :== '$zostać'.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := '$zostać'.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    Neg := 1, setscore 3

#


#
kcz0 = czasownik_być kcz0*
    Neg = false,
    A :== any,
    (P == mian ; (P == miejsc, czasownik_być.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := czasownik_być.C,
    O := czasownik_być.O,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := czasownik_być.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1)

#
kcz0 = zznie czasownik_być kcz0*
    Neg = false,
    A :== any,
    (P == mian ; (P == miejsc, czasownik_być.C = bezokolicznik)),
    C = imiesłów_bierny,
    C := czasownik_być.C,
    O := czasownik_być.O,
    R :== czasownik_być.R,
    L :== czasownik_być.L,
    # "zostanę zmiażdżony" nie powinno być interpretowane z R = mnż
    (O == 3 ; L == 2 ; R == mż ; R == ż),
    (Podmiot = potencjalny, Podmiot := czasownik_być.Podmiot;
         Podmiot = potencjalny_wypełn, Podmiot := wypełn;
     1),
    Neg := 1, setscore 3

#

#

#
kcz0 =  uszfrz kcz0*
        (kcz0 != kcz0; C != odsłownik;
         uszfrz.Core = "on"; uszfrz.Core = "ona"; uszfrz.Core = "ono";
     uszfrz.Core = "oni"; uszfrz.Core = "one"
),  # po lewej stronie odsłownika nie dołączamy
    (( Wyp1 = false,
        uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
        Wyp1 := 1,
        (uszfrz.O != 3; L1 :== uszfrz.L, R1 :== uszfrz.R),
       (Neg = false ; Dop1 != bier ; Przyim1 != brak),
        (kcz0 = pkcz;
         (Ps1 != podczasownik1, Ps1 != podczasownik2, Ps1 != podczasownik3,
          Ps1 != podpodczasownik1, Ps1 != podpodczasownik2, Ps1 != podpodczasownik3);
         Przyim1 != brak;
         1
    ),
          %kcz0[uszfrz:dop1+]%);
     ( Wyp2 = false,
        uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
        Wyp2 := 1,
        (uszfrz.O != 3; L2 :== uszfrz.L, R2 :== uszfrz.R),
       (Neg = false ; Dop2 != bier ; Przyim2 != brak),
        (kcz0 = pkcz;
         (Ps2 != podczasownik1, Ps2 != podczasownik2, Ps2 != podczasownik3,
          Ps2 != podpodczasownik1, Ps2 != podpodczasownik2, Ps2 != podpodczasownik3);
         Przyim2 != brak;
         1
    ),
          %kcz0[uszfrz:dop2+]%);
     ( Wyp3 = false,
        uszfrz.P == Dop3, uszfrz.Przyim == Przyim3,
        Wyp3 := 1,
        (uszfrz.O != 3; L3 :== uszfrz.L, R3 :== uszfrz.R),
       (Neg = false ; Dop3 != bier ; Przyim3 != brak),
        (kcz0 = pkcz;
         (Ps3 != podczasownik1, Ps3 != podczasownik2, Ps3 != podczasownik3,
          Ps3 != podpodczasownik1, Ps3 != podpodczasownik2, Ps3 != podpodczasownik3);
         Przyim3 != brak;
         1
    ),
          %kcz0[uszfrz:dop3+]%)),
    {uszfrz.Neg = 1, Neg = false, Neg := oczekiwana},
    setscore 1,
    {uszfrz.Wyp1 = 1, setscore 1},
    {uszfrz.Wyp2 = 1, setscore 1}

kcz0 =  hak kcz0*
    Hk := Hk & hak.Hk,
    Nr = hak.Nr,
    setscore 9

#
#
#
#
kcz0 =  krzlicz kcz0*
    (C = przeszły ; C = teraźniejszy ; C = przyszły ;
     C = bezokolicznik ; C = przypuszczający ; C = bezosobnik),
    (( Wyp1 = false,
      krzlicz.P == Dop1, Przyim1 != brak,
          Sem1 := krzlicz.S ^ kcz0.Sem1,
          Wyp1 := 1,
      L1 :== krzlicz.L, R1 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim1, OczekiwanyP := Dop1,
      %kcz0[FPR:dop1[krzlicz]+]%);
     ( Wyp2 = false,
      krzlicz.P == Dop2, Przyim2 != brak,
          Sem2 := krzlicz.S ^ kcz0.Sem2,
          Wyp2 := 1,
      L2 :== krzlicz.L, R2 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim2, OczekiwanyP := Dop2,
      %kcz0[FPR:dop2[krzlicz]+]%);
     ( Wyp3 = false,
      krzlicz.P == Dop3, Przyim3 != brak,
          Sem3 := krzlicz.S ^ kcz0.Sem3,
          Wyp3 := 1,
      L3 :== krzlicz.L, R3 :== krzlicz.R,
      OczekiwanyPrzyim := Przyim3, OczekiwanyP := Dop3,
      %kcz0[FPR:dop3[krzlicz]+]%))

#
#
kcz0 =  szfrz kcz0*
        (kcz0 != kcz0; C != odsłownik),
    {Neg = false, Neg := oczekiwana},
    (szfrz.P == dop ; szfrz.P == bier),
    (( Wyp1 = false,
      Dop1 == bier, Przyim1 = brak,
      Sem1 := szfrz.S ^ kcz0.Sem1,
      Wyp1 := 1,
      (szfrz.O != 3 ; L1 :== szfrz.L, R1 :== szfrz.R),
      %kcz0[szfrz:dop1+]%);
    ( Wyp2 = false,
      Dop2 == bier, Przyim2 = brak,
      Sem2 := szfrz.S ^ kcz0.Sem2,
      Wyp2 := 1,
      (szfrz.O != 3 ; L2 :== szfrz.L, R2 :== szfrz.R),
      %kcz0[szfrz:dop2+]%);
    ( Wyp3 = false,
      Dop3 == bier, Przyim3 = brak,
      Sem3 := szfrz.S ^ kcz0.Sem3,
      Wyp3 := 1,
      (szfrz.O != 3 ; L3 :== szfrz.L, R3 :== szfrz.R),
      %kcz0[szfrz:dop3+]%)),
    setscore 1,
    {szfrz.Wyp1 = 1, setscore 1},
    {szfrz.Wyp2 = 1, setscore 1}


#
#
#
#
#
kcz0 =  uszfprzym kcz0* %kcz0[uszfprzym:dop1+]%
    Wyp1 = false,
    ((uszfprzym.P = mian, Dop1 = prz_mian);
     (uszfprzym.P = dop, Dop1 = prz_dop);
     (uszfprzym.P = cel, Dop1 = prz_cel);
     (uszfprzym.P = bier, Dop1 = prz_bier);
     (uszfprzym.P = narz, Dop1 = prz_narz);
     (uszfprzym.P = miejsc, Dop1 = prz_miejsc)),
        uszfprzym.Przyim == Przyim1,
        ((Dop2 == bier ; Dop2 == dop), Przyim2 = brak,
     L2 :== uszfprzym.L, R2 :== uszfprzym.R ;
     (Dop3 == bier ; Dop3 == dop), Przyim3 = brak,
         L3 :== uszfprzym.L, R3 :== uszfprzym.R ;
     (Dop2 != bier, Dop2 != dop ; Przyim2 != brak),
     (Dop3 != bier, Dop3 != dop ; Przyim3 != brak),
     # @todo odsłownik ma R pusty (w przeciwieństwie do bezokolicznika)
     (C = odsłownik ; R :== uszfprzym.R, L :== uszfprzym.L)
        ),
        Wyp1 := 1,
    {uszfprzym.Wyp1 = 1, setscore 1},
    {uszfprzym.Wyp2 = 1, setscore 1}

#
#
kcz0 =  szfokol kcz0*
        (kcz0 != kcz0; C != odsłownik),
    szfokol.Typ = odprzym,
    (Wyp1 = false, Dop1 == av, Wyp1 := 1, %kcz0[szfokol:dop1+]%;
         Wyp2 = false, Dop2 == av, Wyp2 := 1, %kcz0[szfokol:dop2+]%;
     Wyp3 = false, Dop3 == av, Wyp3 := 1, %kcz0[szfokol:dop3+]%),
     {szfokol.Wyp1 = 1, setscore 1},
     {szfokol.Wyp2 = 1, setscore 1}


#
kcz0 =  szfokol kcz0*
        (kcz0 != kcz0; C != odsłownik),
    szfokol.S = loc,
    ( Wyp1 = false, Dop1 == lc, Wyp1 := 1, %kcz0[szfokol:dop1+]%;
          Wyp2 = false, Dop2 == lc, Wyp2 := 1, %kcz0[szfokol:dop2+]%;
      Wyp3 = false, Dop3 == lc, Wyp3 := 1, %kcz0[szfokol:dop3+]%),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1},
    {szfokol.Neg = 1, Neg = false, Neg := oczekiwana}

#
#
#
#
#
#
#
#
kcz0 =  szfokol kcz0* %kcz0[szfokol:okol+]%
    ('+'  = '+' ;
         Wyp3 = false ;
         (Dop3 != bk, Dop3 != odsł_mian, Dop3 != odsł_dop, Dop3 != odsł_cel,
          Dop3 != odsł_bier, Dop3 != odsł_narz, Dop3 != odsł_miejsc )),
    ('' = '+' ;
     (C != odsłownik,
      #C != imiesłów_bierny,
      C != imiesłów_czynny ; szfokol.Length = 1,
          C != imiesłów_współczesny,
          C != imiesłów_uprzedni)),
    (szfokol.Typ != odprzym;
         ((Wyp1; Dop1 != av),
      (Wyp2; Dop2 != av),
      (Wyp3; Dop3 != av))),
    (szfokol.S != loc;
         ((Wyp1; Dop1 != lc),
      (Wyp2; Dop2 != lc),
      (Wyp3; Dop3 != lc))),
    (szfokol.Przyim = false ;
     (Wyp1; Przyim1 != szfokol.Przyim; Dop1 != szfokol.P),
     (Wyp2; Przyim2 != szfokol.Przyim; Dop2 != szfokol.P),
     (Wyp3; Przyim3 != szfokol.Przyim; Dop3 != szfokol.P)),
        (szfokol.Lu = false ; kcz0.L == szfokol.Lu),
    (szfokol.Ru = false ; kcz0.R == szfokol.Ru),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1},
    {szfokol.Neg = 1, Neg = false, Neg := oczekiwana}
    # żeby FPR się przyklejała raczej do czasownika
    , setscore 1

#
#
kcz0 =  przecinek zdanie_wtrącone przecinek kcz0*

#
#
kcz0 =  przecinek szfokol przecinek kcz0*


#
#
kcz0 =  spójnik kcz0* %kcz0[spójnik:inkorporacyjny+]%
       spójnik.Ps1 = 1, spójnik.Ps3 = 3,
#
#
#
#
#
    setscore 1


#
#
kcz0 = szfrz kcz0*  %kcz0[szfrz:podmiot+]%
    ( C = przeszły; C = przyszły; C = teraźniejszy; C = przypuszczający; C = nieustalony),
    ((Podmiot = 1; Podmiot = oblig), Podmiot := wypełn;
         Podmiot = potencjalny, Podmiot := potencjalny_wypełn),
    szfrz.P == mian,
    ( L :== szfrz.L,
      R :== szfrz.R;
      L :== szfrz.L2,
      R :== szfrz.R2 ),
    (O :== szfrz.O; O :== szfrz.O1),
    S := szfrz.S ^ kcz0.S,
    {szfrz.Neg = 1, Neg = false, Neg := oczekiwana},
    setscore 1

#

kcz0 = partykuła_niech kcz0* %kcz0[partykuła_niech:posiłk+]%
    (C = teraźniejszy, A == ndk ;
    C = przyszły, A == dk ;
    Core = "być", C = przyszły),
    Typ != modalny, Typ != modalny0,
    (O == 3 ; (O == 1, L == 1)),
    C := rozkazujący,
    setscore 1

#
#
#
kcz0 = zzby kcz0* %kcz0[zzby:posiłk+]%
    C = przeszły,
    O == 3,
    C := przypuszczający
    ,
    L :== zzby.L,
    O := zzby.O

#

#
#
#
#
#
#

#

#
#
#
#
#

kcz1 = kcz0*

kcz1 = kcz0 przecinek? spójnik kcz1*  %FC[SFC[kcz0,przecinek,spójnik,kcz1]]%
    (
#
      spójnik.Ps1 = 0 ;
#
      spójnik.Ps1 = 1, (spójnik.Ps3 = 1; spójnik.Ps3 = 2), spójnik.Ps4 = 0, spójnik.Ps5 = 1  ;
#
      spójnik.Ps1 = 2, spójnik.Ps2 = 1, spójnik.Ps3 != 4
    ),
    (kcz0.C = bezokolicznik, kcz1.C = bezokolicznik ; kcz0.C = odsłownik, kcz1.C = odsłownik),
    (kcz0.Hk = 255 ; kcz0.Hk = false),
    setscore 1

#
#
#
szkcz1 = spójnik kcz0 przecinek? spójnik kcz1* %FC[SFC[FCs[spójnik{0}, kcz0], przecinek, FCs[spójnik{1}, kcz1]]]%
spójnik[0].Ps1 = 2, (spójnik[0].Ps2 = 1; spójnik[0].Ps2 = 2),
   spójnik[0].Ps3 = 4,
   (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9), setscore 2,
   Typ := spójnik[0].Typ,
   spójnik[0].Typ = spójnik[1].Typ,
   {spójnik.Neg = 1, Neg :=1},
   (kcz0.C = bezokolicznik, kcz1.C = bezokolicznik ; kcz0.C = odsłownik, kcz1.C = odsłownik),
   (kcz0.Hk = 255 ; kcz0.Hk = false)


#
#
szkcz1 = szkcz1* przecinek? spójnik kcz0 %szkcz1[FCs[przecinek, spójnik, kcz0]]%
   spójnik.Ps1 = 9, setscore 1,
   spójnik.Typ = szkcz1.Typ,
   (szkcz1.C = bezokolicznik, kcz0.C = bezokolicznik ; szkcz1.C = odsłownik, kcz0.C = odsłownik)

kcz = (kcz1* | szkcz1*)

kcz = zkcz przecinek kcz0*  %kcz0[zkcz:okol+,przecinek+]%
    (zkcz.C = imiesłów_współczesny ; zkcz.C = imiesłów_uprzedni),
    (C = przeszły ; C = teraźniejszy ; C = przyszły ; C = bezosobnik ;
         C = przypuszczający)

kcz = zdanie_niema*
    O := 3,
    Podmiot := brak,
    Dop1 := dop,
    Przyim1 := brak,
    L1 := nil,
    R1 := nil

kcz = zdanie_jest*
    O := 3,
    Podmiot := brak,
    L1 := nil,
    R1 := nil

#

#!!! TRACE dopeln.g

uszfrz = szfrz*
         Przyim := brak

uszfrz = szfokol*
    Przyim, SpecWyp = false

#

#

#
#
zplubszfprzym = zaimek_przymiotny*
    Typ = dzierżawczy

zplubszfprzym = szfprzym*

uszfprzym = zplubszfprzym*
    Przyim := brak

uszfprzym = przyimek zplubszfprzym* %FPR[przyimek,zplubszfprzym]%
    P :== przyimek.P,
    Przyim := przyimek.Typ,
    Block := 1

#

uodsłownik = zkcz*
    C = odsłownik,
    Przyim := brak

uodsłownik = przyimek zkcz*
    C = odsłownik,
    (P == mian, przyimek.P == odsł_mian ;
     P == dop, przyimek.P == odsł_dop   ;
     P == cel, przyimek.P == odsł_cel   ;
     P == bier, przyimek.P == odsł_bier ;
     P == narz, przyimek.P == odsł_narz ;
     P == miejsc, przyimek.P == odsł_miejsc;
     P :== przyimek.P
    ),
    Przyim := przyimek.Typ





#
#
#



#!!! TRACE extra.g



#

katr1 = katr1* szfokol

kprzdopełn0Z = kprzdopełn0Z* szfokol

kpsdopełn0Z = kpsdopełn0Z* szfokol


#!!! TRACE liczebnik.g



#
#
#
#
#
#
#

flicz = liczebnik_główny*

rzeczownik_liczba = rzeczownik*
    rzeczownik.S = number,
    (Core = "tysiąc";
     Core = "tys.";
     Core = "milion";
     Core = "mln";
     Core = "miliard";
     Core = "mld";
     Core = "bilion";
     Core = "bln";
     Core = "trylion")

#
#
#
flicz = liczebnik_główny* rzeczownik_liczba %FLG[liczebnik_główny, rzeczownik_liczba]%
    ((Typ == zg,
     rzeczownik_liczba.L == 2,
     P :== rzeczownik_liczba.P,
     R :== rzeczownik_liczba.Rp);
    (Typ == d,
     rzeczownik_liczba.L == 2,
     rzeczownik_liczba.P == dop,
     R :== rzeczownik_liczba.Rp);
    (Typ == zg1,
     rzeczownik_liczba.L == 1,
     P :== rzeczownik_liczba.P,
     R :== rzeczownik_liczba.Rp)),
    Typ := d,
    R := any,
    {Ps1 := false},
    {Ps2 := false},
    {Ps3 := false},
    setscore 4

#
flicz = zzjeden rzeczownik_liczba %FLG[zzjeden, rzeczownik_liczba]%
        rzeczownik_liczba.L == 1,
    P :== rzeczownik_liczba.P,
    R :== rzeczownik_liczba.Rp,
    Typ := d,
    R := any,
    {Ps1 := false},
    {Ps2 := false},
    {Ps3 := false},
    setscore 4


#
#
#
#
#
#
#
#
#
flicz = flicz liczebnik_główny* %FLG[flicz, liczebnik_główny]%
    P == flicz.P

#
    flicz = flicz* (zzprocent | '%') %FLG[flicz, FR[zzprocent]]%
    setscore 1

flicz = zzzero* %LG[zzzero]%
    Typ := d0,
    setscore 1

flicz = zzjeden* %LG[zzjeden]%
    Typ := zg1,
    setscore 1

#
flicz = fpartprzysł flicz*  %FLG[fpartprzysł,flicz]%
    fpartprzysł.Typ == licz

#!!! TRACE listowe.g





#!!! TRACE okol.g

#
#
#
#


#
#
#


#
#
#
#
#
fprzyim = przyimek frz*  %FPR[przyimek, frz]%
    P :== przyimek.P,
    SpecOblig = false,
    Składnia == pr,
    Przyim := przyimek.Typ

#
#
#
fprzyim = fprzyim* ('a' | 'i') frz %fprzyim[frz:a['a'+]]%
    (Przyim = "między" ; Przyim = "pomiędzy"),
    P :== frz.P,
    setscore 5

#
#
#
#
fprzyim = przyimek frz* %FPR[przyimek, frz]%
    C = odsłownik,
    (P == mian, przyimek.P == odsł_mian ;
     P == dop, przyimek.P == odsł_dop   ;
     P == cel, przyimek.P == odsł_cel   ;
     P == bier, przyimek.P == odsł_bier ;
     P == narz, przyimek.P == odsł_narz ;
     P == miejsc, przyimek.P == odsł_miejsc),
     Przyim := przyimek.Typ,
     setscore 1

#
fprzyim = zzsam przyimek zzsię*  %FPR[zzsam,przyimek,zzsię]%
    P :== przyimek.P,
    zzsam.P == mian,
    R := zzsam.R,
    L := zzsam.L,
    X := 10

#
fprzyim = przyimek zzsię zzsam*  %FPR[przyimek,zzsię,zzsam]%
    P == przyimek.P,
    zzsię.P == przyimek.P

#
fprzyim = przyimek zzsam* zzsię  %FPR[przyimek,zzsam,zzsię]%
    P == przyimek.P,
    zzsię.P == przyimek.P

#

#
fprzyimspec = przyimek frz*   %FSPEC[przyimek,frz]%
    P :== przyimek.P,
        SpecPrzyim = przyimek.Typ,
    Składnia == pr,
        P == SpecP,
    Typ := przyimek.Typ,
    SpecWyp := 1,
    setscore 4


#

#
#
#
okol = fprzyim*
    S := any


#
okol = zzsam*  %FPS[zzsam]%
    (zzsam.P == mian ; zzsam.L = 1, zzsam.R = mż, zzsam.P == cel),
    Ru := zzsam.R,
    Lu := zzsam.L
    , S := manner

#
#
okol = fprzyimspec*
    S := any

#
#
#
okol = fpartprzysł*
    (Typ = odprzym ; Typ = czasow ; Typ = modulant)

#
#
#
#
okol = zztam przecinek? zzgdzie zdanie_oznajm %FPS[zztam,przecinek,zzgdzie,zdanie_oznajm]%
    S := loc

#
okol = zzwtedy przecinek? zzgdy zdanie_oznajm %FPS[zzwtedy,przecinek,zzgdy,zdanie_oznajm]%
    S := time

#
okol = zztak przecinek? zzjak zdanie_oznajm %FPS[zztak:zapowiednik,przecinek,zzjak,zdanie_oznajm]%
    setscore 2
    , S := manner

#
#
#
#
#
okol = szfrz* %szfrz%
    szfrz.P = narz,
    P := narz,
    szfrz.S ^ c,
    S := manner,
    Typ := frz_narz

#
okol = szfrz* %szfrz%
    szfrz.P = cel,
    P := cel,
    szfrz.S ^ h,
    S := manner,
    Typ := frz_cel


#
okol = szfrz* 'temu' %FPR[szfrz,'temu']%
    szfrz.P = mian
    , S ^ time_unit,
    S := time

okol = 'jako' szfrz* %FPR['jako',szfrz]%

okol = dzień*
    P :== dop

#

fokol0 = okol*

#
fokol0 = okol* spójnik_jak okol  %FPS[SFPS[okol,FPSs[spójnik_jak,okol{1}]]]%

#
#
#
fokol0 = okol spójnik fokol0* %SFPS[okol,FPSs[spójnik,fokol0]]%
        spójnik.Ps1 != 8,
    (
      spójnik.Ps1 = 0
    ;
      spójnik.Ps1 = 2, (spójnik.Ps2 = 1 ; spójnik.Ps2 = 2 ; spójnik.Ps2 = 3, setscore 1), spójnik.Ps3 != 4
    ),
    (okol.Typ != frz_narz ; fokol0.Typ != frz_narz),
    (okol.Typ != frz_cel ; fokol0.Typ != frz_cel),
    (
       okol.Przyim = fokol0.Przyim,
       okol.P = fokol0.P
    ;
       {Przyim := false},
       {P := false}
    )

#
fokol0 = okol przecinek fokol0*  %SFPS[okol,przecinek,fokol0]%
      (okol.Typ != frz_narz ; fokol0.Typ != frz_narz),
      (okol.Typ != frz_cel ; fokol0.Typ != frz_cel),
           S := okol.S ^ fokol0.S,
       (okol.Przyim = fokol0.Przyim,
        okol.P = fokol0.P;
        {Przyim := false},
        {P := false}),
      fokol0.Typ != frz_cel

#

#
#
#
szfokol0 = spójnik fokol0* przecinek? spójnik fokol0 %FPS[SFPS[FPSs[spójnik{0},fokol0{0}],przecinek,FPSs[spójnik{1},fokol0{1}]]]%
    spójnik[0].Ps1 = 2, (spójnik[0].Ps2 = 1; spójnik[0].Ps2 = 2; spójnik[0].Ps2 = 3, setscore 1),
    spójnik[0].Ps3 = 4,
    (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9),
    Typ := spójnik[0].Typ,
    spójnik[0].Typ = spójnik[1].Typ,
    {S := fokol0[0].S ^ fokol0[1].S},
    (
      fokol0[0].Przyim = fokol0[1].Przyim,
      fokol0[0].P = fokol0[1].P
        ;
      Przyim := false,
          P := false
    )

szfokol0 = szfokol0* przecinek? spójnik fokol0 %szfokol0[FPSs[przecinek, spójnik, fokol0]]%
    spójnik.Ps1 = 9, setscore 1,
    spójnik.Typ = szfokol0.Typ,
    {S := szfokol0.S ^ fokol0.S},
    (
      szfokol0.Przyim = fokol0.Przyim,
      szfokol0.P = fokol0.P
        ;
      Przyim := false,
          P := 0
    )

szfokol1 = (fokol0* | szfokol0*)

szfokol = szfokol1*


#
szfokol = fpartprzysł szfokol1*  %szfokol1[fpartprzysł:partykuła+]%
    fpartprzysł.Typ = part

#
#

#
#
#
rok = liczebnik_główny zzrkropka? %FR[liczebnik_główny,zzrkropka]%
    liczebnik_główny.Ps1 = rok,
    liczebnik_główny.P = mian,
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := any,
    Przyim := brak, Składnia := any,
    S := year,
    P := any,
    R := mnż, R1 := mnż, Rp := mnż,
    setscore 1

#
#
rok = liczebnik_główny zzrok %FR[ROK[liczebnik_główny],zzrok:słowo_rok]%
    liczebnik_główny.Ps1 = rok,
    liczebnik_główny.P == mian,
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := any,
    Przyim := brak, Składnia := any,
    zzrok.L == 1,
    P := zzrok.P,
    R := mnż, R1 := mnż, Rp := mnż,
    S := year,
    setscore 1

#
#
rok = zzrok liczebnik_główny %FR[zzrok:słowo_rok,liczebnik_główny]%
    liczebnik_główny.Ps1 = rok,
    liczebnik_główny.P == mian,
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := any,
    Przyim := brak, Składnia := any,
    zzrok.L == 1,
    P := zzrok.P,
    R := mnż, R1 := mnż, Rp := mnż,
    S := year,
    setscore 10

#
#
dzień0 = liczebnik_główny rzeczownik %FR[liczebnik_główny:dzień,rzeczownik:miesiąc]%
    rzeczownik.S = month,
    rzeczownik.P == dop,
    liczebnik_główny.Ps2 = dzień,
    liczebnik_główny.P == bier,
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := any,
    Przyim := brak, Składnia := any,
    S := any,
    P := any,
    R := mnż, R1 := mnż, Rp := mnż,
    setscore 1


#
#
#
dzień0 = liczebnik_porządkowy rzeczownik %FR[liczebnik_porządkowy:dzień,rzeczownik:miesiąc]%
    rzeczownik.S = month,
    rzeczownik.P == dop,
    liczebnik_porządkowy.R = mnż,
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := any,
    Przyim := brak, Składnia := any,
    S := any,
    P := liczebnik_porządkowy.P,
    R := mnż, R1 := mnż, Rp := mnż,
    setscore 1


#
dzień = dzień0*

#
#
dzień = dzień0* rok %dzień0[+rok:rok]%
    rok.P == dop,
    setscore 10

#
dzień = dzień0* zzbrkropka %dzień0[+zzbrkropka:rok]%
    setscore 5

frz = dzień*
    S := day

frz = rok*

#
frz = rzeczownik* zzbrkropka %FR[rzeczownik,zzbrkropka:rok]%
    rzeczownik.S = month,
    setscore 5

#

#!!! TRACE przymiotnik.g

#
#
#
#
#
#
#
#
#
#
#
#

#

#
#
#
#
#
#
#
#
#
#
#
#
#
#

#

#
#
#
#
#
#
#
#
#

#

przymiotnik = liczebnik_porządkowy*

kprz0 = przymiotnik* %FP[przymiotnik]%

kprz0 = zznie przymiotnik* %FP[zznie,przymiotnik]%

#
#
#
#
#
kprzdopełn0 = kprz0*

#
kprzdopełn0 = kprzdopełn0* uszfrz
    (Wyp1 = false,
         uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
     Wyp1 := 1, setscore 3, %kprzdopełn0[+uszfrz:dop1]%);
    (Wyp2 = false,
         uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
     Wyp2 := 1, setscore 3, %kprzdopełn0[+uszfrz:dop2]%),
    {uszfrz.Wyp1 = 1, setscore 1},
    {uszfrz.Wyp2 = 1, setscore 1}


kprzdopełn0 = kprzdopełn0* szfokol
    szfokol.Typ = odprzym,
    (Wyp1 = empty, Dop1 == av, Wyp1 := 1, %kprzdopełn0[+szfokol:dop1]%;
       Wyp2 = empty, Dop2 == av, Wyp2 := 1, %kprzdopełn0[+szfokol:dop2]%),
       {szfokol.Wyp1 = 1, setscore 1},
       {szfokol.Wyp2 = 1, setscore 1}

kprzdopełn0 = kprzdopełn0* szfokol
    szfokol.S = loc,
    ( Wyp1 = empty, Dop1 == lc, Wyp1 := 1, %kprzdopełn0[+szfokol:dop1]%;
        Wyp2 = empty, Dop2 == lc, Wyp2 := 1, %kprzdopełn0[+szfokol:dop2]%),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1}

#
kprzdopełn0Z = kprzdopełn0* uodsłownik
    Wyp2 = empty,
    ((uodsłownik.P = mian, Dop2 = odsł_mian);
     (uodsłownik.P = dop, Dop2 = odsł_dop);
     (uodsłownik.P = cel, Dop2 = odsł_cel);
     (uodsłownik.P = bier, Dop2 = odsł_bier);
     (uodsłownik.P = narz, Dop2 = odsł_narz);
     (uodsłownik.P = miejsc, Dop2 = odsł_miejsc)),
    uodsłownik.Przyim == Przyim2,
    Wyp2 := 1,
    1,
    setscore 5,
    %kprzdopełn0[+uodsłownik:dop2]%

kprzdopełn0Z = kprzdopełn0* uzdanieże
    Wyp2 = empty, Dop2 = że, Wyp2 := 1, uzdanieże.Typ = Przyim2,
    (Typ = brak, setscore 3 ; że = ob, setscore 5; setscore 4),
    1, %kprzdopełn0[+uzdanieże:dop2]%

kprzdopełn0Z = kprzdopełn0* uzdanieby
    Wyp2 = empty, Dop2 = by, Wyp2 := 1, uzdanieby.Typ = Przyim2,
    (Typ = brak, setscore 3 ; by = ob, setscore 5; setscore 4),
    1, %kprzdopełn0[+uzdanieby:dop2]%

kprzdopełn0Z = kprzdopełn0* uzdaniejak
    Wyp2 = empty, Dop2 = jak, Wyp2 := 1, uzdaniejak.Typ = Przyim2,
    (Typ = brak, setscore 3 ; jak = ob, setscore 5; setscore 4),
    1, %kprzdopełn0[+uzdaniejak:dop2]%

kprzdopełn0Z = kprzdopełn0* upytanie
    Wyp2 = empty, Dop2 = ob, Wyp2 := 1, upytanie.Typ = Przyim2,
    (Typ = brak, setscore 3 ; ob = ob, setscore 5; setscore 4),
    1, %kprzdopełn0[+upytanie:dop2]%

kprzdopełn0Z = kprzdopełn0* zkcz
    zkcz.C = bezokolicznik,
    Wyp2 = empty, Dop2 = bk, Wyp2 := 1,
    1, %kprzdopełn0[+zkcz:dop2]%

#
kprzdopełn0Z = kprzdopełn0*
    1

#

#
przysłówek = 'po' po_przymiotnik* %PS['po',po_przymiotnik]%

#

kps0 = przysłówek* %FPS[przysłówek]%

kps0 = zznie przysłówek* %FPS[zznie,przysłówek]%

#
#
kpsdopełn0 = kps0*

#
kpsdopełn0 = kpsdopełn0* uszfrz
    (Wyp1 = false,
         uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
     Wyp1 := 1, setscore 3, %kpsdopełn0[+uszfrz:dop1]%);
    (Wyp2 = false,
         uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
     Wyp2 := 1, setscore 3, %kpsdopełn0[+uszfrz:dop2]%),
    {uszfrz.Wyp1 = 1, setscore 1},
    {uszfrz.Wyp2 = 1, setscore 1}


kpsdopełn0 = kpsdopełn0* szfokol
    szfokol.Typ = odprzym,
    (Wyp1 = empty, Dop1 == av, Wyp1 := 1, %kpsdopełn0[+szfokol:dop1]%;
       Wyp2 = empty, Dop2 == av, Wyp2 := 1, %kpsdopełn0[+szfokol:dop2]%),
       {szfokol.Wyp1 = 1, setscore 1},
       {szfokol.Wyp2 = 1, setscore 1}

kpsdopełn0 = kpsdopełn0* szfokol
    szfokol.S = loc,
    ( Wyp1 = empty, Dop1 == lc, Wyp1 := 1, %kpsdopełn0[+szfokol:dop1]%;
        Wyp2 = empty, Dop2 == lc, Wyp2 := 1, %kpsdopełn0[+szfokol:dop2]%),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1}

#
kpsdopełn0Z = kpsdopełn0* uodsłownik
    Wyp2 = empty,
    ((uodsłownik.P = mian, Dop2 = odsł_mian);
     (uodsłownik.P = dop, Dop2 = odsł_dop);
     (uodsłownik.P = cel, Dop2 = odsł_cel);
     (uodsłownik.P = bier, Dop2 = odsł_bier);
     (uodsłownik.P = narz, Dop2 = odsł_narz);
     (uodsłownik.P = miejsc, Dop2 = odsł_miejsc)),
    uodsłownik.Przyim == Przyim2,
    Wyp2 := 1,
    1,
    setscore 5,
    %kpsdopełn0[+uodsłownik:dop2]%

kpsdopełn0Z = kpsdopełn0* uzdanieże
    Wyp2 = empty, Dop2 = że, Wyp2 := 1, uzdanieże.Typ = Przyim2,
    (Typ = brak, setscore 3 ; że = ob, setscore 5; setscore 4),
    1, %kpsdopełn0[+uzdanieże:dop2]%

kpsdopełn0Z = kpsdopełn0* uzdanieby
    Wyp2 = empty, Dop2 = by, Wyp2 := 1, uzdanieby.Typ = Przyim2,
    (Typ = brak, setscore 3 ; by = ob, setscore 5; setscore 4),
    1, %kpsdopełn0[+uzdanieby:dop2]%

kpsdopełn0Z = kpsdopełn0* uzdaniejak
    Wyp2 = empty, Dop2 = jak, Wyp2 := 1, uzdaniejak.Typ = Przyim2,
    (Typ = brak, setscore 3 ; jak = ob, setscore 5; setscore 4),
    1, %kpsdopełn0[+uzdaniejak:dop2]%

kpsdopełn0Z = kpsdopełn0* upytanie
    Wyp2 = empty, Dop2 = ob, Wyp2 := 1, upytanie.Typ = Przyim2,
    (Typ = brak, setscore 3 ; ob = ob, setscore 5; setscore 4),
    1, %kpsdopełn0[+upytanie:dop2]%

kpsdopełn0Z = kpsdopełn0* zkcz
    zkcz.C = bezokolicznik,
    Wyp2 = empty, Dop2 = bk, Wyp2 := 1,
    1, %kpsdopełn0[+zkcz:dop2]%

#
kpsdopełn0Z = kpsdopełn0*
    1

#

kprzstopień00 = kprzdopełn0Z*

#
#
#
#
#
#
#
#
#
#
#
#
#
#
kprzstopień00 = kps0 kprzdopełn0Z*
    Stopień == równy,
     (kps0.Typ == przym ;
      kps0.Typ == przym_równy ;
          kps0.Typ == rzecz_przym ;
      kps0.Typ == przym_równy_do_wyższego,
      Stopień := wyższy, setscore 1 ;
      kps0.Typ == przym_równy_do_najwyższego,
      Stopień := najwyższy, setscore 1)


kprzstopień0 = kprzstopień00*

#
#
kprzstopień0 = kps0 kprzstopień00*
    Stopień == wyższy,
    ( kps0.Typ = przym ;
      kps0.Typ = przym_wyższy )

#

#
kpsodprzym = kpsdopełn0Z*
    Typ = czasow ; Typ = modalny

kpsstopień00 = kpsodprzym*

kpsstopień00 = kps0 kpsodprzym*
    Stopień == równy,
     (kps0.Typ == przym ;
      kps0.Typ == przym_równy ;
          kps0.Typ == rzecz_przym ;
      kps0.Typ == przym_równy_do_wyższego,
      Stopień := wyższy ;
      kps0.Typ == przym_równy_do_najwyższego,
      Stopień := najwyższy)


kpsstopień0 = kpsstopień00*

#
#
#
kpsstopień0 = kps0 kpsstopień00*
    Stopień == wyższy,
    ( kps0.Typ = przym;
      kps0.Typ = przym_wyższy )

#

kprzstopień1 = kprzstopień0*

#
#
#
#
#
#

kprzstopień1 = kprzstopień0* spójnik_od (szfrz | zplubszfprzym)  %kprzstopień0[+FNIŻ[spójnik_od,szfrz]]%
    szfrz.P == dop,
    Stopień = wyższy,
    setscore 2

#
#
#
#
kprzstopień1 = kprzstopień0* przecinek? spójnik_niż szfrz %kprzstopień0[+FNIŻ[przecinek,spójnik_niż,szfrz]]%
    szfrz.P == mian,
    Stopień = wyższy,
    setscore 2

#
#
#
#
#

#
#
#
#
kprzstopień1 = kprzstopień0* przecinek? spójnik_niż (szfokol | zdanie_elem) %kprzstopień0[+FNIŻ[przecinek,spójnik_niż,szfokol]]%
    Stopień = wyższy,
    setscore 2

#
#
#
#
#
kprzstopień1 = kprzstopień0* przyimek_spośród szfrz %kprzstopień0[+FS[przyimek_spośród,szfrz]]%
    szfrz.P == dop,
    Stopień = najwyższy

#

#
#
#
kprzstopień1 = kprzstopień0* uktóry %kprzstopień0[+ZWZGL[uktóry]]%
    uktóry.Typ = jaki,
    Stopień = najwyższy

#

kpsstopień1 = kpsstopień0*

#
#
#
kpsstopień1 = kpsstopień0* spójnik_od (szfrz | szfprzym)  %kpsstopień0[+FNIŻ[+spójnik_od,+szfrz]]%
    szfrz.P == dop,
    Stopień = wyższy,
    setscore 2

#
kpsstopień1 = kpsstopień0* spójnik_niż (szfrz | szfprzym) %kpsstopień0[+FNIŻ[spójnik_niż,szfrz]]%
    szfrz.P == mian,
    Stopień = wyższy,
    setscore 2

#
kpsstopień1 = kpsstopień0* spójnik_niż (szfokol | zdanie_elem) %kpsstopień0[+FNIŻ[spójnik_niż,szfokol]]%
    Stopień = wyższy,
    setscore 2

#
kpsstopień1 = kpsstopień0* przyimek_spośród szfrz %kpsstopień0[+FS[przyimek_spośród,szfrz]]%
    szfrz.P == dop,
    Stopień = najwyższy

#

kprzdopełn1 = kprzstopień1*

#
#

kprzdopełn1 = uszfrz kprzdopełn1*
    (Wyp1 = false,
         uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
     Wyp1 := 1, setscore 3, %kprzdopełn1[uszfrz:dop1+]%);
    (Wyp2 = false,
         uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
     Wyp2 := 1, setscore 3, %kprzdopełn1[uszfrz:dop2+]%)

kprzdopełn1Z = kprzdopełn1*
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1)

#

kpsdopełn1 = kpsstopień1*

#
kpsdopełn1 = uszfrz kpsdopełn1*
    (Wyp1 = false,
         uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
     Wyp1 := 1, setscore 3, %kpsdopełn1[uszfrz:dop1+]%);
    (Wyp2 = false,
         uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
     Wyp2 := 1, setscore 3, %kpsdopełn1[uszfrz:dop2+]%)

kpsdopełn1Z = kpsdopełn1*
    (Oblig1 = empty ; Wyp1 = 1 ; Typ = modalny),
    (Oblig2 = empty ; Wyp2 = 1 ; Typ = modalny)

#

#
kprzzdp = kprzdopełn1Z*

kprzzdp = ('wystarczająco' | 'na' 'tyle') kprzdopełn1Z*
    Stopień = równy ; Stopień = wyższy,
    setscore 1

#
#
kprzzdp = ('wystarczająco' | 'na' 'tyle') kprzdopełn1Z* (zdanieby | zdanieże)?
    Stopień = równy ; Stopień = wyższy,
    setscore 2

#
#
kprzzdp = 'tak' kprzdopełn1Z*
    Stopień = równy,
    setscore 1

kprzzdp = 'tak' kprzdopełn1Z* (zdanieby | zdanieże | zdaniejak)
    Stopień = równy,
    setscore 2

kprzzdp = 'tak'? kprzdopełn1Z* 'jak' szfrz
    Stopień = równy,
    szfrz.P == mian,
    setscore 1

#
#
#
kprzzdp = '$taki' kprzdopełn1Z*
    Stopień = równy,
    L :== '$taki'.L,
    P :== '$taki'.P,
    R :== '$taki'.R

kprzzdp = '$taki' kprzdopełn1Z* (zdanieby | zdanieże)
    Stopień = równy,
    L :== '$taki'.L,
    P :== '$taki'.P,
    R :== '$taki'.R

kprzzdp = '$taki' kprzdopełn1Z* 'jak' szfrz
    Stopień = równy,
    L :== '$taki'.L,
    P :== '$taki'.P,
    R :== '$taki'.R,
    szfrz.P == mian

#
kprzzdr = '@taki'*


#

#
kpszdp = kpsdopełn1Z*

kpszdp = ('wystarczająco' | 'na' 'tyle') kpsdopełn1Z*  (zdanieby | zdanieże)
    (Stopień = równy ; Stopień = wyższy),
    setscore 3

kpszdp = 'tak' kpsdopełn1Z*
    Stopień = równy,
    setscore 3

kpszdp = 'tak' kpsdopełn1Z* (zdanieby | zdanieże | zdaniejak)
    Stopień = równy,
    setscore 5

kpszdp = 'tak'? kpsdopełn1Z* 'jak' szfokol
    Stopień = równy,
    setscore 1

#

kprz = kprzzdp*

#
kprz = zkcz*  %FP[zkcz]%
    (C = imiesłów_bierny ; C = imiesłów_czynny ;
     C = imiesłów_bierny_współczesny ;
     C = imiesłów_czynny_uprzedni)

#

fpartprzysł = kpszdp*

#
#
fpartprzysł = kpsdopełn0Z*
    Typ != czasow, Typ != modalny

#

fprz0 = kprz*

#
#
#
#
#
#
fprz0 = kprz przecinek spójnik fprz0*   %FP[SFP[kprz,przecinek,FPs[spójnik,fprz0]]]%
(
  spójnik.Ps1 = 0
;
  spójnik.Ps1 = 2, (spójnik.Ps2 = 0; spójnik.Ps2 = 1 ; spójnik.Ps2 = 2 ; spójnik.Ps2 = 3, setscore 1), spójnik.Ps3 != 4
),
L :== kprz.L,
P :== kprz.P,
R :== kprz.R
, S := kprz.S ^ fprz0.S

fprz0 = kprz spójnik fprz0*   %FP[SFP[kprz,FPs[spójnik,fprz0]]]%
(
  spójnik.Ps1 = 0
;
  spójnik.Ps1 = 2, (spójnik.Ps2 = 0; spójnik.Ps2 = 1 ; spójnik.Ps2 = 2 ; spójnik.Ps2 = 3, setscore 1), spójnik.Ps3 != 4
),
L :== kprz.L,
P :== kprz.P,
R :== kprz.R
, S := kprz.S ^ fprz0.S

#
#
#
#
fprz0 = kprz przecinek fprz0*  %FP[SFP[kprz,przecinek,fprz0]]%
    L :== kprz.L,
    P :== kprz.P,
    R :== kprz.R
    , S := kprz.S ^ fprz0.S

#
#
#
#
#
szfprzym0 = spójnik fprz0* przecinek spójnik fprz0 %FP[SFP[FPs[spójnik{0},fprz0{0}],przecinek,FPs[spójnik{1},fprz0{1}]]]%
    spójnik[0].Ps1 = 2, (spójnik[0].Ps2 = 1; spójnik[0].Ps2 = 2; spójnik[0].Ps2 = 3, setscore 1),
    spójnik[0].Ps3 = 4,
    (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9), setscore 2,
    Typ := spójnik[0].Typ,
    spójnik[0].Typ = spójnik[1].Typ,
    P :== fprz0[1].P,
    L :== fprz0[1].L,
    R :== fprz0[1].R
    , S := fprz0[0].S ^ fprz0[1].S

szfprzym0 = spójnik fprz0* spójnik fprz0 %FP[SFP[FPs[spójnik{0},fprz0{0}],FPs[spójnik{1},fprz0{1}]]]%
    spójnik[0].Ps1 = 2, (spójnik[0].Ps2 = 1; spójnik[0].Ps2 = 2; spójnik[0].Ps2 = 3, setscore 1),
    spójnik[0].Ps3 = 4,
    (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9), setscore 2,
    Typ := spójnik[0].Typ,
    spójnik[0].Typ = spójnik[1].Typ,
    P :== fprz0[1].P,
    L :== fprz0[1].L,
    R :== fprz0[1].R
    , S := fprz0[0].S ^ fprz0[1].S

#
#
#
szfprzym0 = szfprzym0* przecinek spójnik fprz0 %szfprzym0[przecinek,FPs[spójnik, fprz0]]%
    spójnik.Ps1 = 9, setscore 1,
    spójnik.Typ = szfprzym0.Typ,
    P :== fprz0.P,
    L :== fprz0.L,
    R :== fprz0.R
    , S := szfprzym0.S ^ fprz0.S

szfprzym0 = szfprzym0* spójnik fprz0 %szfprzym0[FPs[spójnik, fprz0]]%
    spójnik.Ps1 = 9, setscore 1,
    spójnik.Typ = szfprzym0.Typ,
    P :== fprz0.P,
    L :== fprz0.L,
    R :== fprz0.R
    , S := szfprzym0.S ^ fprz0.S

szfprzym = (fprz0* | szfprzym0*)

#

#!!! TRACE rzeczownik.g

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#



#
#
#
#
#
#
rzeczownik = przymiotnik* %R[przymiotnik]%
    Składnia = jakorz,
    (R = mż ; R = ż ; R = mo ; R = mno, (P = mian ; P = bier)),
    O := 3,
    Rp := R,
    Przyim := brak,
    Składnia := any

#

#

#
#
#
coś_nieleksykalnego = element_nieleksykalny*

#
coś_nieleksykalnego = angielska_nazwa_własna*
setscore 1

#
#
coś_nieleksykalnego = nazwa_własna*

#
#
coś_nieleksykalnego = akronim*

#
coś_nieleksykalnego = angielski_wyraz*

#
ciąg_nieleksykalny = coś_nieleksykalnego*  %FN[coś_nieleksykalnego]%

#
#
#
#
#
ciąg_nieleksykalny = ciąg_nieleksykalny* coś_nieleksykalnego
        setscore 4

#
ciąg_nieleksykalny = ciąg_nieleksykalny* liczebnik_główny
        liczebnik_główny.P = mian, liczebnik_główny.R == mnż,
        liczebnik_główny.Sh = digits


#

#

#
#
#
#
krz = rzeczownik*  %FR[rzeczownik]%
    Typ != pron, Typ != gen_pl, Typ != gen_sg

#
#
krz = czasownik*   %FR[czasownik]%
    C = odsłownik,
    Length = 1,
    L := 1, R := n, Rp := n,
    #! Uwaga! jeśli chce się zmienić semantykę odsłownika, należy
    #! też zmodyfikować funkcję PEDic2AttrConverter::moreSpecificTransfer.
    O := 3,
    {SpecPrzyim := false}, {SpecP := false}

#

#

katr0 = krz*

#
#
#
#
#
#
#
katr0 = krz* kprz
#
    kprz.Length = 1,
    L :== kprz.L,
    P :== kprz.P,
    R :== kprz.R

#

katr00 = katr0*

#
#
#
#
katr00 = katr0* przymiotnik_nieodmienny

kwłasna = katr00*

#
#
#
#
#
#
#
#
kwłasna = katr00* ciąg_nieleksykalny
    setscore 4

#
numer = liczebnik_główny*   %FN[liczebnik_główny]%
    Sh = digits,
    P == mian, R == mnż

#
#
#
krzdop = katr00* numer

kwłasna = ciąg_nieleksykalny* %FR[ciąg_nieleksykalny]%
    L := 1, R := any, Rp := any,
    Sem := any,
    O := 3,
    P := any, L1 := 1, R1 := any, Składnia := any,
    Przyim := brak

kwłasna = katr00* rzeczownik
    rzeczownik.Typ = nazwa_własna,
    (rzeczownik.P == mian ; P == rzeczownik.P)

kwłasna = rzeczownik rzeczownik* %FR[rzeczownik,rzeczownik{1}]%
    S = surname,
    rzeczownik.S = "first_name",
    P :== rzeczownik.P,
    setscore 2

kwłasna = rzeczownik rzeczownik* %FR[rzeczownik,rzeczownik{1}]%
    S = surname,
    rzeczownik.S = "first_name",
    (P :== rzeczownik.P ; rzeczownik.R = ż, P = mian, P := rzeczownik.P),
    setscore 2

#
#
#
#
#
#
#
#

kwłasna = angielska_nazwa_własna rzeczownik* %FR[angielska_nazwa_własna,rzeczownik]%
    S = surname,
    angielska_nazwa_własna.S = "first_name",
    setscore 1

kwłasna = rzeczownik rzeczownik krz*
    S = surname,
    rzeczownik.S = "first_name",
    rzeczownik[1].S = "first_name",
    P :== rzeczownik.P,
    P :== rzeczownik[1].P,
    setscore 4

#
#
kwłasna = rzeczownik kwłasna* %kwłasna[rzeczownik:apoz+]%
    rzeczownik.Typ = appos,
    (S = surname ; Sh = 1),
    P :== rzeczownik.P,
    L :== rzeczownik.L,
    R :== rzeczownik.R,
    setscore 4

#

#


krzdop = kwłasna*

#
#
#
#
#
#
#
#
#
#
#
#
#
#
krzdop = kwłasna* szfrz %kwłasna[+szfrz:dopełniacz]%
    szfrz.P = dop,
    szfrz.Podmiot != niedopełn

#


#
#
#
#
#
#
#
#
#
krzdopełn = krzdop*

#
krzdopełn = krzdopełn* uszfrz
    (Wyp1 = false,
         uszfrz.P == Dop1, uszfrz.Przyim == Przyim1,
     Wyp1 := 1, setscore 3, %krzdopełn[+uszfrz:dop1]%);
    (Wyp2 = false,
         uszfrz.P == Dop2, uszfrz.Przyim == Przyim2,
     Wyp2 := 1, setscore 3, %krzdopełn[+uszfrz:dop2]%),
    {uszfrz.Wyp1 = 1, setscore 1},
    {uszfrz.Wyp2 = 1, setscore 1}


krzdopełn = krzdopełn* szfokol
    szfokol.Typ = odprzym,
    (Wyp1 = empty, Dop1 == av, Wyp1 := 1, %krzdopełn[+szfokol:dop1]%;
       Wyp2 = empty, Dop2 == av, Wyp2 := 1, %krzdopełn[+szfokol:dop2]%),
       {szfokol.Wyp1 = 1, setscore 1},
       {szfokol.Wyp2 = 1, setscore 1}

krzdopełn = krzdopełn* szfokol
    szfokol.S = loc,
    ( Wyp1 = empty, Dop1 == lc, Wyp1 := 1, %krzdopełn[+szfokol:dop1]%;
        Wyp2 = empty, Dop2 == lc, Wyp2 := 1, %krzdopełn[+szfokol:dop2]%),
    {szfokol.Wyp1 = 1, setscore 1},
    {szfokol.Wyp2 = 1, setscore 1}

#
krzdopełnZ = krzdopełn* uodsłownik
    Wyp2 = empty,
    ((uodsłownik.P = mian, Dop2 = odsł_mian);
     (uodsłownik.P = dop, Dop2 = odsł_dop);
     (uodsłownik.P = cel, Dop2 = odsł_cel);
     (uodsłownik.P = bier, Dop2 = odsł_bier);
     (uodsłownik.P = narz, Dop2 = odsł_narz);
     (uodsłownik.P = miejsc, Dop2 = odsł_miejsc)),
    uodsłownik.Przyim == Przyim2,
    Wyp2 := 1,
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1),
    setscore 5,
    %krzdopełn[+uodsłownik:dop2]%

krzdopełnZ = krzdopełn* uzdanieże
    Wyp2 = empty, Dop2 = że, Wyp2 := 1, uzdanieże.Typ = Przyim2,
    (Typ = brak, setscore 3 ; że = ob, setscore 5; setscore 4),
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1), %krzdopełn[+uzdanieże:dop2]%

krzdopełnZ = krzdopełn* uzdanieby
    Wyp2 = empty, Dop2 = by, Wyp2 := 1, uzdanieby.Typ = Przyim2,
    (Typ = brak, setscore 3 ; by = ob, setscore 5; setscore 4),
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1), %krzdopełn[+uzdanieby:dop2]%

krzdopełnZ = krzdopełn* uzdaniejak
    Wyp2 = empty, Dop2 = jak, Wyp2 := 1, uzdaniejak.Typ = Przyim2,
    (Typ = brak, setscore 3 ; jak = ob, setscore 5; setscore 4),
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1), %krzdopełn[+uzdaniejak:dop2]%

krzdopełnZ = krzdopełn* upytanie
    Wyp2 = empty, Dop2 = ob, Wyp2 := 1, upytanie.Typ = Przyim2,
    (Typ = brak, setscore 3 ; ob = ob, setscore 5; setscore 4),
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1), %krzdopełn[+upytanie:dop2]%

krzdopełnZ = krzdopełn* zkcz
    zkcz.C = bezokolicznik,
    Wyp2 = empty, Dop2 = bk, Wyp2 := 1,
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1), %krzdopełn[+zkcz:dop2]%

#
krzdopełnZ = krzdopełn*
    (Oblig1 = empty ; Wyp1 = 1),
    (Oblig2 = empty ; Wyp2 = 1)

#

katr1 = krzdopełnZ*

katr1 = katrSa*

#
#
#
#
katr1 = krzdopełnZ* przecinek? szfprzym
    szfprzym.Length != 1,
    L :== szfprzym.L,
    P :== szfprzym.P,
    R :== szfprzym.R


#

#
#
#
#
#
#
#
#
#
#
#
#
#
katr1 = szfprzym katr1*
    L :== szfprzym.L,
    (P :== szfprzym.P;
     P = woł, L == 1,
     (R = mo; R = mż; R = mnż; R = ż),
         szfprzym.P = mian), # drogi kliencie!
    R :== szfprzym.R,
    (szfprzym.Wyp2 = false;        # dopełnienia tylko rzeczownikowe
     szfprzym.Dop2 = mian;
     szfprzym.Dop2 = dop;
     szfprzym.Dop2 = cel;
     szfprzym.Dop2 = bier;
     szfprzym.Dop2 = narz;
     szfprzym.Dop2 = miejsc)


katr2 = katr1*

#
#
#
#
#

#
katr2 = szfprzym katr1* zdanie_niż
    szfprzym.Stopień == wyższy,
    L :== szfprzym.L,
    P :== szfprzym.P,
    R :== szfprzym.R,
    zdanie_niż,
    setscore 2

#
katr2 = szfprzym katr1* przyimek_spośród (szfrz | szfprzym) %katr1[+szfprzym[+FS[przyimek_spośród,szfrz]]]%
    szfprzym.Stopień == najwyższy,
    L :== szfprzym.L,
    P :== szfprzym.P,
    R :== szfprzym.R,
    szfrz.P == dop,
    szfrz.L == 2,
    setscore 2

#

#

krzpodrz = katr2*

#
#
#
krzpodrz = katr2* uktóry %katr2[+ZWZGL[uktóry]]%
    R :== uktóry.Ru,
    L :== uktóry.Lu,
    (uktóry.Typ = który ; uktóry.Typ = jaki ;
     uktóry.Typ = co_zaimek)


#

#

#
#
#
#
#
#
#
#
#
#
#
#
#
krzlicz0 = flicz krzpodrz*
    (
     flicz.Typ == zg,
     L == 2,
     P :== flicz.P,
     (Rp :== flicz.R ; Rp = n, flicz.R = mnż)
    ;
     flicz.Typ == d,
     L == 2,
     P == dop,
     (Rp :== flicz.R ; Rp = n, flicz.R = mnż),
     ( flicz.P != mian ; (L := 1, L1 := 1, Rw := R, R := n, R1 := n)),
     P := flicz.P
    ;
     flicz.Typ == zg1,
         L == 1,
     P :== flicz.P,
     Rp := flicz.R
    ;
     flicz.Typ = 1,
     L == 1
    ;
     flicz.Typ == d0,
     (flicz.P != mian ;
      P == dop, L := 1, L1 := 1, Rw := R, R := n, R1 := n),
      P := flicz.P
     )

#
#
krzlicz0 = flicz przyimek_spośród krzlicz* %FR[flicz,FS[przyimek_spośród,krzlicz]]%
    krzlicz.P == dop,
    krzlicz.L == 2,
    Rp :== flicz.R,
    P := flicz.P,
    (flicz.Typ == zg1, L := 1, L1 := 1, R := Rp; L := 2),
    O := 3,
    ( flicz.Typ != d ; flicz.P != mian ;
    (L := 1, L1 := 1, Rw := R, R := n, R1 := n))

krzlicz = krzpodrz*

krzlicz = krzlicz0*

#
krzlicz = flicz zzprocent %FR[flicz, R[zzprocent]]%
    L := 2,
    L1 := 1,
    S := number,
    P := flicz.P,
    Przyim := brak,
    R := mno,
    Rp := mnż,
    R1 := mno,
    Składnia := any,
    O := 3

#
#
#
krzlicz = kprz krzlicz0*
    (Rw, kprz.P == dop, kprz.L == 2, Rw :== kprz.R ;
         Rw = false, L :== kprz.L, P :== kprz.P, R :== kprz.R)


#
#
krzlicz = rzeczownik* flicz %FR[rzeczownik,flicz]%
    flicz.Sh != digits,
    ((flicz.Typ == zg,
     L :== 2,
     P :== flicz.P,
     Rp :== flicz.R);
    (flicz.Typ == d,
     L :== 2,
     P :== dop,
     Rp :== flicz.R,
     ( flicz.P != mian ; (L := 1, L1 := 1, R := n, R1 := n)),
     P := flicz.P))

#
krzlicz = flicz krzpodrz*
    flicz.Typ == f,
    L :== 1,
    P :== dop,
    Rp :== flicz.R,
    P := flicz.P,
    R := n, R1 := n,
    setscore 1

#
#
krzlicz = flicz krzpodrz*
    flicz.R = zbiorowy,
    L :== 2,
    (Rp :== n ; Rp :== mo),
    (((flicz.P == miejsc; flicz.P == cel), P :== flicz.P) ;
     P == dop, P := flicz.P),
    ( flicz.P != mian ; (L := 1, L1 := 1, R := n, R1 := n))

#

#


#
#

krzpartprzysł = fpartprzysł krzpodrz* %krzpodrz[fpartprzysł:kwant+]%
    fpartprzysł.Typ == kwant,
    krzpodrz.P == dop,
    L := 1, L1 := 1,
    R := n, R1 := n,
    P := mian

krzpartprzysł = fpartprzysł krzpodrz* %krzpodrz[fpartprzysł:kwant+]%
    fpartprzysł.Typ == kwant,
    krzpodrz.P == dop,
    L := 1, L1 := 1,
    R := n, R1 := n,
    P := dop

krzpartprzysł = fpartprzysł krzpodrz* %krzpodrz[fpartprzysł:kwant+]%
    fpartprzysł.Typ == kwant,
    krzpodrz.P == dop,
    L := 1, L1 := 1,
    R := n, R1 := n,
    P := bier

#

#
kzaimków_nieokreślonych0 = zaimek_rzeczowny_nieokreślony*   %FR[zaimek_rzeczowny_nieokreślony]%

#
#
#
#
#
kzaimków_nieokreślonych0 = zaimek_rzeczowny_nieokreślony* szfprzym %FR[zaimek_rzeczowny_nieokreślony,szfprzym]%
    szfprzym.L == 1,
    R :== szfprzym.R,
    ( (R != n ; (P != mian, P != bier)), P :== szfprzym.P ;
          (R == n, (P == mian ; P == bier), szfprzym.P == dop))
    , S := zaimek_rzeczowny_nieokreślony.S ^ szfprzym.S

kzaimków_nieokreślonych = kzaimków_nieokreślonych0*

#
#
#
kzaimków_nieokreślonych = kzaimków_nieokreślonych0* przyimek_spośród szfrz %kzaimków_nieokreślonych0[+FS[przyimek_spośród,szfrz]]%
    szfrz.P == dop,
    szfrz.L == 2
    , S := kzaimków_nieokreślonych0.S ^ szfrz.S


#
#
#
kzaimków_nieokreślonych = kzaimków_nieokreślonych0* uktóry %kzaimków_nieokreślonych0[+ZWZGL[uktóry]]%
    (uktóry.Typ == kto ; uktóry.Typ == co),
    R :== uktóry.R,
    L :== uktóry.L
    , S := kzaimków_nieokreślonych0.S ^ uktóry.Su

#

#
#
kkażdy = zzkażdy*   %FR[zzkażdy]%
    Typ == nominalny,
    L :== 1, R :== mż,
    L1 := L, R1 := R, Rp := mo,
    S := h,
    Składnia := any, Przyim := brak,
    O := 3


#
#
kkażdy = zzkażdy* uktóry  %FR[zzkażdy, ZWZGL[uktóry]]%
    uktóry.Typ == kto,
    Typ == nominalny,
    R :== uktóry.R,
    L :== uktóry.L,
    L :== 1, R :== mż,
    L1 := L, R1 := R, Rp := mo,
    S := h,
    Składnia := any, Przyim := brak,
    O := 3


#
#
kkażdy = zzkażdy* kspośród %FR[zzkażdy,kspośród]%
    Typ == nominalny,
    L :== 1, R :== mż,
    L1 := L, R1 := R, Rp := mo,
    S := kspośród.S ^ h,
    Składnia := any, Przyim := brak,
    O := 3, setscore 2


#

#

kwszystko = zzwszystko* %FR[zzwszystko]%

#
kwszystko = zzwszystko* uktóry %FR[zzwszystko,ZWZGL[uktóry]]%
    uktóry.Typ == co,
    R :== uktóry.R,
    L :== uktóry.L

#
#
kwszystko = zzwszystko* zzten uktóry %FR[zzwszystko,zzten,ZWZGL[uktóry]]%
    P :== zzten.P,
    R :== zzten.R,
    L :== zzten.L,
    R :== uktóry.R,
    L :== uktóry.L,
    uktóry.Typ == co

#
kwszystko = zzten zzwszystko* uktóry %FR[zzten,zzwszystko,ZWZGL[uktóry]]%
    P :== zzten.P,
    R :== zzten.R,
    L :== zzten.L,
    R :== uktóry.R,
    L :== uktóry.L,
    uktóry.Typ == co

#

#
kwszyscy = zzwszyscy* %FR[zzwszyscy]%


#
#
kwszyscy = zaimek_rzeczowny_osobowy* zzwszyscy %FR[zaimek_rzeczowny_osobowy, zzwszyscy]%
    R :== zzwszyscy.R,
    L :== zzwszyscy.L,
    P :== zzwszyscy.P,
    R1 := R, L1 := L

#
kwszyscy = zzwszyscy* uktóry %FR[zzwszyscy,ZWZGL[uktóry]]%
    uktóry.Typ == który,
    R :== uktóry.R,
    L :== uktóry.L

#
kwszyscy = zzwszyscy* zzten uktóry %FR[zzwszyscy,zzten,ZWZGL[uktóry]]%
    P :== zzten.P,
    R :== zzten.R,
    L :== zzten.L,
    R :== uktóry.R,
    L :== uktóry.L,
    uktóry.Typ == który

#
kwszyscy = zzten zzwszyscy* uktóry %FR[zzten,zzwszyscy,ZWZGL[uktóry]]%
    P :== zzten.P,
    R :== zzten.R,
    L :== zzten.L,
    R :== uktóry.R,
    L :== uktóry.L,
    uktóry.Typ == który

#

#
kten0 = (zzten* | zzżaden*) %FR[zzten]%
    L1 := L, R1 := R, Rp := mo,
    S := any,
    Składnia := any, Przyim := brak,
    O := 3

kten0 = (zzten* | zzżaden*) kspośród %FR[zzten,kspośród]%
    L1 := L, R1 := R, Rp := mo,
    S := kspośród.S,
    Składnia := any, Przyim := brak,
    O := 3,
    setscore 2


#
#
#
kten = kten0* uktóry %kten0[+ZWZGL[uktóry]]%
    (uktóry.Typ == który ; (uktóry.Typ == kto, L == 1, R == mż))

#

#
#
#
kzaimekto = zzto* przecinek? pytanie %FR[zzto,przecinek,pytanie]%
    Przyim := brak,
    S := abstr,
    setscore 4

#
kzaimekto = zzto* zdanieże %FR[zzto,zdanieże]%
    Przyim := brak,
    S := abstr,
    setscore 2

#
kzaimekto = zzto* zdanieby %FR[zzto,zdanieby]%
    Przyim := brak,
    S := abstr,
    setscore 2

#

#
kżaden = zzżaden* %FR[zzżaden]%
    L1 := L, R1 := R, Rp := any,
    S := any,
    Składnia := any, Przyim := brak,
    O := 3

#

#
#
#
#
krzwsk0 = zaimek_dzierżawczy krzlicz*
    (Rw, zaimek_dzierżawczy.P == dop,
         zaimek_dzierżawczy.L == 2, Rw :== zaimek_dzierżawczy.R ;
     Rw = false,
         P :== zaimek_dzierżawczy.P,
     R :== zaimek_dzierżawczy.R,
     L :== zaimek_dzierżawczy.L)

#
krzwsk0 = krzlicz* zaimek_dzierżawczy
    zaimek_dzierżawczy.P != any, # nie chcemy 'jej', 'jego', 'ich'
    (Rw, zaimek_dzierżawczy.P == dop,
         zaimek_dzierżawczy.L == 2, Rw :== zaimek_dzierżawczy.R ;
     Rw = false,
         P :== zaimek_dzierżawczy.P,
     R :== zaimek_dzierżawczy.R,
     L :== zaimek_dzierżawczy.L)

#
#
#
#
krzwsk0 = zaimek_dzierżawczy zzwszystek krzlicz*
    zzwszystek.L == 2,
         (Rw, zaimek_dzierżawczy.P == dop,
          zaimek_dzierżawczy.L == 2, Rw :== zaimek_dzierżawczy.R,
      zzwszystek.P == dop, Rw :== zzwszystek.R;
      Rw = false,
      P :== zaimek_dzierżawczy.P,
      R :== zaimek_dzierżawczy.R,
      L :== zaimek_dzierżawczy.L,
      P :== zzwszystek.P,
      R :== zzwszystek.R,
      L :== zzwszystek.L)

krzwsk = krzwsk0*

krzwsk1 = krzwsk0*

krzwsk1 = krzlicz*

#
#
#
krzwsk = zaimek_wskazujący (krzlicz* | krzwsk)
    L :== zaimek_wskazujący.L,
    R :== zaimek_wskazujący.R,
    P :== zaimek_wskazujący.P,
    setscore 4

#
#
krzwsk = zaimek_przymiotny krzwsk1*
    (zaimek_przymiotny.Typ = kwant ;
     zaimek_przymiotny.Typ = pytajny),
    L :== zaimek_przymiotny.L,
    R :== zaimek_przymiotny.R,
    P :== zaimek_przymiotny.P

#
#
krzwsk = zaimek_przymiotny* kspośród %FR[zaimek_przymiotny,kspośród]%
    zaimek_przymiotny.Typ = kwant,
    ( zaimek_przymiotny.L == 2, R :== kspośród.R ;
      zaimek_przymiotny.L == 1,
          ( kspośród.Rp == n, R :== n ;
            kspośród.Rp == ż, R :== ż ;
        kspośród.Rp == mż, R :== mż ;
        kspośród.Rp == mo, R :== mż ;
            kspośród.Rp == mnż, R :== mnż )),
    R1 := R, L1 := L,
    S := any,
    Składnia := any, Przyim := brak, O := 3,
    setscore 1

#

frzpoj0 = krzlicz*

frzpoj0 = krzpartprzysł*

frzpoj0 = kzaimków_nieokreślonych*

frzpoj0 = kkażdy*

frzpoj0 = kwszystko*

frzpoj0 = kwszyscy*

frzpoj0 = kten*

frzpoj0 = kzaimekto*

frzpoj0 = kżaden*

frzpoj0 = krzwsk*

#
#
#
#
#
zaimek_rzeczowny_osobowy = rzeczownik*
    Typ = pron, setscore 4

frzpoj0 = zaimek_rzeczowny_osobowy* %FR[zaimek_rzeczowny_osobowy]%
    Podmiot := niedopełn,
    { P = cel, L = 1, R != n, setscore 1}

#

#
frzpoj0 = '@to'* %FR['@to']%
    Podmiot := niedopełn,
    setscore 1

#
#
#
frzpoj0 = zaimek_rzeczowny_osobowy* flicz  %FR[zaimek_rzeczowny_osobowy,flicz]%
    (flicz.Typ == zg,
     L :== 2,
     P :== flicz.P,
     Rp :== flicz.R);
    (flicz.Typ == d,
     #L :== 2,
     P == dop,
     Rp :== flicz.R,
     ( flicz.P != mian ; (L := 1, L1 := 1, R := n, R1 := n)),
     P := flicz.P)

#
#
frzdwajznas = flicz przyimek_spośród zaimek_rzeczowny_osobowy*  %FR[flicz,FS[przyimek_spośród,FR[zaimek_rzeczowny_osobowy]]]%
    zaimek_rzeczowny_osobowy.P == dop,
    zaimek_rzeczowny_osobowy.L == 2,
    zaimek_rzeczowny_osobowy.Składnia == pr,
    Składnia := any,
    Rp :== flicz.R,
    P := flicz.P,
    (flicz.Typ == zg1, L := 1, L1 := 1; L := 2),
    O := 3,
    ( flicz.Typ != d ; flicz.P != mian ; (L := 1, L1 := 1, R := n, R1 := n))

#
frzdwajznas = zaimek_wskazujący flicz przyimek_spośród zaimek_rzeczowny_osobowy* %FR[zaimek_wskazujący,flicz,FS[przyimek_spośród,FR[zaimek_rzeczowny_osobowy]]]%
    zaimek_rzeczowny_osobowy.P == dop,
    zaimek_rzeczowny_osobowy.L == 2,
    (flicz.Typ == d, flicz.P == mian, zaimek_wskazujący.P == dop;
         zaimek_wskazujący.P == flicz.P),
    Rp :== flicz.R,
    P := flicz.P,
    (flicz.Typ == zg1, L := 1; L := 2),
    O := 3,
    ( flicz.Typ != d ; flicz.P != mian ; (L := 1, R := n))

frzpoj0 = frzdwajznas*

frzpoj0 = frzdwajznas* uktóry

#
frzpoj0 = '@się'* %FR['@się']%
    P != mian

#
frzpoj0 = '@się'* 'nawzajem' %FR['@się','nawzajem']%
    P != mian, L := 2, setscore 4

frzpoj0 = '@sam' '@się'* %FR['@sam','@się']%

frzpoj0 = '@się'* '@sam'  %FR['@się', '@sam']%



#
#
#
frzpoj0 = akronim* %FR[akronim]%
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := mian,
    S := any,
    Przyim := brak, Składnia := any

frzpoj0 = akronim* %FR[akronim]%
    L := 1, L1 := 1, R1 := n, R := n, Rp := n, O := 3, P := bier,
    S := any,
    Przyim := brak, Składnia := any

#
#
#
#
#
frzpoj1 = zznie frzpoj0* %frzpoj0[zznie:partykuła+]%

frzpoj = frzpoj0*

#
#
frzpoj = fpartprzysł frzpoj0*  %frzpoj0[fpartprzysł:partykuła+]%
    (fpartprzysł.Typ == rzecz ; fpartprzysł.Typ == rzecz_przym)

#
#
frzpoj = fpartprzysł frzpoj1* %frzpoj1[fpartprzysł:partykuła+]%
    (fpartprzysł.Typ == rzecz ; fpartprzysł.Typ == rzecz_przym)

#
#
#
frzpoj = zkcz*   %FR[zkcz]%
    Length != 1,
    C = odsłownik,
#
#
    L := 1, R := n, Rp := n,
    O := 3,
    {SpecPrzyim := false}, {SpecP := false},
    setscore 1

#
frzpoj = rzeczownik* ('@ten' | '@taki') %FR[rzeczownik,'@ten']%
    L :== '@ten'.L,
    R :== '@ten'.R,
    P :== '@ten'.P

#

frz0 = frzpoj*
    L1 := L,
    R1 := R



#
#
#
#
#
#
#
frz0_ze_spójnikiem = frzpoj przecinek? spójnik frz0*   %FR[SFR[frzpoj, przecinek, FRs[spójnik,frz0]]]%
    Ps1 != 8,
    (
      spójnik.Ps1 = 0 ;
      spójnik.Ps1 = 2, (spójnik.Ps2 = 1 ; spójnik.Ps2 = 2), spójnik.Ps3 != 4
    ),
    frzpoj.P == frz0.P,
    P :== frzpoj.P,
    L := 2,
    (frz0.Rp == frzpoj.Rp,
         ((frz0.Rp != any), Rp := frz0.Rp; Rp := frzpoj.Rp) ;
     Rp := any),
    L1 := frzpoj.L,
    R1 := frzpoj.R,
    O1 := frzpoj.O,
    L2 := frz0.L,
    R2 := frz0.R,
    O := frz0.O,
    ( frzpoj.L == 2, frz0.L == 2,
      ( frzpoj.R = frz0.R; R := any) ;
          frzpoj.R = any, frz0.R = any, R := any;
      frzpoj.L == 1, frz0.L == 1,
       (((frzpoj.Rp = mo; frz0.Rp = mo), R := mo); R := mno) ;
      frzpoj.L == 1, frz0.L == 2,
       (((frzpoj.Rp = mo; frz0.Rp = mo), R := mo); R := mno) ;
      frzpoj.L == 2, frz0.L == 1,
       (((frzpoj.Rp = mo; frz0.Rp = mo), R := mo); R := mno)
        )

#
frz0_ze_spójnikiem = frzpoj przecinek frz0_ze_spójnikiem* %FR[SFR[frzpoj,przecinek,frz0_ze_spójnikiem]]%
    frzpoj.P == frz0_ze_spójnikiem.P,
    P :== frzpoj.P,
    L := 2,
    (frz0_ze_spójnikiem.Rp == frzpoj.Rp,
         ((frz0_ze_spójnikiem.Rp != any), Rp := frz0_ze_spójnikiem.Rp; Rp := frzpoj.Rp) ;
     Rp := any),
    L1 := frzpoj.L,
    R1 := frzpoj.R,
    O1 := frzpoj.O,
    L2 := frz0_ze_spójnikiem.L,
    R2 := frz0_ze_spójnikiem.R,
    O := frz0_ze_spójnikiem.O,
    ( frzpoj.L == 2, frz0_ze_spójnikiem.L == 2,
      ( frzpoj.R = frz0_ze_spójnikiem.R; R := any) ;
          frzpoj.R = any, frz0_ze_spójnikiem.R = any, R := any;
      frzpoj.L == 1, frz0_ze_spójnikiem.L == 1,
       (((frzpoj.Rp = mo; frz0_ze_spójnikiem.Rp = mo), R := mo); R := mno) ;
      frzpoj.L == 1, frz0_ze_spójnikiem.L == 2,
       (((frzpoj.Rp = mo; frz0_ze_spójnikiem.Rp = mo), R := mo); R := mno) ;
      frzpoj.L == 2, frz0_ze_spójnikiem.L == 1,
       (((frzpoj.Rp = mo; frz0_ze_spójnikiem.Rp = mo), R := mo); R := mno)
        )

frz0 = frz0_ze_spójnikiem*

frz = frz0*

#
#
#
#
frz = frz0* spójnik_jak frz0  %FR[SFR[frz0,spójnik_jak,frz0{1}]]%
  (frz0[1].P == mian ; P :== frz0[1].P),
  spójnik_jak.Ps1 = 5

#

#
#

#
#
#
#
#
#
#
#

#
#
#
#
#
#
#
#
#

#
#
#
szfrz0 = spójnik frz0 przecinek? spójnik frz* %FR[SFR[FRs[spójnik{0}, frz0], przecinek, FRs[spójnik{1}, frz]]]%
   spójnik[0].Ps1 = 2, (spójnik[0].Ps2 = 1; spójnik[0].Ps2 = 2),
   spójnik[0].Ps3 = 4,
   (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9), setscore 2,
   Typ := spójnik[0].Typ,
   spójnik[0].Typ = spójnik[1].Typ,
   {spójnik.Neg = 1, Neg :=1},
   frz0.P == frz.P,
    P :== frz0.P,
    L := 2,
    (frz.Rp == frz0.Rp,
         ((frz.Rp != any), Rp := frz.Rp; Rp := frz0.Rp) ;
     Rp := any),
    L1 := frz0.L,
    R1 := frz0.R,
    O1 := frz0.O,
    L2 := frz.L,
    R2 := frz.R,
    O := frz.O,
    ( frz0.L == 2, frz.L == 2,
      ( frz0.R = frz.R; R := any) ;
          frz0.R = any, frz.R = any, R := any;
      frz0.L == 1, frz.L == 1,
       (((frz0.Rp = mo; frz.Rp = mo), R := mo); R := mno) ;
      frz0.L == 1, frz.L == 2,
       (((frz0.Rp = mo; frz.Rp = mo), R := mo); R := mno) ;
      frz0.L == 2, frz.L == 1,
       (((frz0.Rp = mo; frz.Rp = mo), R := mo); R := mno)
        )

#
#
szfrz0 = szfrz0* przecinek? spójnik frz %szfrz0[+SFR[FRs[przecinek, spójnik, frz]]]%
   spójnik.Ps1 = 9, setscore 1,
   spójnik.Typ = szfrz0.Typ,
   szfrz0.P == frz.P,
    P :== szfrz0.P,
    L := 2,
    (frz.Rp == szfrz0.Rp,
         ((frz.Rp != any), Rp := frz.Rp; Rp := szfrz0.Rp) ;
     Rp := any),
    L1 := szfrz0.L,
    R1 := szfrz0.R,
    O1 := szfrz0.O,
    L2 := frz.L,
    R2 := frz.R,
    O := frz.O,
    ( szfrz0.L == 2, frz.L == 2,
      ( szfrz0.R = frz.R; R := any) ;
          szfrz0.R = any, frz.R = any, R := any;
      szfrz0.L == 1, frz.L == 1,
       (((szfrz0.Rp = mo; frz.Rp = mo), R := mo); R := mno) ;
      szfrz0.L == 1, frz.L == 2,
       (((szfrz0.Rp = mo; frz.Rp = mo), R := mo); R := mno) ;
      szfrz0.L == 2, frz.L == 1,
       (((szfrz0.Rp = mo; frz.Rp = mo), R := mo); R := mno)
        )

#
#
#
#


#

#
wyliczanka0 = frzpoj przecinek frzpoj* %FR[SFR[frzpoj{0},przecinek{0},frzpoj{1}]]%
    P :== frzpoj[0].P,
        L1 := frzpoj[0].L,
        R1 := frzpoj[0].R,
        L2 := frzpoj[1].L,
        R2 := frzpoj[1].R,
    Block := 1

#
#
wyliczanka = frzpoj przecinek (wyliczanka0* | wyliczanka*) %FR[SFR[frzpoj,przecinek,wyliczanka]]%
        P :== frzpoj[0].P,
    L1 := frzpoj.L,
    R1 := frzpoj.R,
    L2 := wyliczanka0.L2,
    R2 := wyliczanka0.R2,
    {Block := false},
    setscore 8

frz = wyliczanka*

#

#
#
#
szfrz = (szfrz0* | frz*)
    Składnia == npr,
    SpecOblig != 1

#
szfrz = '"' szfrz* '"'

#

#!!! TRACE slots.g

#
#
#
#
#
#


ws1 = with_slots*




#
#


#


ws1 = ws1* zdanieże
    (Oblig1 = slot, Dop1 = że, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zdanieże:slot1]%;
     Oblig2 = slot, Dop2 = że, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zdanieże:slot2]%;
     Oblig3 = slot, Dop3 = że, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zdanieże:slot3]%)

ws1 = ws1* zdanieby
    (Oblig1 = slot, Dop1 = by, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zdanieby:slot1]%;
     Oblig2 = slot, Dop2 = by, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zdanieby:slot2]%;
     Oblig3 = slot, Dop3 = by, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zdanieby:slot3]%)

#

ws1 = ws1* uodsłownik
    uodsłownik.P = mian,
    (Oblig1 = slot, Dop1 = odsł_mian, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_mian, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_mian, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

ws1 = ws1* uodsłownik
    uodsłownik.P = dop,
    (Oblig1 = slot, Dop1 = odsł_dop, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_dop, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_dop, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

ws1 = ws1* uodsłownik
    uodsłownik.P = cel,
    (Oblig1 = slot, Dop1 = odsł_cel, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_cel, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_cel, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

ws1 = ws1* uodsłownik
    uodsłownik.P = bier,
    (Oblig1 = slot, Dop1 = odsł_bier, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_bier, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_bier, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

ws1 = ws1* uodsłownik
    uodsłownik.P = narz,
    (Oblig1 = slot, Dop1 = odsł_narz, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_narz, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_narz, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

ws1 = ws1* uodsłownik
    uodsłownik.P = miejsc,
    (Oblig1 = slot, Dop1 = odsł_miejsc, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+uodsłownik:slot3]%)

#

ws1 = ws1* szfrz
    (Oblig1 = slot, (Dop1 == szfrz.P), (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, (Dop2 == szfrz.P), (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, (Dop3 == szfrz.P), (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%)

#

#
ws1 = ws1* szfrz
    szfrz.P = dop, szfrz.Podmiot != niedopełn,
    ((Oblig1 = slot, Dop1 = p1, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p1, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p1, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p2, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p2, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p2, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p3, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p3, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p3, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p4, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p4, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p4, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p5, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p5, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p5, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p6, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p6, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p6, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p7, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p7, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p7, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p8, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p8, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p8, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p9, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p9, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p9, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p10, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p10, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p10, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p11, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p11, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p11, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+szfrz:slot3]%))


#

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.P == mian,
     (zaimek_dzierżawczy.R == mż ;
      zaimek_dzierżawczy.R == mnż) ;
         zaimek_dzierżawczy.P == bier, zaimek_dzierżawczy.R == mnż),
    (Oblig1 = slot, Dop1 = p1, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p1, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p1, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.P == dop,
     (zaimek_dzierżawczy.R == mż ;
      zaimek_dzierżawczy.R == mnż ;
      zaimek_dzierżawczy.R == n) ;
         zaimek_dzierżawczy.P == bier, zaimek_dzierżawczy.R == mż),
    (Oblig1 = slot, Dop1 = p2, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p2, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p2, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.P == cel,
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.R == mż ;
     zaimek_dzierżawczy.R == mnż ;
     zaimek_dzierżawczy.R == n),
    (Oblig1 = slot, Dop1 = p3, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p3, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p3, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    ((zaimek_dzierżawczy.P == narz ; zaimek_dzierżawczy.P == miejsc),
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.R == mż ;
     zaimek_dzierżawczy.R == mnż ;
     zaimek_dzierżawczy.R == n)),
    (zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.P == cel),
    (Oblig1 = slot, Dop1 = p4, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p4, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p4, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)


ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == n, (zaimek_dzierżawczy.P == mian ; zaimek_dzierżawczy.P == bier) ;
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.R == mno, (zaimek_dzierżawczy.P == mian; zaimek_dzierżawczy.P == bier),
    (Oblig1 = slot, Dop1 = p5, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p5, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p5, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == ż, zaimek_dzierżawczy.P == mian,
    (Oblig1 = slot, Dop1 = p6, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p6, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p6, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == ż,
    (zaimek_dzierżawczy.P == dop ; zaimek_dzierżawczy.P == cel; zaimek_dzierżawczy.P == miejsc),
    (Oblig1 = slot, Dop1 = p7, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p7, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p7, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    (zaimek_dzierżawczy.P == bier ;
     zaimek_dzierżawczy.P == narz),
    zaimek_dzierżawczy.L == 1,
    zaimek_dzierżawczy.R == ż,
    (Oblig1 = slot, Dop1 = p8, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p8, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p8, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.R == mo, zaimek_dzierżawczy.P == mian,
    (Oblig1 = slot, Dop1 = p9, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p9, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p9, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 2, (zaimek_dzierżawczy.P == dop ; zaimek_dzierżawczy.P == miejsc),
    (Oblig1 = slot, Dop1 = p10, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p10, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p10, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zaimek_dzierżawczy
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.P == narz,
    (Oblig1 = slot, Dop1 = p11, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p11, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p11, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zaimek_dzierżawczy:slot3]%)

ws1 = ws1* zkcz
    zkcz.C = bezokolicznik,
    (Oblig1 = slot, Dop1 = bk, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+zkcz:slot1]%;
     Oblig2 = slot, Dop2 = bk, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+zkcz:slot2]%;
     Oblig3 = slot, Dop3 = bk, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+zkcz:slot3]%)

ws1 = ws1* (sen | simple_sen)
    (Oblig1 = slot, Dop1 = rc, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+sen:slot1]%;
     Oblig2 = slot, Dop2 = rc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+sen:slot2]%;
     Oblig3 = slot, Dop3 = rc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+sen:slot3]%)

ws1 = ws1* flicz
    flicz.P == mian, (Oblig1 = slot, Dop1 = lg_mian, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_mian, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_mian, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)

ws1 = ws1* flicz
    flicz.P == dop, (Oblig1 = slot, Dop1 = lg_dop, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_dop, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_dop, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)

ws1 = ws1* flicz
    flicz.P == cel, (Oblig1 = slot, Dop1 = lg_cel, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_cel, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_cel, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)

ws1 = ws1* flicz
    flicz.P == bier, (Oblig1 = slot, Dop1 = lg_bier, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_bier, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_bier, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)

ws1 = ws1* flicz
    flicz.P == narz, (Oblig1 = slot, Dop1 = lg_narz, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_narz, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_narz, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)

ws1 = ws1* flicz
    flicz.P == miejsc, (Oblig1 = slot, Dop1 = lg_miejsc, (Przyim1 = Frag, Wyp1 != 1), Wyp1 := 1, %ws1[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = Frag, Wyp2 != 1), Wyp2 := 1, %ws1[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = Frag, Wyp3 != 1), Wyp3 := 1, %ws1[+flicz:slot3]%)


ws1 =  zdanieże with_slots*
    (Oblig1 = slot, Dop1 = że, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zdanieże:slot1]%;
     Oblig2 = slot, Dop2 = że, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zdanieże:slot2]%;
     Oblig3 = slot, Dop3 = że, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zdanieże:slot3]%)

ws1 =  zdanieby with_slots*
    (Oblig1 = slot, Dop1 = by, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zdanieby:slot1]%;
     Oblig2 = slot, Dop2 = by, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zdanieby:slot2]%;
     Oblig3 = slot, Dop3 = by, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zdanieby:slot3]%)

#

ws1 =  uodsłownik with_slots*
    uodsłownik.P = mian,
    (Oblig1 = slot, Dop1 = odsł_mian, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_mian, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_mian, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

ws1 =  uodsłownik with_slots*
    uodsłownik.P = dop,
    (Oblig1 = slot, Dop1 = odsł_dop, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_dop, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_dop, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

ws1 =  uodsłownik with_slots*
    uodsłownik.P = cel,
    (Oblig1 = slot, Dop1 = odsł_cel, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_cel, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_cel, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

ws1 =  uodsłownik with_slots*
    uodsłownik.P = bier,
    (Oblig1 = slot, Dop1 = odsł_bier, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_bier, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_bier, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

ws1 =  uodsłownik with_slots*
    uodsłownik.P = narz,
    (Oblig1 = slot, Dop1 = odsł_narz, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_narz, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_narz, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

ws1 =  uodsłownik with_slots*
    uodsłownik.P = miejsc,
    (Oblig1 = slot, Dop1 = odsł_miejsc, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+uodsłownik:slot1]%;
     Oblig2 = slot, Dop2 = odsł_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+uodsłownik:slot2]%;
     Oblig3 = slot, Dop3 = odsł_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+uodsłownik:slot3]%)

#

ws1 =  szfrz with_slots*
    (Oblig1 = slot, (Dop1 == szfrz.P), (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, (Dop2 == szfrz.P), (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, (Dop3 == szfrz.P), (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%)

#

#
ws1 =  szfrz with_slots*
    szfrz.P = dop, szfrz.Podmiot != niedopełn,
    ((Oblig1 = slot, Dop1 = p1, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p1, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p1, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p2, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p2, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p2, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p3, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p3, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p3, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p4, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p4, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p4, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p5, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p5, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p5, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p6, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p6, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p6, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p7, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p7, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p7, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p8, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p8, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p8, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p9, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p9, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p9, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p10, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p10, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p10, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%);
     (Oblig1 = slot, Dop1 = p11, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+szfrz:slot1]%;
     Oblig2 = slot, Dop2 = p11, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+szfrz:slot2]%;
     Oblig3 = slot, Dop3 = p11, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+szfrz:slot3]%))


#

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.P == mian,
     (zaimek_dzierżawczy.R == mż ;
      zaimek_dzierżawczy.R == mnż) ;
         zaimek_dzierżawczy.P == bier, zaimek_dzierżawczy.R == mnż),
    (Oblig1 = slot, Dop1 = p1, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p1, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p1, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.P == dop,
     (zaimek_dzierżawczy.R == mż ;
      zaimek_dzierżawczy.R == mnż ;
      zaimek_dzierżawczy.R == n) ;
         zaimek_dzierżawczy.P == bier, zaimek_dzierżawczy.R == mż),
    (Oblig1 = slot, Dop1 = p2, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p2, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p2, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.P == cel,
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.R == mż ;
     zaimek_dzierżawczy.R == mnż ;
     zaimek_dzierżawczy.R == n),
    (Oblig1 = slot, Dop1 = p3, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p3, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p3, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    ((zaimek_dzierżawczy.P == narz ; zaimek_dzierżawczy.P == miejsc),
    zaimek_dzierżawczy.L == 1,
    (zaimek_dzierżawczy.R == mż ;
     zaimek_dzierżawczy.R == mnż ;
     zaimek_dzierżawczy.R == n)),
    (zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.P == cel),
    (Oblig1 = slot, Dop1 = p4, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p4, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p4, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)


ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == n, (zaimek_dzierżawczy.P == mian ; zaimek_dzierżawczy.P == bier) ;
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.R == mno, (zaimek_dzierżawczy.P == mian; zaimek_dzierżawczy.P == bier),
    (Oblig1 = slot, Dop1 = p5, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p5, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p5, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == ż, zaimek_dzierżawczy.P == mian,
    (Oblig1 = slot, Dop1 = p6, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p6, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p6, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 1, zaimek_dzierżawczy.R == ż,
    (zaimek_dzierżawczy.P == dop ; zaimek_dzierżawczy.P == cel; zaimek_dzierżawczy.P == miejsc),
    (Oblig1 = slot, Dop1 = p7, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p7, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p7, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    (zaimek_dzierżawczy.P == bier ;
     zaimek_dzierżawczy.P == narz),
    zaimek_dzierżawczy.L == 1,
    zaimek_dzierżawczy.R == ż,
    (Oblig1 = slot, Dop1 = p8, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p8, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p8, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.R == mo, zaimek_dzierżawczy.P == mian,
    (Oblig1 = slot, Dop1 = p9, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p9, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p9, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 2, (zaimek_dzierżawczy.P == dop ; zaimek_dzierżawczy.P == miejsc),
    (Oblig1 = slot, Dop1 = p10, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p10, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p10, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zaimek_dzierżawczy with_slots*
    zaimek_dzierżawczy.L == 2, zaimek_dzierżawczy.P == narz,
    (Oblig1 = slot, Dop1 = p11, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zaimek_dzierżawczy:slot1]%;
     Oblig2 = slot, Dop2 = p11, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zaimek_dzierżawczy:slot2]%;
     Oblig3 = slot, Dop3 = p11, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zaimek_dzierżawczy:slot3]%)

ws1 =  zkcz with_slots*
    zkcz.C = bezokolicznik,
    (Oblig1 = slot, Dop1 = bk, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+zkcz:slot1]%;
     Oblig2 = slot, Dop2 = bk, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+zkcz:slot2]%;
     Oblig3 = slot, Dop3 = bk, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+zkcz:slot3]%)

ws1 =  (sen | simple_sen) with_slots*
    (Oblig1 = slot, Dop1 = rc, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+sen:slot1]%;
     Oblig2 = slot, Dop2 = rc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+sen:slot2]%;
     Oblig3 = slot, Dop3 = rc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+sen:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == mian, (Oblig1 = slot, Dop1 = lg_mian, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_mian, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_mian, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == dop, (Oblig1 = slot, Dop1 = lg_dop, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_dop, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_dop, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == cel, (Oblig1 = slot, Dop1 = lg_cel, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_cel, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_cel, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == bier, (Oblig1 = slot, Dop1 = lg_bier, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_bier, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_bier, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == narz, (Oblig1 = slot, Dop1 = lg_narz, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_narz, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_narz, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)

ws1 =  flicz with_slots*
    flicz.P == miejsc, (Oblig1 = slot, Dop1 = lg_miejsc, (Przyim1 = 0, Wyp1 != 1), Wyp1 := 1, %with_slots[+flicz:slot1]%;
     Oblig2 = slot, Dop2 = lg_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Przyim2 = 0, Wyp2 != 1), Wyp2 := 1, %with_slots[+flicz:slot2]%;
     Oblig3 = slot, Dop3 = lg_miejsc, (Oblig1 != slot ; Wyp1 = 1), (Oblig2 != slot ; Wyp2 = 1), (Przyim3 = 0, Wyp3 != 1), Wyp3 := 1, %with_slots[+flicz:slot3]%)


#
ws1 = ws1* frag
    (Oblig1 != slot; Przyim1 != Frag; Wyp1 = 1),
    (Oblig2 != slot; Przyim2 != Frag; Wyp2 = 1),
    (Oblig3 != slot; Przyim3 != Frag; Wyp3 = 1),
    Nr = frag.Nr,
    Hk = frag.Frag,
    {Hk := frag.Hk},
    {Frag := frag.Frag}

#
wsZ = ws1*
    Hk = last_frag,
    (Oblig1 != slot ; Wyp1 = 1),
    (Oblig2 != slot ; Wyp2 = 1),
    (Oblig3 != slot ; Wyp3 = 1),
    Hk := 255

#
#
zdanie = wsZ* %Z[wsZ]%
        Symbol, Symbol != przysłówek, Symbol != rzeczownik, Symbol != przymiotnik

rzeczownik = wsZ* %R[wsZ]%
    Symbol = rzeczownik

przymiotnik = wsZ* %P[wsZ]%
    Symbol = przymiotnik

okol = wsZ* %FPS[wsZ]%
    Symbol = przysłówek

hak = wsZ*
    Hk := SuperHk,
    Nr := SuperNr

#

#












#!!! TRACE slowniczek.g


#
#
#
#
#

przecinek = ','

myślnik = '-'

dwukropek = ':'

spójnik = spójnik* (przecinek | myślnik)
          Ps5 = 3

spójnik = spójnik* dwukropek
          Ps5 = 7

spójnik_jak = spójnik*
    Typ = jak

#
spójnik_niż = spójnik*
    Ps1 = 2, Typ = niż

przyimek_spośród = przyimek*
    (przyimek.Typ == spośród ; przyimek.Typ == z),
    przyimek.P == dop

zzwszystek = '@wszystek'*

zzwszystko = '@wszystko'*

zzten = '@ten'*

zzto = '$to'*

uuto = 'to'

zzsam = '$sam'*

zzsiebie = '@się'*

zzsię = '$się'*

zzjak = '$jak'*

zzby = '$by'*

zzzero = '$zero'*

zzjeden = '$jeden'*

zzprocent = '$procent'*

zzkażdy = '@każdy'*

zzżaden = '@żaden'*

zzwszyscy = '@wszyscy'*

zznie = 'nie'

zzniema = 'nie' 'ma'  %FC['nie','ma']%

zzwłaśnie = 'właśnie'

zzrkropka = 'r.'

zzrok = '@rok'*

zzbrkropka = 'br.'

partykuła_niech = ('niech' | 'niechaj' | 'niechże')

#
#
czasownik_być = '$być'*

zaimekktóry = '@który'*
    Typ = względny,
    Typ := który,
    Rp := R

zaimekktóry = '@jaki'*
    Typ = względny,
    Typ := jaki,
    Rp := R

zaimekktóry = '@kto'*
    Typ := kto

zaimekktóry = '@co'*
    Typ := co

zztam = 'tam'

zzgdzie = 'dokąd'

zzgdzie = 'gdzie'

zzgdzie = 'skąd'

zzwtedy = 'wtedy'

zzgdy = 'gdy'

zzgdy = 'kiedy'

zztak = 'tak'

zzjak = 'jak'



kspośród = przyimek_spośród frzpoj* %FS[przyimek_spośród,frzpoj]%
    Składnia == pr,
    P :== dop,
    L :== 2,
    SpecPrzyim = false
    , S := frzpoj.S ^ przyimek_spośród.S

zaimek_dzierżawczy = zaimek_przymiotny*
    Typ = dzierżawczy

zaimek_wskazujący = zaimek_przymiotny*
    Typ = wskazujący


#
spójnik_ale = 'ale'

spójnik_czyli = 'czyli'

spójnik_więc = 'więc'

spójnik_by = ('@by'* | '@żeby'* | '@aby'* | '@ażeby'*)

#

spójnik_od = ('od' | 'ode') %SPÓJNIK_OD['od']%

#
#

#
#

wykrzyknik = ( '!' | '!!' | '!!!' | '!!!!' | '!!!!!' | '!?' | '?!' | '?!?' | '!?!')

#!!! TRACE zdania.g


zkcz = kcz*
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )

#
#
#
#
#

#
#
zdanie_elem = zkcz*
    Length != 1 ; C != rozkazujący

zdanie_elem = zdanie_to*

#

#
#
zdanie_to = szfokol? szfrz uuto* szfokol? szfrz %FC[szfokol,szfrz:podmiot,uuto,szfokol{1},szfrz{1}:dop1]%
    C := teraźniejszy,
    szfrz[0].P == mian,
    szfrz[1].P == mian

#
#
#
zdanie_to = szfokol? szfrz uuto* szfokol? zznie szfrz %FC[szfokol,szfrz:podmiot,uuto,szfokol{1},zznie,szfrz{1}:dop1]%
    C := teraźniejszy,
    szfrz[0].P == mian,
    szfrz[1].P == mian,
    setscore 1

#
zdanie_to = szfokol? szfrz 'jest' uuto* szfokol? szfrz %FC[szfokol,szfrz:podmiot,'jest',uuto,szfokol{1},szfrz{1}:dop1]%
    szfrz[0].L == 1,
    szfrz[0].O == 3,
    szfrz[0].P == mian,
    szfrz[1].L == 1,
    szfrz[1].O == 3,
    szfrz[1].P == mian,
    C := teraźniejszy

#
zdanie_to = szfokol? szfrz 'są' uuto* szfokol? szfrz %FC[szfokol,szfrz:podmiot,'są',uuto,szfokol{1},szfrz{1}:dop1]%
    szfrz[0].L == 2,
    szfrz[0].O == 3,
    szfrz[0].P == mian,
    szfrz[1].L == 2,
    szfrz[1].O == 3,
    szfrz[1].P == mian,
    C := teraźniejszy

#
#
#
zdanie_to = szfokol? szfrz uuto* szfokol? czasownik_być szfrz %FC[szfokol,szfrz:podmiot,uuto,szfokol{1},czasownik_być:zbędny,szfrz{1}:dop1]%
    C := czasownik_być.C,
    O := czasownik_być.O,
    L := czasownik_być.L,
    R := czasownik_być.R,
    O :== 3,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający),
    szfrz[0].P == mian, szfrz[1].P == mian,
    L :== szfrz[0].L,
    R :== szfrz[0].R,
    setscore 1

#
#
zdanie_to = szfokol? szfrz uuto* szfokol? zznie czasownik_być szfrz %FC[szfokol,szfrz:podmiot,uuto,szfokol{1},zznie,czasownik_być:zbędny,szfrz{1}:dop1]%
    C := czasownik_być.C,
    O := czasownik_być.O,
    L := czasownik_być.L,
    R := czasownik_być.R,
    O :== 3,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający),
    szfrz[0].P == mian, szfrz[1].P == mian,
    L :== szfrz[0].L,
    R :== szfrz[0].R,
    setscore 2


#
zdanie_to = ('oto' | 'to') szfokol? szfrz szfokol? %FC['to',szfokol,szfrz:dop1,szfokol{1}]%
    szfrz.P == mian,
    C := teraźniejszy,
    szfokol.Neg != 1

#
zdanie_to = uuto szfokol? szfprzym szfokol? %FC[uuto,szfokol,szfprzym:dop1,szfokol{1}]%
    C := teraźniejszy,
    szfprzym.R = n,
    szfprzym.L = 1

#
zdanie_to = ('oto' | 'to') szfokol? czasownik_być* szfokol? szfrz %FC['to',szfokol,czasownik_być:zbędny,szfokol{1},szfrz:dop1]%
    szfrz.P == mian,
    (C == przeszły ; C == teraźniejszy ;
     C == przyszły ; C == przypuszczający),
    szfokol.Neg != 1,
    L :== szfrz.L,
    R :== szfrz.R,
    O :== szfrz.O

#
#
zdanie_to = uuto szfokol? zznie szfrz %FC[uuto,szfokol,zznie,szfrz:dop1]%
    szfrz.P == mian,
    C := teraźniejszy,
    Neg := 1

#
#
zdanie_to = uuto szfokol? zznie czasownik_być* szfokol? szfrz %FC[uuto,szfokol,zznie,czasownik_być:zbędny,szfokol{1},szfrz:dop1]%
    szfrz.P == mian,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający),
    L :== szfrz.L,
    R :== szfrz.R,
    O :== szfrz.O,
    Neg := 1

#
zdanie_to = uktóry*
    Typ = to

#
#

#

#
#
#
#
#
#
#
zdanie_niema = szfokol? zzniema szfokol? szfrz*? fokol0? %zzniema[szfokol+,+szfokol{1},+szfrz:dop1,+fokol0]%
    C := teraźniejszy,
    (szfrz.P = false; szfrz.P == dop),
    (Sem1 := szfrz.S; Sem1 := any),
    #L := szfrz.L,
    setscore 5

#
#
#
#
#
#
zdanie_niema = szfokol? zznie czasownik_być* szfokol? szfrz? fokol0? %FC[szfokol, zznie, czasownik_być, szfokol{1}, szfrz:dop1, fokol0]%
    (C == przeszły; C == przyszły; C == przypuszczający),
    R == n,
    (szfrz.P = false; szfrz.P == dop),
    (Sem1 := szfrz.S; Sem1 := any),
    (szfrz.L = false; L:= szfrz.L),
    (szfrz.S = false; S:= szfrz.S),
    setscore 3

#
#
#
#
#
zdanie_niema = szfokol? szfrz*? szfokol? zzniema fokol0? %zzniema[szfokol+, szfrz:dop1+, szfokol{1}+, +fokol0]%
    C := teraźniejszy,
    (szfrz.P = false; szfrz.P == dop),
    (Sem1 := szfrz.S; Sem1 := any),
    #L := szfrz.L,
    setscore 5

#
#
#
#
zdanie_niema = szfokol? szfrz szfokol? zznie czasownik_być* fokol0? %FC[szfokol, szfrz:dop1, szfokol{1}, zznie, czasownik_być, fokol0]%
    (C == przeszły; C == przyszły; C == przypuszczający),
    R == n,
    (szfrz.P = false; szfrz.P == dop),
    (Sem1 := szfrz.S; Sem1 := any),
    L := szfrz.L,
    S := szfrz.S,
    setscore 3

zdanie_jest = szfokol? czasownik_być* fpartprzysł szfokol?      %FC[szfokol{0}, czasownik_być, fpartprzysł, szfokol{1}]%
    (C == teraźniejszy; C == przeszły; C == przyszły; C == przypuszczający),
    R == n, O = 3, L = 1,
        setscore 1

#
#
zdanie_jest = szfokol? zznie czasownik_być* fpartprzysł szfokol?    %FC[szfokol{0}, zznie, czasownik_być, fpartprzysł, szfokol{1}]%
    (C == teraźniejszy; C == przeszły; C == przyszły; C == przypuszczający),
    R == n, O = 3, L = 1,
        setscore 4


#

uzaimekktóry = zaimekktóry* %FR[zaimekktóry]%
    Przyim := brak

uzaimekktóry = przyimek zaimekktóry* %FPR[przyimek,FR[zaimekktóry]]%
    P :== przyimek.P,
    Przyim := przyimek.Typ

#

#
#
uzaimekktóry = 'co' ('$on'* | '$ona'* | '$ono'*)   %FR['co']%
    P != mian,
    Przyim := brak,
    Typ := co_zaimek
    , S := any

#
#
uzaimekktóry = 'co' przyimek ('$on'* | '$ona'* | '$ono'*) %FPR[przyimek,FR['co']]%
    P :== przyimek.P,
    Przyim := przyimek.Typ,
    Typ := co_zaimek
    , S := any

#

#

#
#
uzaimekktóry = uuto zzwłaśnie? zaimek_rzeczowny_osobowy*   %FR[RO[uuto],zaimek_rzeczowny_osobowy]%
    Przyim := brak,
    Typ := to

#
#
uzaimekktóry = uuto zzwłaśnie? krzwsk* %FR[RO[uuto],krzwsk]%
    Przyim := brak,
    Typ := to

#
#
uzaimekktóry = uuto zzwłaśnie? nazwa_własna %FR[RO[uuto],nazwa_własna]%
    Przyim := brak,
    Typ := to


#
#
uzaimekktóry = uuto zzwłaśnie? przyimek zaimek_rzeczowny_osobowy* %FPR[przyimek,FR[RO[uuto],zaimek_rzeczowny_osobowy]]%
    P :== przyimek.P,
    Przyim := przyimek.Typ,
    Typ := to

#
#
uzaimekktóry = uuto zzwłaśnie? przyimek krzwsk* %FPR[przyimek,FR[RO[uuto],krzwsk]]%
    P :== przyimek.P,
    Przyim := przyimek.Typ,
    Typ := to


#
#
uzaimekktóry = uuto zzwłaśnie? przyimek nazwa_własna %FPR[przyimek,FR[RO[uuto],nazwa_własna]]%
    P :== przyimek.P,
    Przyim := przyimek.Typ,
    Typ := to


#

#
#
#
#
#
uktóry = przecinek? uzaimekktóry kcz* przecinek? %kcz[przecinek{0}:prerel+,uzaimekktóry:podmiot+,+przecinek{1}]%
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający),
    (Podmiot = 1; Podmiot = oblig), Podmiot := wypełn,
    uzaimekktóry.Przyim = brak,
    uzaimekktóry.P == mian,
    L :== uzaimekktóry.L,
    R :== uzaimekktóry.R,
    O :== uzaimekktóry.O,
    Lu := L, Ru := R, Ou := O,
    Typ := uzaimekktóry.Typ,
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )

#


#
#
#
#
#
#
uktóry = przecinek? uzaimekktóry kcz* przecinek?
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający ; C == bezosobnik),
    setscore 2,
         ((Wyp1 = false,
     uzaimekktóry.P == Dop1,
     uzaimekktóry.Przyim == Przyim1,
     Su := uzaimekktóry.S ^ kcz.Sem1,
     Wyp1 := 1,
     L1 :== uzaimekktóry.L, R1 :== uzaimekktóry.R,
     setscore 3,
     %kcz[przecinek{0}:prerel+,uzaimekktóry:dop1+,+przecinek{1}]%);
      (Wyp2 = false,
     uzaimekktóry.P == Dop2,
     uzaimekktóry.Przyim == Przyim2,
     Su := uzaimekktóry.S ^ kcz.Sem2,
     Wyp2 := 1,
     L2 :== uzaimekktóry.L, R2 :== uzaimekktóry.R,
     setscore 3,
     %kcz[przecinek{0}:prerel+,uzaimekktóry:dop2+,+przecinek{1}]%);
      (Wyp3 = false,
     uzaimekktóry.P == Dop3,
     uzaimekktóry.Przyim == Przyim3,
     Su := uzaimekktóry.S ^ kcz.Sem3,
     Wyp3 := 1,
     L3 :== uzaimekktóry.L, R3 :== uzaimekktóry.R,
     setscore 3,
     %kcz[przecinek{0}:prerel+,uzaimekktóry:dop3+,+przecinek{1}]%)
          ; (uzaimekktóry.Przyim != brak, Su := uzaimekktóry.S);
      (uzaimekktóry.Przyim = brak, uzaimekktóry.P == narz, Su := '-an');
      (uzaimekktóry.Przyim = brak, uzaimekktóry.P == cel, Su := h)
    ),
       (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        ),
        Lu := uzaimekktóry.L,
        Ru := uzaimekktóry.R,
        Typ := uzaimekktóry.Typ,
        Ou := 3

#

#
pytanie = uktóry*
    (Typ = kto ; Typ = co)

#
#
pytanie = uzaimekktóry uuto szfokol? czasownik_być* szfokol? szfrz? fokol0? %FC[uzaimekktóry:dop1, uuto, szfokol{0}, czasownik_być:zbędny, szfokol{1}, szfrz:podmiot, fokol0]%
    uzaimekktóry.P = mian,
    (uzaimekktóry.Typ = kto ; uzaimekktóry.Typ = co),
    O :== 3,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający),
    szfrz.P == mian,
    L :== szfrz.L,
    R :== szfrz.R


#
#

#
pytanie = zaimek_przysłowny kcz*
    zaimek_przysłowny.Typ = pytajny,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający ; C == bezosobnik ; C == bezokolicznik),
    zaimek_przysłowny.S = loc,
    (( Wyp1 = false, Dop1 == lc, Wyp1 := 1);
       ( Wyp2 = false, Dop2 == lc, Wyp2 := 1);
       ( Wyp3 = false, Dop3 == lc, Wyp3 := 1)),
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )
    , Su := zaimek_przysłowny.S

#
pytanie = zaimek_przysłowny kcz*
    zaimek_przysłowny.Typ = pytajny,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający ; C == bezosobnik ; C == bezokolicznik),
    zaimek_przysłowny.S = manner
    , Su := zaimek_przysłowny.S

#
pytanie = zaimek_przysłowny zdanie_elem*
    zaimek_przysłowny.Typ = pytajny,
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający ; C == bezosobnik ; C == bezokolicznik),
    (
         zaimek_przysłowny.S != manner;
         (Wyp1; Dop1 != av),
     (Wyp2; Dop2 != av),
     (Wyp3; Dop3 != av)),
    (
     zaimek_przysłowny.S != loc;
         (Wyp1; Dop1 != lc),
     (Wyp2; Dop2 != lc),
     (Wyp3; Dop3 != lc)),
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )
    , Su := zaimek_przysłowny.S

#
pytanie = przysłówek kcz*
    (C == przeszły ; C == teraźniejszy ; C == przyszły ; C == przypuszczający ; C == bezosobnik ; C == bezokolicznik),
    przysłówek.Typ = interrog,
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )
    , Su := przysłówek.S

#

zaimekktórypytajny = ('@który'* | '@jaki'* | '@czyj'*)
    Typ = pytajny,
    Pytanie := 1

zaimekktórypytajny = '@ile'*
    Typ = zg,
    L := 2,
    Pytanie := 1


#
#
#
#
#

pytanie = zaimekktórypytajny zdanie_oznajm*
     (Przyim1 = brak,
     R1 != any,
     Dop1 == zaimekktórypytajny.P,
     L1 :== zaimekktórypytajny.L,
     R1 :== zaimekktórypytajny.R
     ;
     Przyim2 = brak,
     Dop2 == zaimekktórypytajny.P,
     R2 != any,
     L2 :== zaimekktórypytajny.L,
     R2 :== zaimekktórypytajny.R
     ;
     Przyim3 = brak,
     Dop3 == zaimekktórypytajny.P,
     R3 != any,
     L3 :== zaimekktórypytajny.L,
     R3 :== zaimekktórypytajny.R
     ;
     Podmiot = wypełn,
     zaimekktórypytajny.P == mian,
     R != any,
     L :== zaimekktórypytajny.L,
     R :== zaimekktórypytajny.R
        ),
    1

#
#
pytanie = przyimek zaimekktórypytajny kcz*
    OczekiwanyPrzyim = przyimek.Typ,
    OczekiwanyP == zaimekktórypytajny.P,
    OczekiwanyP == przyimek.P,
    (kcz.Przyim1 = OczekiwanyPrzyim,
     zaimekktórypytajny.R == kcz.R1, zaimekktórypytajny.L == kcz.L1;
     kcz.Przyim2 = OczekiwanyPrzyim,
     zaimekktórypytajny.R == kcz.R2, zaimekktórypytajny.L == kcz.L2;
     kcz.Przyim3 = OczekiwanyPrzyim,
     zaimekktórypytajny.R == kcz.R3, zaimekktórypytajny.L == kcz.L3),
    {OczekiwanyPrzyim := false},
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )

#
zaimekiled = '@ile'*
    Typ = d,
    L := any

zaimekiled = 'jak' '@wiele'*
    Typ = d,
    L := any

#
#
#

#
#
#

zaimekiled = 'jak' 'dużo'* %LG['dużo']%
    Typ := d,
    L := any,
    R := any


#
#

#
#
pytanie = zaimekiled kcz*
    Neg = oczekiwana,
    (Dop1 = bier,
     R1 != any,
     L1 :== zaimekiled.L,
     R1 := zaimekiled.R
     ;
     Dop2 = bier,
     R2 != any,
     L2 :== zaimekiled.L,
     R2 := zaimekiled.R
     ;
     Dop3 = bier,
     R3 != any,
     L3 :== zaimekiled.L,
     R3 := zaimekiled.R
    ),
    Neg := 0,
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )

#
pytanie = zaimekiled zdanie_oznajm*
    (Dop1 = dop,
     L1 :== zaimekiled.L,
     R1 := zaimekiled.R;
     Dop2 = dop,
     L2 :== zaimekiled.L,
     R2 := zaimekiled.R;
     Dop3 = dop,
     L3 :== zaimekiled.L,
     R3 := zaimekiled.R)

#
#
pytanie = zaimekiled katr2 kcz*
    (Podmiot = 1; Podmiot = oblig), Podmiot := wypełn,
    katr2.P == dop,
    O == 3,
    L == 1,
    R == n,
    R := katr2.R,
    L := katr2.L,
    R :== zaimekiled.R,
    L :== zaimekiled.L,
    S := katr2.S ^ kcz.S,
    (Podmiot = 1 ; Podmiot = brak; Podmiot = wypełn ; Podmiot = potencjalny),
    (Wyp1 = 1 ; Oblig1 = false),
    (Wyp2 = 1 ; Oblig2 = false),
    (Wyp3 = 1 ; Oblig3 = false),
    Neg != oczekiwana,
    OczekiwanyPrzyim = false,
    (Zwrotność = false ; Zwrotność = wypełn),
    (SobieP = false ; SobieP = wypełn),
    (Hk = 255 ; Hk = false
        )

#

możliwy_przecinek = przecinek
    Typ := 1

#
#
#
#

#
#
#
#
#
#
#
#
#

#
#
#
#
#
#

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#


#
#
#
#


#
#
#
#
#

#
#
#

#
#
#
#
#
#
#
#
#

#

zdanie_oznajm = zdanie_elem*
    (C == przeszły ; C == teraźniejszy ; C == przyszły ;
         C == przypuszczający ; C == bezosobnik)

zdaniejak = przecinek? zzjak zdanie_oznajm* %ZJAK[przecinek, zzjak,zdanie_oznajm]%
    Typ := brak,   # przy przejściu na uzdaniejak nie powstanie zbędny węzeł
    setscore 1

#
#
#
#
#

zdanieby = przecinek? spójnik zdanie_elem* %ZBY[FCs[przecinek, spójnik, zdanie_elem]]%
    spójnik.Typ = by,
    C != nieustalony,
    ( spójnik.Ps4 = 3, C = bezokolicznik ;
      spójnik.Ps4 != 3, C != bezokolicznik ),
    Typ := brak,   # przy przejściu na uzdanieby nie powstanie zbędny węzeł
    setscore 1

#
zdanieby = przecinek? spójnik szfrz kcz0*  %ZBY[FCs[przecinek, spójnik, kcz0[szfrz:podmiot+]]]%
        spójnik.Typ = by,
    (spójnik.O = 1 ; spójnik.O = 2),
    spójnik.O = szfrz.O,
    C = przeszły,
    O = 3,
    szfrz.P == mian,
    ( L :== szfrz.L,
      R :== szfrz.R;
      L :== szfrz.L2,
      R :== szfrz.R2 ),
    S := szfrz.S ^ kcz0.S,
    Typ := brak,   # przy przejściu na uzdanieby nie powstanie zbędny węzeł
    setscore 1

#
zdanieże = przecinek? spójnik zdanie_oznajm* %ZŻE[FCs[przecinek, spójnik, zdanie_oznajm]]%
    spójnik.Typ = że,
    C != nieustalony,
    Typ := brak,   # przy przejściu na uzdanieże nie powstanie zbędny węzeł
    setscore 1

#
#
#
#
#


zdanie_niż = spójnik_niż (szfrz | szfprzym) %FNIŻ[spójnik_niż, szfrz]%
    szfrz.P == mian

zdanie_niż = spójnik_niż zaimek_dzierżawczy %FNIŻ[spójnik_niż, zaimek_dzierżawczy]%
    zaimek_dzierżawczy.P == mian,
    setscore 2

zdanie_niż = spójnik_niż (szfokol | zdanie_elem) %FNIŻ[spójnik_niż, szfokol]%

zdanie_niż = spójnik_od (szfrz | szfprzym) %FNIŻ[spójnik_od, szfrz]%
    szfrz.P == dop

zdanie_niż = spójnik_od zaimek_dzierżawczy %FNIŻ[spójnik_od, zaimek_dzierżawczy]%
    zaimek_dzierżawczy.P == dop

#

zdanie_podrzędne_celu = zdanieby*

zdanie_podrzędne_celu = 'po' 'to' zdanieby* %zdanieby%

#

zdanie_elem1 = zdanie_oznajm*

zdanie_elem1 = zdanie_elem*
    (C = bezokolicznik; C = rozkazujący)

zdanie_elem1 = zdanie_elem* zdanie_podrzędne_celu
    zdanie_elem.Dop3 != by ; zdanie_elem.Wyp3

zdanie_elem1 = zdanie_podrzędne_celu przecinek zdanie_elem* %SFC[zdanie_podrzędne_celu, przecinek, zdanie_elem]%
    zdanie_elem.Dop3 != by ; zdanie_elem.Wyp3

#

#
#
#

zdanie_wtrącone = spójnik (zdanie_oznajm* | okol* | frz* | fprz0*) %FPS[FCs[spójnik, zdanie_oznajm]]%
    spójnik.Ps1 = 3,
    (  spójnik.Ps4 = 0 ;
       spójnik.Ps4 = 1,
       (zdanie_oznajm.C = przeszły ; zdanie_oznajm.C = bezosobnik; zdanie_oznajm.C = bezokolicznik);
       spójnik.Ps4 = 2, (zdanie_oznajm.C = przeszły ; zdanie_oznajm.C = bezosobnik);
       spójnik.Ps4 = 3, zdanie_oznajm.C = bezokolicznik;
       spójnik.Ps4 = 5, zdanie_oznajm.C = przyszły, setscore 2
     )

#

zdanie_szeregowe = zdanie_elem1*

#
zdanie_szeregowe = zdanie_elem1 przecinek zdanie_wtrącone przecinek zdanie_elem1

#
#
zdanie_szeregowe = zdanie_elem1 spójnik zdanie_szeregowe* %FC[SFC[zdanie_elem1, FCs[spójnik, zdanie_szeregowe]]]%
    spójnik.Ps1 = 0

zdanie_szeregowe = zdanie_elem1 przecinek zdanie_szeregowe* %FC[SFC[zdanie_elem1, przecinek, zdanie_szeregowe]]%

#

zdanie_poj = (pytanie* | zdanie_szeregowe*)

#
#
#
#
zdanie_podrzędne_warunkowe = (spójnik_warunkowy_by | spójnik) zdanie_klauzula* %FCs[spójnik_warunkowy_by, zdanie_klauzula]%
        spójnik.Ps4 = 4,
    (zdanie_klauzula.C = przeszły,
     zdanie_klauzula.O == 3,
     L :== spójnik_warunkowy_by.L,
     O := spójnik_warunkowy_by.O);
    zdanie_klauzula.C = bezokolicznik,
    spójnik_warunkowy_by.L == 1,
    spójnik_warunkowy_by.O == 3

zdanie_podrzędne_warunkowe = spójnik_jeśli zdanie_klauzula* %FCs[spójnik_jeśli, zdanie_klauzula]%
    zdanie_klauzula.C = przypuszczający

#
zdanie_warunkowe = zdanie_klauzula* przecinek? zdanie_podrzędne_warunkowe %FC[SFC[zdanie_klauzula, przecinek, zdanie_podrzędne_warunkowe]]%
    ( zdanie_klauzula.C = przypuszczający;
      zdanie_klauzula.C = przyszły)

#
#
zdanie_warunkowe = zdanie_podrzędne_warunkowe przecinek 'to' zdanie_klauzula* %FC[SFC[zdanie_podrzędne_warunkowe,  przecinek, zdanie_klauzula]]%
    (zdanie_klauzula.C = przypuszczający;
     zdanie_klauzula.C = przyszły),
    setscore 4

#
zdanie_warunkowe = zdanie_podrzędne_warunkowe przecinek zdanie_klauzula* %FC[SFC[zdanie_podrzędne_warunkowe,  przecinek, zdanie_klauzula]]%
    (zdanie_klauzula.C = przypuszczający;
     zdanie_klauzula.C = przyszły)



#

#
#
zdanie_złożone_pojedynczo = spójnik zdanie_klauzula przecinek zdanie_klauzula  %FC[SFC[FCs[spójnik, zdanie_klauzula{0}], przecinek, zdanie_klauzula{1}]]%
     spójnik.Typ != by,
     (  spójnik.Ps1 = 1, setscore 1,
        spójnik.Ps3 = 1
     ;
        spójnik.Ps1 = 2,
        spójnik.Ps2 = 1,
        spójnik.Ps3 != 4
     ),
     (zdanie_klauzula[0].C != bezokolicznik ; zdanie_klauzula[1].C != bezokolicznik),
     (zdanie_klauzula[0].C != odsłownik ; zdanie_klauzula[1].C != odsłownik),
     ( spójnik.Ps4 = 0 ;
       spójnik.Ps4 = 1,
        (zdanie_klauzula[0].C = przeszły ; zdanie_klauzula[0].C = bezosobnik; zdanie_klauzula[0].C = bezokolicznik);
       spójnik.Ps4 = 2, (zdanie_klauzula[0].C = przeszły ; zdanie_klauzula[0].C = bezosobnik);
       spójnik.Ps4 = 3, zdanie_klauzula[0].C = bezokolicznik;
       spójnik.Ps4 = 5, zdanie_klauzula[0].C = przyszły, setscore 2
     )

#
#
#
zdanie_złożone_pojedynczo = zdanie_klauzula przecinek? spójnik zdanie_klauzula %FC[SFC[zdanie_klauzula{0}, przecinek, FCs[spójnik, zdanie_klauzula{1}]]]%
     spójnik.Typ != by,
     (  spójnik.Ps1 = 1, setscore 1,
        (spójnik.Ps3 = 1; spójnik.Ps3 = 2)
     ;
        spójnik.Ps1 = 2,
        spójnik.Ps2 = 1,
        spójnik.Ps3 != 4
     ),
#
     ( zdanie_klauzula[0].C != bezokolicznik,
       zdanie_klauzula[1].C != bezokolicznik),
     ( zdanie_klauzula[0].C != odsłownik,
       zdanie_klauzula[1].C != odsłownik),
     ( spójnik.Ps4 = 0 ;
       spójnik.Ps4 = 1,
        (zdanie_klauzula[1].C = przeszły ; zdanie_klauzula[1].C = bezosobnik; zdanie_klauzula[1].C = bezokolicznik);
       spójnik.Ps4 = 2, (zdanie_klauzula[1].C = przeszły ; zdanie_klauzula[1].C = bezosobnik);
       spójnik.Ps4 = 3, zdanie_klauzula[1].C = bezokolicznik;
       spójnik.Ps4 = 5, zdanie_klauzula[1].C = przyszły, setscore 2
     )

zdanie_klauzula = (zdanie_poj* | zdanie_złożone_pojedynczo*)

#
#
#
#
#
zdanie_złożone0 = spójnik zdanie_klauzula przecinek? spójnik zdanie_klauzula* %FC[SFC[FCs[spójnik{0},                   zdanie_klauzula{0}], przecinek, FCs[spójnik{1}, zdanie_klauzula{1}]]]%
    (
      spójnik[0].Ps1 = 2, spójnik[0].Ps2 = 1
    ;
      spójnik[0].Ps1 = 1
    ),
    spójnik[0].Ps3 = 4,
    (spójnik[1].Ps1 = 8 ; spójnik[1].Ps1 = 9), setscore 4,
    Typ := spójnik[0].Typ,
    spójnik[0].Typ = spójnik[1].Typ,
    {spójnik.Neg = 1, Neg :=1},
    (zdanie_klauzula[0].C != bezokolicznik ; zdanie_klauzula[1].C != bezokolicznik),
    (zdanie_klauzula[0].C != odsłownik ; zdanie_klauzula[1].C != odsłownik)

#
#
zdanie_złożone0 = zdanie_złożone0 przecinek? spójnik zdanie_klauzula %zdanie_złożone0[FCs[przecinek, spójnik, zdanie_klauzula]]%
        spójnik.Ps1 = 9, setscore 1,
    spójnik.Typ = zdanie_złożone0.Typ

#
#
zdanie_złożone0 = spójnik frz przecinek? spójnik zdanie_klauzula* %FC[SFC[FPR[spójnik{0}, frz], przecinek, FCs[spójnik{1}, zdanie_klauzula]]]%
        spójnik[0].Ps1 = 7,
    (
        spójnik[0].Ps2 = 4, frz.P = dop
    ;
        spójnik[0].Ps2 = 5, frz.P = mian
    ;
        spójnik[0].Ps2 = 6, frz.P = bier
    ),
    spójnik[0].Ps3 = 4,
    spójnik[1].Ps1 = 8, setscore 2,
    spójnik[0].Typ = spójnik[1].Typ

zdanie_złożone = (zdanie_klauzula* | zdanie_warunkowe* | zdanie_złożone0*)

#

#
#

#


początek_zdania = fpartprzysł* przecinek?
    Typ = modulant ; Typ = init ; ( Typ = ż , setscore 2)

#
początek_zdania = wykrzyknienie* przecinek %WY[wykrzyknienie,przecinek]%
    setscore 10

koniec_zdania = '.'

#
#

#
#
zdanie_końcowe = spójnik (frz* | szfokol* | fprz0*) %FCs[spójnik, frz]%
    (spójnik.Ps1 = 5 ; spójnik.Ps1 = 6)

#
#
#

#

zdanie = zdanie* przecinek zdanie_końcowe

zdanie = zdanie_złożone*

#
#

zdanie = spójnik zdanie*  %FCs[spójnik, zdanie]%
     spójnik.Typ != by,
     (
        spójnik.Ps1 = 1,
        (spójnik.Ps3 = 1; spójnik.Ps3 = 2)
     ;
        spójnik.Ps1 = 2,
        spójnik.Ps2 = 1,
        spójnik.Ps3 != 4
     ),
     (
       spójnik.Ps4 = 1, (zdanie.C = przeszły ; zdanie.C = bezosobnik; zdanie.C = bezokolicznik);
       spójnik.Ps4 = 2, (zdanie.C = przeszły ; zdanie.C = bezosobnik);
       spójnik.Ps4 = 3, zdanie.C = bezokolicznik;
       spójnik.Ps4 = 5, zdanie.C = przyszły, setscore 2
     )

równoważnik_zdania = fpartprzysł*
    Typ = modulant

wypowiedzenie = zdanie*

wypowiedzenie = zdanie_elem*
    C = rozkazujący

wypowiedzenie = i*

#

znak_zapytania = '?'

znak_zapytania = '??'

znak_zapytania = '???'

#
#
zdanie_pytające = początek_zdania? pytanie znak_zapytania %Z[początek_zdania,pytanie,znak_zapytania]%
    Pytanie := 1

#
zdanie_pytające = początek_zdania? zdanie znak_zapytania %Z[początek_zdania,zdanie,znak_zapytania]%
    Pytanie := 1

#

pełne_zdanie = początek_zdania? wypowiedzenie koniec_zdania? %Z[początek_zdania,wypowiedzenie,koniec_zdania]%

pełne_zdanie = zdanie_pytające*

#
#
#

pełne_zdanie = przysłówek %Z[przysłówek]%
    (przysłówek.Typ = modulant ; przysłówek.Typ = ż)

pełne_zdanie = wykrzyknienie ( wykrzyknik | znak_zapytania ) %Z[wykrzyknienie,wykrzyknik]%
    setscore 10

pełne_zdanie = przysłówek przecinek szfrz ( wykrzyknik? | znak_zapytania? | koniec_zdania? ) %Z[przysłówek,przecinek,szfrz,wykrzyknik]%
    przysłówek.Typ = modulant,
    (szfrz.P == mian; szfrz.P == woł),
    setscore 4

#
#
#
pełne_zdanie = wykrzyknienie przecinek szfrz ( wykrzyknik? | znak_zapytania? | koniec_zdania? ) %Z[wykrzyknienie,przecinek,szfrz,wykrzyknik]%
    (szfrz.P == mian; szfrz.P == woł),
    setscore 4

#
#
pełne_zdanie = wykrzyknienie %Z[wykrzyknienie]%
    wykrzyknienie.Core = "tak",
        setscore 10

#
