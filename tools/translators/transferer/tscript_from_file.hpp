#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "tscript_factory.hpp"

namespace tmil {

/**
 * A factory for TScript structures.
 * It is encapsulating a bison parser.
 */
class FileParsingScriptFactory: public ScriptFactory {
public:
    FileParsingScriptFactory();
    ~FileParsingScriptFactory();

    /**
     * Gets the script from a file <code>file_name</code>.
     *
     * @return   A valid TScript or NULL in case of errors.
     */
    virtual TScript getScriptForName(const char* file_name,
                     zsymbolfactory* a_zsymbolfactory,
                     zobjects_holder* a_holder);
};

}; //namespace tmil
