/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.
  Copyright (C) 2013 Adam Mickiewicz University

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/


#include "numconversion.hpp"

namespace ncnv
{

enum {
    CARDINAL,
    ORDINAL,
    CARDINAL_G,
    ORDINAL_F,
    ORDINAL_NPL,
    ORDINAL_GPL,
    BEFORE_NOUN,
    CARDINAL_NPL, N_FORMS = CARDINAL_NPL,
    CARDINAL_GPL,
    CARDINAL_F, N_FORMS_1000 = CARDINAL_F,
    CARDINAL_FG,
};

enum {
    UNITS,
    TEENS,
    TENS,
    HUNDREDS,
};

enum {
    ZERO,
    JEDEN,
    DWA,
    PIEC,
};

const char* units[N_FORMS][10] =
{
    {  "zero",
       "jeden",
       "dwa",
       "trzy",
       "cztery",
       "pięć",
       "sześć",
       "siedem",
       "osiem",
       "dziewięć" },
    {  "zerowy",
       "pierwszy",
       "drugi",
       "trzeci",
       "czwarty",
       "piąty",
       "szósty",
       "siódmy",
       "ósmy",
       "dziewiąty" },
    {  "zera",
       "jeden",
       "dwóch",
       "trzech",
       "czterech",
       "pięciu",
       "sześciu",
       "siedmiu",
       "ośmiu",
       "dziewięciu" },
    {  "zerowa",
       "pierwsza",
       "druga",
       "trzecia",
       "czwarta",
       "piąta",
       "szósta",
       "siódma",
       "ósma",
       "dziewiąta" },
    {  "zerowe",
       "pierwsze",
       "drugie",
       "trzecie",
       "czwarte",
       "piąte",
       "szóste",
       "siódme",
       "ósme",
       "dziewiąte" },
    {  "zerowych",
       "pierwszych",
       "drugich",
       "trzecich",
       "czwartych",
       "piątych",
       "szóstych",
       "siódmych",
       "ósmych",
       "dziewiątych" },
    {  "zero",
       "jedno",
       "dwu",
       "trzy",
       "cztero",
       "pięcio",
       "sześcio",
       "siedmio",
       "ośmio",
       "dziewięcio" },
};

const char* teens[N_FORMS][10] =
{
    {
        "dziesięć",
        "jedenaście",
        "dwanaście",
        "trzynaście",
        "czternaście",
        "piętnaście",
        "szesnaście",
        "siedemnaście",
        "osiemnaście",
        "dziewiętnaście" },
    {
        "dziesiąty",
        "jedenasty",
        "dwunasty",
        "trzynasty",
        "czternasty",
        "piętnasty",
        "szesnasty",
        "siedemnasty",
        "osiemnasty",
        "dziewiętnasty" },
    {
        "dziesięciu",
        "jedenastu",
        "dwunastu",
        "trzynastu",
        "czternastu",
        "piętnastu",
        "szesnastu",
        "siedemnastu",
        "osiemnastu",
        "dziewiętnastu" },
    {
        "dziesiąta",
        "jedenasta",
        "dwunasta",
        "trzynasta",
        "czternasta",
        "piętnasta",
        "szesnasta",
        "siedemnasta",
        "osiemnasta",
        "dziewiętnasta" },
    {
        "dziesiąte",
        "jedenaste",
        "dwunaste",
        "trzynaste",
        "czternaste",
        "piętnaste",
        "szesnaste",
        "siedemnaste",
        "osiemnaste",
        "dziewiętnaste" },
    {
        "dziesiątych",
        "jedenastych",
        "dwunastych",
        "trzynastych",
        "czternastych",
        "piętnastych",
        "szesnastych",
        "siedemnastych",
        "osiemnastych",
        "dziewiętnastych" },
    {
        "dziesięcio",
        "jedenasto",
        "dwunasto",
        "trzynasto",
        "czternasto",
        "piętnasto",
        "szesnasto",
        "siedemnasto",
        "osiemnasto",
        "dziewiętnasto" },
};

const char* tens[N_FORMS][10] =
{
    {
        "",
        "dziesięć",
        "dwadzieścia",
        "trzydzieści",
        "czterdzieści",
        "pięćdziesiąt",
        "sześćdziesiąt",
        "siedemdziesiąt",
        "osiemdziesiąt",
        "dziewięćdziesiąt" },
    {
        "",
        "dziesiąty",
        "dwudziesty",
        "trzydziesty",
        "czterdziesty",
        "pięćdziesiąty",
        "sześćdziesiąty",
        "siedemdziesiąty",
        "osiemdziesiąty",
        "dziewięćdziesiąty" },
    {
        "",
        "dziesięciu",
        "dwudziestu",
        "trzydziestu",
        "czterdziestu",
        "pięćdziesięciu",
        "sześćdziesięciu",
        "siedemdziesięciu",
        "osiemdziesięciu",
        "dziewięćdziesięciu" },
    {
        "",
        "dziesiąta",
        "dwudziesta",
        "trzydziesta",
        "czterdziesta",
        "pięćdziesiąta",
        "sześćdziesiąta",
        "siedemdziesiąta",
        "osiemdziesiąta",
        "dziewięćdziesiąta" },
    {
        "",
        "dziesiąte",
        "dwudzieste",
        "trzydzieste",
        "czterdzieste",
        "pięćdziesiąte",
        "sześćdziesiąte",
        "siedemdziesiąte",
        "osiemdziesiąte",
        "dziewięćdziesiąte" },
    {
        "",
        "dziesiątych",
        "dwudziestych",
        "trzydziestych",
        "czterdziestych",
        "pięćdziesiątych",
        "sześćdziesiątych",
        "siedemdziesiątych",
        "osiemdziesiątych",
        "dziewięćdziesiątych" },
    {
        "",
        "dziesięcio",
        "dwudziesto",
        "trzydziesto",
        "czterdziesto",
        "pięćdziesięcio",
        "sześćdziesięcio",
        "siedemdziesięcio",
        "osiemdziesięcio",
        "dziewięćdziesięcio" },
};

const char* hundreds[N_FORMS][10] =
{
    {
        "",
        "sto",
        "dwieście",
        "trzysta",
        "czterysta",
        "pięćset",
        "sześćset",
        "siedemset",
        "osiemset",
        "dziewięćset" },
    {
        "",
        "setny",
        "dwusetny",
        "trzysetny",
        "czterysetny",
        "pięćsetny",
        "sześćsetny",
        "siedemsetny",
        "osiemsetny",
        "dziewięćsetny" },
    {
        "",
        "stu",
        "dwustu",
        "trzystu",
        "czterystu",
        "pięciuset",
        "sześciuset",
        "siedmiuset",
        "ośmiuset",
        "dziewięciuset" },
    {
        "",
        "setna",
        "dwusetna",
        "trzysetna",
        "czterysetna",
        "pięćsetna",
        "sześćsetna",
        "siedemsetna",
        "osiemsetna",
        "dziewięćsetna" },
    {
        "",
        "setne",
        "dwusetne",
        "trzysetne",
        "czterysetne",
        "pięćsetne",
        "sześćsetne",
        "siedemsetne",
        "osiemsetne",
        "dziewięćsetne" },
    {
        "",
        "setnych",
        "dwusetnych",
        "trzysetnych",
        "czterysetnych",
        "pięćsetnych",
        "sześćsetnych",
        "siedemsetnych",
        "osiemsetnych",
        "dziewięćsetnych" },
    {
        "",
        "stu",
        "dwustu",
        "trzystu",
        "czterystu",
        "pięćset",
        "sześćset",
        "siedemset",
        "osiemset",
        "dziewięćset" },
};

const char* thomils[N_FORMS_1000][20] =
{
    {
        "jeden",
        "tysiąc",
        "milion",
        "miliard",
        "bilion",
        "biliard",
        "trylion",
        "tryliard",
        "kwadrylion",
        "kwadryliard",
        "kwintylion",
        "kwintyliard",
        "sekstylion",
        "sekstyliard",
        "septylion",
        "septyliard",
        "oktylion",
        "oktyliard",
        "nonylion",
        "nonyliard" },
    {
        "pierwszy",
        "tysięczny",
        "milionowy",
        "miliardowy",
        "bilionowy",
        "biliardowy",
        "trylionowy",
        "tryliardowy",
        "kwadrylionowy",
        "kwadryliardowy",
        "kwintylionowy",
        "kwintyliardowy",
        "sekstylionowy",
        "sekstyliardowy",
        "septylionowy",
        "septyliardowy",
        "oktylionowy",
        "oktyliardowy",
        "nonylionowy",
        "nonyliardowy" },
    {
        "jednego",
        "tysiąca",
        "miliona",
        "miliarda",
        "biliona",
        "biliarda",
        "tryliona",
        "tryliarda",
        "kwadryliona",
        "kwadryliarda",
        "kwintyliona",
        "kwintyliarda",
        "sekstyliona",
        "sekstyliarda",
        "septyliona",
        "septyliarda",
        "oktyliona",
        "oktyliarda",
        "nonyliona",
        "nonyliarda" },
    {
        "",
        "tysięczna",
        "milionowa",
        "miliardowa",
        "bilionowa",
        "biliardowa",
        "trylionowa",
        "tryliardowa",
        "kwadrylionowa",
        "kwadryliardowa",
        "kwintylionowa",
        "kwintyliardowa",
        "sekstylionowa",
        "sekstyliardowa",
        "septylionowa",
        "septyliardowa",
        "oktylionowa",
        "oktyliardowa",
        "nonylionowa",
        "nonyliardowa" },
    {
        "",
        "tysięczne",
        "milionowe",
        "miliardowe",
        "bilionowe",
        "biliardowe",
        "trylionowe",
        "tryliardowe",
        "kwadrylionowe",
        "kwadryliardowe",
        "kwintylionowe",
        "kwintyliardowe",
        "sekstylionowe",
        "sekstyliardowe",
        "septylionowe",
        "septyliardowe",
        "oktylionowe",
        "oktyliardowe",
        "nonylionowe",
        "nonyliardowe" },
    {
        "",
        "tysięcznych",
        "milionowych",
        "miliardowych",
        "bilionowych",
        "biliardowych",
        "trylionowych",
        "tryliardowych",
        "kwadrylionowych",
        "kwadryliardowych",
        "kwintylionowych",
        "kwintyliardowych",
        "sekstylionowych",
        "sekstyliardowych",
        "septylionowych",
        "septyliardowych",
        "oktylionowych",
        "oktyliardowych",
        "nonylionowych",
        "nonyliardowych" },
    {
        "jedno",
        "tysiąc",
        "milion",
        "miliard",
        "bilion",
        "biliard",
        "trylion",
        "tryliard",
        "kwadrylion",
        "kwadryliard",
        "kwintylion",
        "kwintyliard",
        "sekstylion",
        "sekstyliard",
        "septylion",
        "septyliard",
        "oktylion",
        "oktyliard",
        "nonylion",
        "nonyliard" },
    {
        "",
        "tysiące",
        "miliony",
        "miliardy",
        "biliony",
        "biliardy",
        "tryliony",
        "tryliardy",
        "kwadryliony",
        "kwadryliardy",
        "kwintyliony",
        "kwintyliardy",
        "sekstyliony",
        "sekstyliardy",
        "septyliony",
        "septyliardy",
        "oktyliony",
        "oktyliardy",
        "nonyliony",
        "nonyliardy" },
    {
        "",
        "tysięcy",
        "milionów",
        "miliardów",
        "bilionów",
        "biliardów",
        "trylionów",
        "tryliardów",
        "kwadrylionów",
        "kwadryliardów",
        "kwintylionów",
        "kwintyliardów",
        "sekstylionów",
        "sekstyliardów",
        "septylionów",
        "septyliardów",
        "oktylionów",
        "oktyliardów",
        "nonylionów",
        "nonyliardów" },
};

int dix(char c)
{
    return int(c - '0');
}

void join2(const std::string& te0,
            const std::string& te1,
            const std::string& ti0,
            const std::string& ti1,
            std::string& tequiv,
            std::string& tinflection)
{
    if (te0 == "#" || te1 == "#") {
        tequiv = "#";
        tinflection = "";
        return;
    }
    tequiv = te0 + " " + te1;
    tinflection = ti0 + " " + ti1;
}

void join3(const std::string& te0,
            const std::string& te1,
            const std::string& te2,
            const std::string& ti0,
            const std::string& ti1,
            const std::string& ti2,
            std::string& tequiv,
            std::string& tinflection)
{
    std::string te01, ti01;

    join2(te0, te1, ti0, ti1, te01, ti01);
    join2(te01, te2, ti01, ti2, tequiv, tinflection);
}

void join4(const std::string& te0,
            const std::string& te1,
            const std::string& te2,
            const std::string& te3,
            const std::string& ti0,
            const std::string& ti1,
            const std::string& ti2,
            const std::string& ti3,
            std::string& tequiv,
            std::string& tinflection)
{
    std::string te012, ti012;

    join3(te0, te1, te2, ti0, ti1, ti2, te012, ti012);
    join2(te012, te3, ti012, ti3, tequiv, tinflection);
}

void prepend(std::string& s, const std::string& p, int type)
{
    if (s.length() > 0 && s[0] != ' ')
    {
        if (p.length() > 0) {
            static const char* joiner[N_FORMS] = {" ", " ", " ", " ", " ", " ", ""};

            s = p + joiner[type] + s;
        }
    }
    else
    {
        s = p;
    }
}

int bigger(const std::string& n0, const std::string& n1)
{
    size_t i0, i1, end;

    for (i0=0, end=n0.length(); i0<end; i0++) {
        if (!isdigit(n0[i0]))
            break;
    }
    for (i1=0, end=n1.length(); i1<end; i1++) {
        if (!isdigit(n1[i1]))
            break;
    }
    if (i0 > i1)
        return 1;
    if (i0 < i1)
        return 0;
    for (end=i0, i0=0; i0<end; i0++) {
        if (n0[i0] > n1[i0])
            return 1;
        if (n0[i0] < n1[i0])
            return 0;
    }
    if (i0 == n0.length() && i0 == n1.length())
        return 0;
    end = n0.length();
    if (n1.length() < end)
        end = n1.length();
    for (i0++; i0<end; i0++) {
        if (n0[i0] > n1[i0])
            return 1;
    }
    if (n0.length() > n1.length())
        return 1;
    return 0;
}

const std::string make_tinfl(int type, char digit, int rank)
{
    static const char* unitsI[2][10] = {
        {"R:5", "0", "LG:z", "LG:z", "LG:z", "LG:r", "LG:r", "LG:r", "LG:r", "LG:r"},
        {"P", "LP-P", "LP-P", "LP-P", "LP-P", "LP-P", "LP-P", "LP-P", "LP-P", "LP-P"},
    };
    static const char* icode[ORDINAL+1] = {"LG:r", "LP-P"};
    static const char* (*names[4])[10] = {units, teens, tens, hundreds};
    int d = dix(digit);

    switch (type) {
        case CARDINAL:
        case ORDINAL:
            if (rank == UNITS)
                return unitsI[type][d];
            else
                return icode[type];
        case CARDINAL_G:
            return "0";
        case ORDINAL_F:
        case ORDINAL_NPL:
        case ORDINAL_GPL:
            return std::string(names[rank][ORDINAL][d]) + "_LP-P";
        case BEFORE_NOUN:
            return "";
        default:
            return "#";
    };
}

const std::string thomil(int type, int rank)
{
    if (rank < 20)
        return thomils[type][rank];

    static const char* unduotre[10] = {
        "", "un", "duo", "tre", "kwattuor", "kwin",
        "seks", "septen", "okto", "nowem",
    };
    static const char* decvigint[11] = {
        "", "dec", "wigint", "trigint", "kwadragint", "kwinkwagint",
        "seksagint", "septuagint", "oktogint", "nowemgint", "cent",
    };
    static const char* illionard[N_FORMS_1000][2] = {
        { "ylion", "yliard" },
        { "ylionowy", "yliardowy" },
        { "yliona", "yliarda" },
        { "ylionowa", "yliardowa" },
        { "ylionowe", "yliardowe" },
        { "ylionowych", "yliardowych" },
        { "ylion", "yliard" },
        { "yliony", "yliardy" },
        { "ylionów", "yliardów" },
    };

    return std::string(unduotre[rank/2%10])
        + decvigint[rank/2/10]
        + illionard[type][rank%2];
}

void make_large_unit(int type, int rank,
                            std::string& tequiv,
                            std::string& tinflection)
{
    static const char* unit_tinfl[N_FORMS] =
        { "R:3", "LP-P", "0", "", "", "", "" };

    if (rank < 220) {
        tequiv = thomil(type, rank);
        if (type == ORDINAL_F || type == ORDINAL_NPL || type == ORDINAL_GPL)
            tinflection = thomil(ORDINAL, rank) + "_LP-P";
        else if (type == CARDINAL_NPL || type == CARDINAL_GPL)
            tinflection = thomil(CARDINAL, rank) + "_R:3";
        else
            tinflection = unit_tinfl[type];
    } else {
        tequiv = "#";
        tinflection = "";
    }
}

int equals(const std::string& n, char digit)
{
    int i = n.length()-1;

    if (n[i] != digit)
        return 0;
    for (i--; i>=0; i--) {
        if (n[i] != '0')
            return 0;
    }
    return 1;
}

void convert_three_digit_group(int type,
                               const std::string& n,
                               std::string& tequiv,
                               std::string& tinflection)
{
    int i = n.length()-1;

    tequiv = tinflection = "";

    if (i == -1) return;
    if (i > 2 || !isdigit(n[i])) { tequiv = "#"; return;}

    tequiv = units[type][dix(n[i])];
    tinflection = make_tinfl(type, n[i], UNITS);

    if (i > 0)
    {
        if (n[i-1] == '1')
        {
            tequiv = teens[type][dix(n[i])];
            tinflection = make_tinfl(type, n[i], TEENS);
        }
        else if (n[i] == '0')
        {
            tequiv = "";
            tinflection = "";
        }

        --i;

        if (n[i] != '0' && n[i] != '1')
        {
            prepend(tequiv, tens[type][dix(n[i])], type);
            prepend(tinflection, make_tinfl(type, n[i], TENS), type);
        }
        if (i > 0)
        {
            --i;
            if (n[i] == '0') {
                if (type == BEFORE_NOUN)
                    tinflection = "0";
                return;
            }

            static const int lonehundred[N_FORMS] =
                { CARDINAL, CARDINAL, CARDINAL_G,
                  CARDINAL, CARDINAL, CARDINAL, BEFORE_NOUN };

            int nt = tequiv.length()>0 ? lonehundred[type] : type;
            prepend(tequiv, hundreds[nt][dix(n[i])], nt);
            if (nt == CARDINAL && type != CARDINAL)
                prepend(tinflection, "0", nt);
            else
                prepend(tinflection, make_tinfl(nt, n[i], HUNDREDS), nt);
        }
    }
    if (type == BEFORE_NOUN)
        tinflection = "0";
}

int select_government(std::string n)
{
    int i = n.length()-1;

    if (i == -1) return -1;
    if (i > 2 || !isdigit(n[i])) return -1;
    switch (n[i]) {
        case '0':
            if ((i>0 && n[i-1]!='0')
            ||  (i>1 && n[i-2]!='0'))
                return PIEC;
            else
                return ZERO;
        case '1':
            if ((i>0 && n[i-1]!='0')
            ||  (i>1 && n[i-2]!='0'))
                return PIEC;
            else
                return JEDEN;
        case '2':
        case '3':
        case '4':
            if (i>0 && n[i-1]=='1')
                return PIEC;
            else
                return DWA;
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return PIEC;
        default:
            return -1;
    }
}

void convert_cardinal_part(const std::string& n,
                std::string& tequiv,
                std::string& tinflection,
                int rank)
{
    std::string subteq, subtinfl;
    convert_three_digit_group(CARDINAL, n, tequiv, tinflection);
    switch (select_government(n)) {
        case ZERO:
            tequiv = "";
            tinflection = "";
            return;
        case JEDEN:
            make_large_unit(CARDINAL, rank, tequiv, tinflection);
            return;
        case DWA:
            make_large_unit(CARDINAL_NPL, rank, subteq, subtinfl);
            tequiv += " " + subteq;
            tinflection += " " + subtinfl;
            return;
        case PIEC:
            make_large_unit(CARDINAL_GPL, rank, subteq, subtinfl);
            tequiv += " " + subteq;
            tinflection += " " + subtinfl;
            return;
        default:
            tequiv = "#";
            tinflection = "";
            return;
    }
}

void convert_noncardinal_part(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection,
                int rank)
{
    static const int high_type[N_FORMS] =
        { 0, BEFORE_NOUN, CARDINAL_G, BEFORE_NOUN, 0 };
    std::string subteq, subtinfl;

    convert_three_digit_group(high_type[type], n, tequiv, tinflection);
    switch (select_government(n)) {
        case ZERO:
            tequiv = "";
            tinflection = "";
            return;
        case JEDEN:
            make_large_unit(type, rank, tequiv, tinflection);
            return;
        case DWA:
        case PIEC:
            if (type == CARDINAL_G) {
                make_large_unit(CARDINAL_GPL, rank, subteq, subtinfl);
                tequiv += " " + subteq;
                tinflection += " 0";
            } else {
                make_large_unit(type, rank, subteq, tinflection);
                tequiv += subteq;
            }
            return;
        default:
            tequiv = "#";
            tinflection = "";
            return;
    }
}

void convert_part(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection,
                int rank)
{
    if (rank == 0) {
        convert_three_digit_group(type, n, tequiv, tinflection);
    } else {
        switch (type) {
            case CARDINAL:
            case BEFORE_NOUN:
                convert_cardinal_part(n, tequiv, tinflection, rank);
                break;
            case ORDINAL:
            case ORDINAL_F:
            case ORDINAL_NPL:
            case ORDINAL_GPL:
            case CARDINAL_G:
                convert_noncardinal_part(type, n, tequiv, tinflection, rank);
                break;
            default:
                tequiv = "#";
                tinflection = "";
                break;
        }
    }
}

int equals_one_half(std::string s)
{
    int end = s.length();

    if (end == 0)
        return 0;
    if (s[0] != '5')
        return 0;
    for (int ii=1; ii<end; ii++)
        if (s[ii] != '0')
            return 0;
    return 1;
}

void integer2pltext(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection)
{
    static const char* zero[N_FORMS_1000+2][2] = {
        { "zero", "R:5" },              // CARDINAL
        { "zerowy", "P" },              // ORDINAL
        { "zera", "0" },                // CARDINAL_G
        { "zerowa", "zerowy_P" },       // ORDINAL_F
        { "zerowe", "zerowy_P" },       // ORDINAL_NPL
        { "zerowych", "zerowy_P" },     // ORDINAL_GPL
        { "zero", "0" },                // BEFORE_NOUN
        { "", "" },             // CARDINAL_NPL
        { "", "" },             // CARDINAL_GPL
        { "zero", "R:5" },              // CARDINAL_F
        { "zera", "0" },                // CARDINAL_FG
    };
    static const char* jeden[N_FORMS_1000+2][2] = {
        { "jeden", "ZP:z" },             // CARDINAL
        { "pierwszy", "LP-P" },          // ORDINAL
        { "jednego", "0" },              // CARDINAL_G
        { "pierwsza", "pierwszy_LP" },   // ORDINAL_F
        { "pierwsze", "pierwszy_LP" },   // ORDINAL_NPL
        { "pierwszych", "pierwszy_LP" }, // ORDINAL_GPL
        { "jedno", "0" },                // BEFORE_NOUN
        { "", "" },                      // CARDINAL_NPL
        { "", "" },                      // CARDINAL_GPL
        { "jedna", "ZP:z" },             // CARDINAL_F
        { "jednej", "ZP:z" },            // CARDINAL_FG
    };
    static const int higher_units[N_FORMS_1000+2] = {
        CARDINAL, CARDINAL, CARDINAL_G,
        CARDINAL, CARDINAL, CARDINAL, CARDINAL,
        0, 0, CARDINAL, CARDINAL_G,
    };

    for (int ii=0, end=n.length(); ii<end; ii++) {
        if (!isdigit(n[ii])) {
            tequiv = "#";
            tinflection = "";
            return;
        }
    }

    if (equals(n, '0')) {
        tequiv = zero[type][0];
        tinflection = zero[type][1];
        return;
    }
    if (equals(n, '1')) {
        tequiv = jeden[type][0];
        tinflection = jeden[type][1];
        return;
    }

    int nt = type;
    if (type == CARDINAL_F)
        nt = CARDINAL;
    else if (type == CARDINAL_FG)
        nt = CARDINAL_G;
    std::string subn, subteq, subtinfl;
    int i, rank;

    tequiv = "";
    tinflection = "";

    for (i=n.length()-3, rank=0; i>0; i-=3, rank++) {
        subn = n.substr(i, 3);
        convert_part(nt, subn, subteq, subtinfl, rank);
        if (subteq.length() > 0)
            nt = higher_units[nt];
        prepend(tequiv, subteq, CARDINAL);
        prepend(tinflection, subtinfl, CARDINAL);
    }
    subn = n.substr(0, i+3);
    convert_part(nt, subn, subteq, subtinfl, rank);
    prepend(tequiv, subteq, CARDINAL);
    prepend(tinflection, subtinfl, CARDINAL);

    if (type == CARDINAL_F
    &&  tequiv.length() >= 3
    &&  tequiv.substr(tequiv.length()-3, 3) == "dwa") {
        tequiv = tequiv.substr(0, tequiv.length()-3) + "dwie";
        tinflection = tinflection.substr(0, tinflection.length()-4)
            + "dwa_LG:z";
    }
}

void fractional2pltext(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection)
{
    static const char* denominator[] = {"", "dziesiąt", "setn", "tysięczn"};
    std::string te0, ti0, te1, ti1;

    if (type == CARDINAL)
        integer2pltext(CARDINAL_F, n, te0, ti0);
    else if (type == CARDINAL_G)
        integer2pltext(CARDINAL_FG, n, te0, ti0);
    else
        assert(0);
    te1 = denominator[n.length()];
    ti1 = te1 + "y_LP";

    switch (select_government(n)) {
        case JEDEN:
            te1 += (type == CARDINAL) ? "a" : "ej";
            break;
        case DWA:
            te1 += (type == CARDINAL) ? "e" : "ych";
            break;
        case ZERO:
        case PIEC:
            te1 += "ych";
            break;
        default:
            te1 = "#";
            break;
    }
    join2(te0, te1, ti0, ti1, tequiv, tinflection);
}

void decimalfraction2pltext(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection)
{
    size_t pos = n.find(', ');

    if (pos == std::string::npos) {
        integer2pltext(type, n, tequiv, tinflection);
        return;
    }
    std::string n0, n1, te0, te1, ti0, ti1;
    n0 = n.substr(0, pos);
    n1 = n.substr(pos+1);
    integer2pltext(type, n0, te0, ti0);
    if (equals_one_half(n1)) {
        if (type == BEFORE_NOUN) {
            if (te0 != "#")
                tequiv = te0 + "ipół";
        } else {
            join2(te0, "i pół", ti0, "0 0", tequiv, tinflection);
        }
        return;
    }
    if (type != CARDINAL && type != CARDINAL_G)
        return;
    if (n1.length() <= 3) {
        fractional2pltext(type, n1, te1, ti1);
        join3(te0, "i", te1, ti0, "0", ti1, tequiv, tinflection);
    } else {
        integer2pltext(CARDINAL, n1, te1, ti1);
        int i;
        for (i=0; n1[i]=='0'; i++) {
            prepend(te1, "zero", CARDINAL);
            prepend(ti1, "0", CARDINAL);
        }
        join3(te0, "przecinek", te1, ti0, "0", ti1, tequiv, tinflection);
    }
}

void num2pltext(int type,
                const std::string& n,
                std::string& tequiv,
                std::string& tinflection,
                std::string& tsyntax)
{
    size_t pos;

    tsyntax = "";
    tequiv = "#";
    tinflection = "";
    if (n.length() == 0)
        return;
    if (n[0] == '-') {
        num2pltext(type, n.substr(1), tequiv, tinflection, tsyntax);
        prepend(tequiv, "minus", CARDINAL);
        prepend(tinflection, "0", CARDINAL);
        return;
    }
    if (n[0] == '+') {
        num2pltext(type, n.substr(1), tequiv, tinflection, tsyntax);
        prepend(tequiv, "plus", CARDINAL);
        prepend(tinflection, "0", CARDINAL);
        return;
    }
    if (n[n.length()-1] == '%') {
        if (type != CARDINAL)
            return;
        num2pltext(type, n.substr(0, n.length()-1), tequiv, tinflection, tsyntax);
        tequiv += " procent";
        tinflection += " 0";
        return;
    }
    pos = n.find('+');
    if (pos != std::string::npos) {
        if (type != CARDINAL)
            return;
        std::string n0 = n.substr(0, pos);
        std::string n1 = n.substr(pos+1);
        std::string te0, te1, ti0, ti1;
        decimalfraction2pltext(type, n0, te0, ti0);
        decimalfraction2pltext(type, n1, te1, ti1);
        join3(te0, "plus", te1, ti0, "0", ti1, tequiv, tinflection);
        return;
    }
    pos = n.find('-');
    if (pos != std::string::npos) {
        if (type != CARDINAL)
            return;
        std::string n0 = n.substr(0, pos);
        std::string n1 = n.substr(pos+1);
        std::string te0, te1, ti0, ti1;
        if (bigger(n0, n1)) {
            decimalfraction2pltext(type, n0, te0, ti0);
            decimalfraction2pltext(type, n1, te1, ti1);
            join3(te0, "minus", te1, ti0, "0", ti1, tequiv, tinflection);
        } else {
            decimalfraction2pltext(CARDINAL_G, n0, te0, ti0);
            decimalfraction2pltext(CARDINAL_G, n1, te1, ti1);
            join4("od", te0, "do", te1, "0", ti0, "0", ti1, tequiv, tinflection);
        }
        return;
    }
    pos = n.find('/');
    if (pos != std::string::npos) {
        if (type != CARDINAL)
            return;
        std::string n0 = n.substr(0, pos);
        std::string n1 = n.substr(pos+1);
        std::string te0, te1, ti0, ti1;
        integer2pltext(CARDINAL_F, n0, te0, ti0);
        switch (select_government(n0)) {
        case JEDEN:
            integer2pltext(ORDINAL_F, n1, te1, ti1);
            break;
        case DWA:
            integer2pltext(ORDINAL_NPL, n1, te1, ti1);
            break;
        case ZERO:
        case PIEC:
            integer2pltext(ORDINAL_GPL, n1, te1, ti1);
            break;
        default:
            return;
        }
        join2(te0, te1, ti0, ti1, tequiv, tinflection);
        tsyntax = "R=4";
        return;
    }
    decimalfraction2pltext(type, n, tequiv, tinflection);
}


}
