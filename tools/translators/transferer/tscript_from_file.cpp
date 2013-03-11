/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "tscript_from_file.hpp"

#include <iostream>
#include <fstream>
#define ERROR_MELDER(x)  std::cerr << x ;

#include "trparser.tab.hpp"

using namespace tmil;

std::istream* trlex_input_stream; //for flex
extern int trlexlineno; //flex
extern char* trlextext; //flex
extern tmil::TScript trparser_all; //bison
extern char trerror_message[]; //bison
int trparserparse(); //bison call
zsymbol* the_file_name; //for bison
zsymbolfactory* the_zsymbolfactory; //for bison
zobjects_holder* the_holder; //for bison

FileParsingScriptFactory::FileParsingScriptFactory():ScriptFactory() {}

FileParsingScriptFactory::~FileParsingScriptFactory() {}

TScript FileParsingScriptFactory::getScriptForName(const char* file_name,
						   zsymbolfactory* a_zsymbolfactory,
						   zobjects_holder* a_holder)
{
//    static poleng::t5::Mutex ymutex;

    assert(NULL!=file_name);
    assert(NULL!=a_zsymbolfactory);

    std::ifstream ifile(file_name, std::ios::binary);
    if(!ifile) {
        ERROR_MELDER("\n[tmil:FileParsingScriptFactory] Could not open the script file: " << file_name << "\n");
        return NULL;
    }

    TScript result = NULL;
    //setup
    //ymutex.enter();
    the_file_name = a_zsymbolfactory->get_symbol(file_name);
    the_zsymbolfactory = a_zsymbolfactory;
    trlex_input_stream = &ifile;
    the_holder = a_holder;

    if(trparserparse()) {
        std::cerr << "!!trules parse::" << file_name << "::" << trerror_message << '\n';
	exit(1);
    } else {
        result = trparser_all;
    }
    //cleanup
    trlex_input_stream = NULL;
    the_file_name = NULL;
    the_zsymbolfactory = NULL;
    the_holder = EMPTY_ZOBJECTS_HOLDER;
    //ymutex.leave();

    return result;
}
