/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is
 * 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning (edekoning@gmail.com).
 *
 * Portions created by the Initial Developer are Copyright (C) 2010-2011
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/*
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl and is hosted at SourceForge:
    http://psimpl.sf.net/, http://sf.net/projects/psimpl/
*/

#include "test.h"


namespace psimpl {
    namespace test
{
    int TestRun::sDepth = 0;
    int TestRun::sTestsPassed = 0;
    int TestRun::sTestsFailed = 0;
    int TestRun::sErrors = 0;
    int TestRun::sExceptions = 0;
    int TestRun::sTestsDisabled = 0;

    TestRun::TestRun (const std::string& name, bool disabled) :
        mCount (sErrors + sExceptions),
        mDisabled (disabled)
    {
        if (mDisabled) {
            std::cout << Offset () << "- " << name << " (disabled)" << std::endl;
        }
        else {
        std::cout << Offset () << "- " << name << std::endl;
        }
        ++sDepth;
    }
    
    TestRun::~TestRun () {
        --sDepth;

        // update counters
        if (mDisabled)
            ++sTestsDisabled;
        else if (mCount == sErrors + sExceptions)
            ++sTestsPassed;
        else
            ++sTestsFailed;
    }

    //! \brief returns a whitespace string used for log indentation
    std::string TestRun::Offset () {
        return std::string (sDepth * 2, ' ');
    }

    void TestRun::Error (const char* file, int line, const char* condition) {
        std::cout << Offset () << file << "(" << line << ") error in: " << condition << std::endl;
        ++sErrors;
    }

    void TestRun::Exception (const std::string& msg) {
        std::cout << Offset () << "unhandeled exception: " << msg << std::endl;
        ++sExceptions;
    }

    void TestRun::Init () {
        std::cout << "starting unit tests" << std::endl;
        std::cout << std::string(80, '_') << std::endl;
        sTestsPassed = sTestsFailed = sErrors = sExceptions = 0;
    }

    int TestRun::Result () {
        std::cout << std::string(80, '_') << std::endl;
        std::cout << "finished unit tests:" << std::endl;
        std::cout << "passed = " << sTestsPassed << std::endl;
        std::cout << "failed = " << sTestsFailed << std::endl;
        std::cout << "disabled = " << sTestsDisabled << std::endl;
        std::cout << "total errors = " << sErrors << std::endl;
        std::cout << "total unhandeled exceptions = " << sExceptions << std::endl;
        return sTestsFailed;
    }
}}
