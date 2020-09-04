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

#include "TestDouglasPeucker.h"
#include "helper.h"
#include "psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestDouglasPeuckerClassic::TestDouglasPeuckerClassic () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("basic sanity", TestBasicSanity ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("return value", TestReturnValue ());
        TEST_RUN("signed/unsigned integers", TestIntegers ());
    }

    // incomplete point: coord count % DIM > 1
    void TestDouglasPeuckerClassic::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestDouglasPeuckerClassic::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestDouglasPeuckerClassic::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        float tol = 0;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;
        
        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestDouglasPeuckerClassic::TestValidTol () {
        const unsigned DIM = 2;
        const unsigned count = 11;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> ());

        // tiny tol
        {
            float tol = 0.01f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(result == polyline);
        }
        // huge tol
        {
            float tol = 100.f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // normal tol
        {
            float tol = 4.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            float tol = 3.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 7*DIM);
            int keys [] = {0, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        }
    }

    void TestDouglasPeuckerClassic::TestBasicSanity () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        float tol = 0.1f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;
        
        psimpl::simplify_douglas_peucker_classic <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(result.size () == 2*DIM);
        VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
    }

    // different random access iterators, different value types, different dimensions
    void TestDouglasPeuckerClassic::TestRandomIterator () {
        const unsigned count = 11;
        {
            const unsigned DIM = 3;
            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SawToothLine <float, DIM> ());
            float result [count*DIM];
            float tol = 4.1f;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 3)));
        }
        {
            const unsigned DIM = 5;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <int, DIM> ());
            float tol = 3.1f;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            const unsigned DIM = 4;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <double, DIM> ());
            double tol = 2.1;

            psimpl::simplify_douglas_peucker_classic <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 7*DIM);
            int keys [] = {0, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        }
    }

    void TestDouglasPeuckerClassic::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline, polyline + count*DIM, 0,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline, polyline + count*DIM, 10.f,
                    result))
            == 2*DIM);
    }

    void TestDouglasPeuckerClassic::TestIntegers () {
        const unsigned DIM = 3;
        const double tol = 3;

        std::vector <double> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_douglas_peucker_classic <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_douglas_peucker_classic <DIM> (
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
            psimpl::simplify_douglas_peucker_classic <DIM> (
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
            psimpl::simplify_douglas_peucker_classic <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }    

    // --------------------------------------------------------------------------------------------

    TestDouglasPeucker::TestDouglasPeucker () {
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
    void TestDouglasPeucker::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestDouglasPeucker::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestDouglasPeucker::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        float tol = 0;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;
        
        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestDouglasPeucker::TestValidTol () {
        const unsigned DIM = 2;
        const unsigned count = 11;

        // use a step size of 10 to make sure the radial distance preprocessing
        // step won't remove any points when using a tolerance below 10
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> (10));

        // tiny tol
        {
            float tol = 0.01f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(result == polyline);
        }
        // huge tol
        {
            float tol = 100.f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // normal tol
        {
            float tol = 4.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
        {
            float tol = 3.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 6*DIM);
            int keys [] = {0, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        }
        {
            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 8*DIM);
            int keys [] = {0, 4, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 8)));
        }
    }

    void TestDouglasPeucker::TestBasicSanity () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        float tol = 0.1f;

        // use a step size of 10 to make sure the radial distance preprocessing
        // step won't remove any points when using a tolerance below 10
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> (10));
        std::vector <float> result;
        
        psimpl::simplify_douglas_peucker <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        ASSERT_TRUE(result.size () == 2*DIM);
        VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
    }

    // different random access iterators, different value types, different dimensions
    void TestDouglasPeucker::TestRandomIterator () {
        const unsigned count = 11;
        {
            const unsigned DIM = 3;
            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SawToothLine <float, DIM> (10));
            float result [count*DIM];
            float tol = 4.1f;

            psimpl::simplify_douglas_peucker <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 4)));
        }
        {
            const unsigned DIM = 5;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <int, DIM> (10));
            float tol = 3.1f;

            psimpl::simplify_douglas_peucker <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 6*DIM);
            int keys [] = {0, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        }
        {
            const unsigned DIM = 4;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <double, DIM> (10));
            double tol = 2.1;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 8*DIM);
            int keys [] = {0, 4, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 8)));
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestDouglasPeucker::TestBidirectionalIterator () {
        const unsigned count = 11;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
            std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, SawToothLine <float, DIM> (10));
            float tol = 4.1f;

            psimpl::simplify_douglas_peucker <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::inserter (result, result.begin ()));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
    }

    // forward iterator, different value types, different dimensions
    void TestDouglasPeucker::TestForwardIterator () {
        FAIL("TODO");
    }

    void TestDouglasPeucker::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker <DIM> (
                    polyline, polyline + count*DIM, 0,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker <DIM> (
                    polyline, polyline + count*DIM, 10.f,
                    result))
            == 2*DIM);
    }

    void TestDouglasPeucker::TestIntegers () {
        const unsigned DIM = 4;
        const float tol = 10;

        std::vector <float> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <float, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <float, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_douglas_peucker <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_douglas_peucker <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_douglas_peucker <DIM> (
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
            psimpl::simplify_douglas_peucker <DIM> (
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
            psimpl::simplify_douglas_peucker <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }

    // --------------------------------------------------------------------------------------------

    TestDouglasPeuckerN::TestDouglasPeuckerN () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("basic sanity", TestBasicSanity ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("return value", TestReturnValue ());
        TEST_RUN("signed/unsigned integers", TestIntegers ());
    }

    // incomplete point: coord count % DIM > 1
    void TestDouglasPeuckerN::TestIncompletePoint () {
        const unsigned DIM = 2;
        const unsigned tol = 3;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestDouglasPeuckerN::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const unsigned tol = 3;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // invalid: tol < 2
    void TestDouglasPeuckerN::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        unsigned tol = 0;

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        result.clear ();
        tol = 1;

        psimpl::simplify_douglas_peucker_n <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol >= 2
    void TestDouglasPeuckerN::TestValidTol () {
        const unsigned DIM = 2;
        const unsigned count = 11;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> ());

        // 2 points
        {
            unsigned tol = 2;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // 3 points
        {
            unsigned tol = 3;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // 4 points
        {
            unsigned tol = 4;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // count points-1
        {
            unsigned tol = count - 1;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // count points
        {
            unsigned tol = count;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(polyline == result);
        }
        // 2*count points
        {
            unsigned tol = count*2;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(polyline == result);
        }
    }

    void TestDouglasPeuckerN::TestBasicSanity () {
        const unsigned DIM = 2;
        const unsigned count = 11;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());

        // 2 points
        {
            unsigned tol = 2;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // 3 points
        {
            unsigned tol = 3;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // 4 points
        {
            unsigned tol = 4;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // count points-1
        {
            unsigned tol = count - 1;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        // count points
        {
            unsigned tol = count;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(polyline == result);
        }
        // 2*count points
        {
            unsigned tol = count*2;
            std::vector <float> result;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(polyline == result);
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestDouglasPeuckerN::TestRandomIterator () {
        const unsigned count = 11;
        {
            const unsigned DIM = 3;
            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SawToothLine <float, DIM> ());
            float result [count*DIM];
            unsigned tol = 5;

            psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + tol)));
        }
        {
            const unsigned DIM = 4;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <double, DIM> ());
            unsigned tol = 3;

            psimpl::simplify_douglas_peucker_n <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
        {
            const unsigned DIM = 5;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <int, DIM> (10));
            unsigned tol = 4;

            psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == tol*DIM);
            int keys [] = {0, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + tol)));
        }
    }

    void TestDouglasPeuckerN::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline, polyline + count*DIM, 0,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline, polyline + count*DIM, 5,
                    result))
            == 5*DIM);
    }

    void TestDouglasPeuckerN::TestIntegers () {
        const unsigned DIM = 2;
        const short tol = 50;

        std::vector <double> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_douglas_peucker_n <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_douglas_peucker_n <DIM> (
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
            psimpl::simplify_douglas_peucker_n <DIM> (
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
            psimpl::simplify_douglas_peucker_n <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }

}}
