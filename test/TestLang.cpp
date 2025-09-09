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

#include "TestLang.h"
#include "helper.h"
#include "test_helpers.h"  // Include the new test helpers
#include "psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestLang::TestLang () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid tol", TestInvalidTol ());
        TEST_RUN("valid tol", TestValidTol ());
        TEST_RUN("invalid look ahead", TestInvalidLookAhead ());
        TEST_RUN("valid look ahead", TestValidLookAhead ());
        TEST_RUN("basic sanity", TestBasicSanity ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_RUN("return value", TestReturnValue ());
        TEST_RUN("signed/unsigned integers", TestIntegers ());
    }

    // incomplete point: coord count % DIM > 1
    void TestLang::TestIncompletePoint () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        const unsigned lookAhead = 7;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestLang::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        const unsigned lookAhead = 7;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestLang::TestInvalidTol () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        const unsigned lookAhead = 7;
        float tol = 0;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestLang::TestValidTol () {
        const unsigned DIM = 2;
        const unsigned count = 9;
        const unsigned lookAhead = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        // add a spike at the 5th point
        polyline [4*DIM+1] = 2.f;
        {
            float tol = 1.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 3, 4, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            float tol = 1.9f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 4, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
    }

    // invalid: look ahead == 0
    void TestLang::TestInvalidLookAhead () {
        const unsigned DIM = 3;
        const unsigned count = 10;
        const unsigned lookAhead = 0;
        float tol = 2.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_lang <DIM> (
            polyline.begin (), polyline.end (), tol, lookAhead,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: look ahead != 0
    void TestLang::TestValidLookAhead () {
        const unsigned DIM = 2;
        const unsigned count = 7;
        const float tol = 2.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());

        // large: lookahead >= point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 7;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        {
            std::vector <float> result;
            const unsigned lookAhead = 8;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // perfect fit: lookahead + 1 == point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 6;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            VERIFY_TRUE(CompareEndPoints <DIM> (polyline.begin (), polyline.end (), result.begin (), result.end ()));
        }
        // good fit: m*look ahead + 1 == point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 3;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 3, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        // bad fit: m*look ahead + 1 != point count
        {
            std::vector <float> result;
            const unsigned lookAhead = 4;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 4, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            std::vector <float> result;
            const unsigned lookAhead = 5;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 5, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    void TestLang::TestBasicSanity () {
        const unsigned DIM = 2;
        const unsigned count = 9;
        const unsigned lookAhead = 3;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());

        //    spike at the start of the polyline
        {
            polyline [0*DIM+1] = 2.f;   // add a spike

            float tol = 0.5f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 1, 4, 7, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));

            polyline [0*DIM+1] = 0.f;   // remove the spike
        }
        {
            polyline [0*DIM+1] = 2.f;   // add a spike

            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 3, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));

            polyline [0*DIM+1] = 0.f;   // remove the spike
        }
        //    spike one after the start of the polyline
        {
            polyline [1*DIM+1] = 2.f;   // add a spike

            float tol = 0.5f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 1, 2, 5, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));

            polyline [1*DIM+1] = 0.f;   // remove the spike
        }
        {
            polyline [1*DIM+1] = 2.f;   // add a spike

            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 3, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));

            polyline [1*DIM+1] = 0.f;   // remove the spike
        }
        //    spike one before the end of the polyline
        {
            polyline [7*DIM+1] = 2.f;   // add a spike

            float tol = 0.5f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 3, 6, 7, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));

            polyline [7*DIM+1] = 0.f;   // remove the spike
        }
        {
            polyline [7*DIM+1] = 2.f;   // add a spike

            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 3, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));

            polyline [7*DIM+1] = 0.f;   // remove the spike
        }
        //    spike at the end of the polyline
        {
            polyline [8*DIM+1] = 2.f;   // add a spike

            float tol = 0.5f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 3, 6, 7, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));

            polyline [8*DIM+1] = 0.f;   // remove the spike
        }
        {
            polyline [8*DIM+1] = 2.f;   // add a spike

            float tol = 2.1f;
            std::vector <float> result;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 3, 6, 8};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));

            polyline [8*DIM+1] = 0.f;   // remove the spike
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestLang::TestRandomIterator () {
        const unsigned count = 13;
        const unsigned lookAhead = 5;
        {
            const unsigned DIM = 3;
            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SquareToothLine <float, DIM> ());
            float result [count*DIM];
            float tol = 1.1f;

            psimpl::simplify_lang <DIM> (
                    polyline, polyline + count*DIM, tol, lookAhead,
                    result);

            int keys [] = {0, 5, 9, 12};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 4)));
        }
        {
            const unsigned DIM = 4;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SquareToothLine <double, DIM> ());
            double tol = 0.5;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 7*DIM);
            int keys [] = {0, 3, 4, 6, 8, 10, 12};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        }
        {
            const unsigned DIM = 5;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SquareToothLine <int, DIM> ());
            int tol = 3;

            psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (), tol, lookAhead,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 5, 10, 12};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestLang::TestBidirectionalIterator () {
        const unsigned count = 13;
        const unsigned lookAhead = 5;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
            std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, SquareToothLine <float, DIM> ());
            float tol = 0.9f;

            psimpl::simplify_lang <DIM> (
                polyline.begin (), polyline.end (), tol, lookAhead,
                std::inserter (result, result.begin ()));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 3, 6, 9, 12};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
    }

    void TestLang::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE(
            std::distance (
                result,
                psimpl::simplify_lang <DIM> (
                    polyline, polyline + count*DIM, 0, 0,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE(
            std::distance (
                result,
                psimpl::simplify_lang <DIM> (
                    polyline, polyline + count*DIM, 10.f, 10,
                    result))
            == 2*DIM);
    }

    void TestLang::TestIntegers () {
        const unsigned DIM = 3;
        const double tol = 3;
        const int look_ahead = 7;

        std::vector <double> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_lang <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, look_ahead, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_lang <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, look_ahead, std::back_inserter (rexpected));

        // Use the helper function to test integer conversions with look_ahead parameter
        TestIntegerConversionWithRepeat<DIM>(
            polyline, expected, rexpected,
            [](auto first, auto last, auto tolerance, auto lookAhead, auto output) {
                return psimpl::simplify_lang<DIM>(
                    first, last, tolerance, lookAhead, output);
            },
            tol,
            look_ahead,
            "TestLang::TestIntegers"
        );
    }
}}
