#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "trules.hpp"

namespace tmil {

/**
 * An interface of a factory for TScript structures.
 */
class ScriptFactory {
public:
    /**
     * Gets the script named <code>name</code>, where <code>name</code>
     * should be a file name.
     * You should not keep any reference/pointer to the
     * <code>a_zsymbolfactory</code> object. It is likely that it will become
     * invalid after this metod completes. Use the factory only! for the
     * current (<code>name</code>) script.
     *
     * @param  name              The name of the script.
     * @param  a_zsymbolfactory  A factory for creating zsymbols.
     *
     * @return   A valid TScript or NULL if there is no such script or
     *           there were errors instanting (fe. parsing) the script
     */
    virtual TScript getScriptForName(const char* name,
                                     zsymbolfactory* a_zsymbolfactory,
                                     zobjects_holder* a_holder) = 0;

    virtual ~ScriptFactory() {}
};

}; //namespace tmil
