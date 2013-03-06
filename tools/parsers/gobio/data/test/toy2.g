# Toy grammar for testing Gobio rules

x = x* y
        (x != kcz0; C != odsłownik),
    {Neg = false, Neg := oczekiwana},
    (y.P == dop ; y.P == bier),
    (( Wyp1 = false,
      Dop1 == bier, Przyim1 = brak,
      Sem1 := y.S ^ x.Sem1,
      Wyp1 := 1,
      (y.O != 3 ; L1 :== y.L, R1 :== y.R),
      %x[+y:dop1]%);
    ( Wyp2 = false,
      Dop2 == bier, Przyim2 = brak,
      Sem2 := y.S ^ x.Sem2,
      Wyp2 := 1,
      (y.O != 3 ; L2 :== y.L, R2 :== y.R),
      %x[+y:dop2]%);
    ( Wyp3 = false,
      Dop3 == bier, Przyim3 = brak,
      Sem3 := y.S ^ x.Sem3,
      Wyp3 := 1,
      (y.O != 3 ; L3 :== y.L, R3 :== y.R),
      %x[+y:dop3]%)),
    setscore 2,
    {y.Wyp1 = 1, setscore 1},
    {y.Wyp2 = 1, setscore 1}

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
