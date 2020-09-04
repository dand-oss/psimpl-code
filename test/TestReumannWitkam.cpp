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

#include "TestReumannWitkam.h"
#include "helper.h"
#include "psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestReumannWitkam::TestReumannWitkam () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("basic sanity", TestBasicSanity ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
        TEST_RUN("return value", TestReturnValue ());
        TEST_RUN("signed/unsigned integers", TestIntegers ());
    }
    
    // incomplete point: coord count % DIM > 1
    void TestReumannWitkam::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }
    
    // not enough points: point count < 3
    void TestReumannWitkam::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
        
        // 3 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline != result);
    }
    
    // invalid: tol == 0
    void TestReumannWitkam::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        float tol = 0;
        psimpl::simplify_reumann_witkam <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestReumannWitkam::TestValidTol () {
        const unsigned DIM = 3;
        const unsigned count = 25;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SquareToothLine <float, DIM> ());

        // tiny tol, only collinear points removed
        {
            std::vector <float> result;
            float tol = 0.01f;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 13*DIM);
            int keys [] = {0, 1, 2, 3, 4, 6, 8, 10, 12, 15, 18, 21, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 13)));
        }
        // large tol, removes all internal points
        {
            std::vector <float> result;
            float tol = 100.f;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // normal tol, 1 x stepsize
        {
            std::vector <float> result;
            float tol = 1.1f;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 9*DIM);
            int keys [] = {0, 7, 9, 11, 13, 16, 19, 22, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 9)));
        }
        // normal tol, 2 x stepsize
        {
            std::vector <float> result;
            float tol = 2.1f;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 17, 20, 23, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
    }

    void TestReumannWitkam::TestBasicSanity () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        {
            // even num points
            const unsigned count = 6;
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;

            psimpl::simplify_reumann_witkam <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        {
            // odd num points
            const unsigned count = 5;
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;

            psimpl::simplify_reumann_witkam <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestReumannWitkam::TestRandomIterator () {
        const unsigned count = 25;
        {
            const unsigned DIM = 2;

            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SquareToothLine <float, DIM> ());
            float result [count*DIM];
            float tol = 2.5f;

            psimpl::simplify_reumann_witkam <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 17, 20, 23, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 5)));
        }
        {
            const unsigned DIM = 3;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SquareToothLine <double, DIM> ());
            double tol = 2.5;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 17, 20, 23, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            const unsigned DIM = 4;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SquareToothLine <int, DIM> ());
            int tol = 3;

            psimpl::simplify_reumann_witkam <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 17, 20, 23, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestReumannWitkam::TestBidirectionalIterator () {
        const unsigned count = 25;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
            std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, SquareToothLine <float, DIM> ());
            float tol = 2.5f;

            psimpl::simplify_reumann_witkam <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::inserter (result, result.begin ()));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 17, 20, 23, 24};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
    }

    // forward iterator, different value types, different dimensions
    void TestReumannWitkam::TestForwardIterator () {
        FAIL("TODO");
    }

    void TestReumannWitkam::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 15;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::simplify_reumann_witkam <DIM> (
                    polyline, polyline + count*DIM, 0.f,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::simplify_reumann_witkam <DIM> (
                    polyline, polyline + count*DIM, 1.5f,
                    result))
            == 2*DIM);
    }

    void TestReumannWitkam::TestIntegers () {
        const unsigned DIM = 2;
        const float tol = 4;

        std::vector <double> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_reumann_witkam <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_reumann_witkam <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_reumann_witkam <DIM> (
                        intPolyline.begin (), intPolyline.end (),
                        tol, std::back_inserter (intResult));
            VERIFY_TRUE(intResult == intExpected);
        }
        {
            // unsigned integers
            std::vector <unsigned> uintPolyline, uintResult, uintExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (uintPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (uintExpected));
            // simplify -> result should match expected
            psimpl::simplify_reumann_witkam <DIM> (
                        uintPolyline.begin (), uintPolyline.end (),
                        tol, std::back_inserter (uintResult));
            VERIFY_TRUE(uintResult == uintExpected);
        }
        {
            // unsigned integers (reverse)
            std::vector <unsigned> uintPolyline, ruintResult, ruintExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (uintPolyline));
            std::copy (rexpected.begin (), rexpected.end (), std::back_inserter (ruintExpected));
            // simplify reverse -> result should match rexpected
            psimpl::simplify_reumann_witkam <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }

}}
