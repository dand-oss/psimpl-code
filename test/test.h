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

#ifndef PSIMPL_TEST
#define PSIMPL_TEST


#include <string>
#include <iostream>

//! \brief initializes the root level test run
#define TEST_INIT() psimpl::test::TestRun::Init()

//! \brief defines a test run with a name and a (test)function to execute
#define TEST_RUN(name, function)                        \
    try {                                               \
        psimpl::test::TestRun run (name);               \
        function;                                       \
    }                                                   \
    catch (std::exception& e) {                         \
        psimpl::test::TestRun::Exception (e.what ());   \
    }

//! \brief disables a specific test run; replace with TEST_RUN to enable
#define TEST_DISABLED(name, function)                   \
    {                                                   \
        psimpl::test::TestRun run (name, true);         \
    }

//! \brief outputs the results of the root level test run
#define TEST_RESULT() psimpl::test::TestRun::Result()

#define PASS() // do nothing
#define FAIL(condition) psimpl::test::TestRun::Error (__FILE__, __LINE__, condition);
#define ABORT(condition) psimpl::test::TestRun::Error (__FILE__, __LINE__, condition); return;

#define VERIFY_TRUE(condition)  if (condition) { PASS() } else { FAIL(#condition) }
#define VERIFY_FALSE(condition) if (condition) { FAIL(#condition) } else { PASS() }

#define ASSERT_TRUE(condition)  if (condition) { PASS() } else { ABORT(#condition) }
#define ASSERT_FALSE(condition) if (condition) { ABORT(#condition) } else { PASS() }


namespace psimpl {
    namespace test
{
    //! \ brief represents a single, possibly nested, test run
    class TestRun
    {
    public:
        TestRun (const std::string& name, bool disabled=false);
        ~TestRun ();

        static void Error (const char* file, int line, const char* condition);
        static void Exception (const std::string& msg);
        static void Init ();
        static int Result ();

    private:
        static std::string Offset ();

    private:
        static int sDepth;          //!< nesting depth of the current executing test run
        static int sTestsPassed;    //!< total number of passed test runs
        static int sTestsFailed;    //!< total number of failed test runs
        static int sTestsDisabled;  //!< total number of disabled test runs
        static int sErrors;         //!< total number of test run errors
        static int sExceptions;     //!< total number of test run exceptions

        int mCount;                 //!< total number of test run errors/exceptions that occurred before this test run started
        bool mDisabled;             //!< indicates if this test run is disabled
    };
}}


#endif // PSIMPL_TEST
