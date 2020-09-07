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

#include "TestUtil.h"
#include "test.h"
#include "psimpl.h"

#include <list>
#include <set>
#include <iterator>
#include <typeinfo>


namespace psimpl {
    namespace test
{
    TestUtil::TestUtil () {
        TEST_RUN("scoped_array", TestScopedArray ());
        TEST_RUN("copy_key", TestCopyKey ());
        TEST_RUN("copy_keys", TestCopyKeys ());
        TEST_RUN("forward", TestForward ());
        TEST_RUN("backward", TestBackward ());
        TEST_RUN("select_calculation_type", TestSelectCalculationType ());
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestScopedArray () {
        // construction
        psimpl::util::scoped_array <float> a1 (5);
        ASSERT_TRUE(a1.get ());

        // setting / getting values
        a1 [0] = 0.f; VERIFY_TRUE(a1 [0] == 0.f);
        a1 [1] = 1.f; VERIFY_TRUE(a1 [1] == 1.f);
        a1 [2] = 2.f; VERIFY_TRUE(a1 [2] == 2.f);
        a1 [3] = 3.f; VERIFY_TRUE(a1 [3] == 3.f);
        a1 [4] = 4.f; VERIFY_TRUE(a1 [4] == 4.f);

        // const access
        const psimpl::util::scoped_array <float>& a2 = a1;
        VERIFY_TRUE(a2 [0] == 0.f);

        // swap
        psimpl::util::scoped_array <float> a3 (2);
        a3 [0] = 321.f;
        a3 [1] = 654.f;

        a1.swap (a3);
        ASSERT_TRUE(a1.get ());
        ASSERT_TRUE(a3.get ());
        VERIFY_TRUE(a1 [0] == 321.f);
        VERIFY_TRUE(a1 [1] == 654.f);
        VERIFY_TRUE(a3 [0] == 0.f);
        VERIFY_TRUE(a3 [1] == 1.f);
        VERIFY_TRUE(a3 [2] == 2.f);
        VERIFY_TRUE(a3 [3] == 3.f);
        VERIFY_TRUE(a3 [4] == 4.f);

        psimpl::util::swap (a1, a3);
        ASSERT_TRUE(a1.get ());
        ASSERT_TRUE(a3.get ());
        VERIFY_TRUE(a1 [0] == 0.f);
        VERIFY_TRUE(a1 [1] == 1.f);
        VERIFY_TRUE(a1 [2] == 2.f);
        VERIFY_TRUE(a1 [3] == 3.f);
        VERIFY_TRUE(a1 [4] == 4.f);
        VERIFY_TRUE(a3 [0] == 321.f);
        VERIFY_TRUE(a3 [1] == 654.f);
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestCopyKey () {
        const float vec [10] = { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        float res [5];
        auto* it = res;
        psimpl::util::copy_key <5> (vec, it);
        ASSERT_TRUE(5 == std::distance (res, it));
        VERIFY_TRUE(res [0] == vec [0]);
        VERIFY_TRUE(res [1] == vec [1]);
        VERIFY_TRUE(res [2] == vec [2]);
        VERIFY_TRUE(res [3] == vec [3]);
        VERIFY_TRUE(res [4] == vec [4]);
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestCopyKeys () {
        const short coords [10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector <bool> keys (5, false);
        std::deque <short> res;
        auto&& BSr = std::back_inserter (res);
        psimpl::util::copy_keys <2> (coords, coords + 8, keys.begin (), BSr);
        VERIFY_TRUE(res.empty ());

        keys.clear ();
        keys.push_back (true);
        keys.push_back (false);
        keys.push_back (true);
        keys.push_back (true);
        psimpl::util::copy_keys <2> (coords, coords + 8, keys.begin (), BSr);
        VERIFY_TRUE(3 * 2 == res.size ());
        VERIFY_TRUE(0 == res [0]);
        VERIFY_TRUE(1 == res [1]);
        VERIFY_TRUE(4 == res [2]);
        VERIFY_TRUE(5 == res [3]);
        VERIFY_TRUE(6 == res [4]);
        VERIFY_TRUE(7 == res [5]);
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestForward () {
        std::vector <double> values;
        values.push_back (0.0);
        values.push_back (1.0);
        values.push_back (2.0);
        values.push_back (3.0);
        values.push_back (4.0);
        values.push_back (5.0);

        std::vector <double>::iterator it = values.begin ();
        int remaining = 3;
        VERIFY_TRUE(2 == psimpl::util::forward <2> (it, 2, remaining));
        VERIFY_TRUE(1 == remaining);
        VERIFY_TRUE(2 * 2 == std::distance (values.begin (), it));

        VERIFY_TRUE(1 == psimpl::util::forward <2> (it, 2, remaining));
        VERIFY_TRUE(0 == remaining);
        VERIFY_TRUE(3 * 2 == std::distance (values.begin (), it));

        VERIFY_TRUE(0 == psimpl::util::forward <2> (it, 2, remaining));
        VERIFY_TRUE(0 == remaining);
        VERIFY_TRUE(3 * 2 == std::distance (values.begin (), it));
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestBackward() {
        std::list <float> values;
        values.push_back (0.0);
        values.push_back (1.0);
        values.push_back (2.0);
        values.push_back (3.0);
        values.push_back (4.0);
        values.push_back (5.0);

        std::list <float>::iterator it = values.begin ();
        std::advance (it, 3);
        int remaining = 2;

        psimpl::util::backward <2> (it, 1, remaining);
        VERIFY_TRUE(2 + 1 == remaining);
        VERIFY_TRUE(1.0 == *it);
    }

    // ---------------------------------------------------------------------------------------------

    void TestUtil::TestSelectCalculationType ()
    {
        // floating point types are not promoted
        VERIFY_TRUE(typeid (float) == typeid (psimpl::util::select_calculation_type <std::deque <float>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::set <double>::iterator>::type));
        // integer types are promoted
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::list <char>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::vector <unsigned char>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::deque <signed char>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::list <short>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::vector <unsigned short>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::deque <signed short>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::set <int>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::list <unsigned int>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::vector <signed int>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::set <long>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::list <unsigned long>::iterator>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <std::vector <signed long>::iterator>::type));
        // cv qualifiers are removed before promotion
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <const char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <volatile char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <const volatile char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <signed char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <signed const char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <signed volatile char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <signed const volatile char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <unsigned char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <unsigned const char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <unsigned volatile char*>::type));
        VERIFY_TRUE(typeid (double) == typeid (psimpl::util::select_calculation_type <unsigned const volatile char*>::type));
        // custom types are not promoted
        typedef std::pair <std::vector <int>, char***> CustomType;
        VERIFY_TRUE(typeid (CustomType) == typeid (psimpl::util::select_calculation_type <std::set <CustomType>::iterator>::type));
    }

}}

