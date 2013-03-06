#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.
  Copyright (C) 2013 Adam Mickiewicz University

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

/* Konwersja liczby zapisanej cyframi do polskiego napisu.
   Czesc modulu do normalizacji tekstu
*/

#include <string>
#include <cstdio>
#include <cctype>
#include <cassert>

namespace ncnv
{
    enum {
        CARD = 0,
        ORD = 1,
        PREF = 6,
    };

    /* Dla liczby n zwraca slowna reprezentacje w jezyku polskim (tequiv),
       opis sposobu odmieniania przez rodzaj i przypadek (tinflection)
       oraz, gdy to konieczne, rodzaj gramatyczny (tsyntax) */
    void num2pltext(int type,
                    const std::string& n,
                    std::string& tequiv,
                    std::string& tinflection,
                    std::string& tsyntax);
}
