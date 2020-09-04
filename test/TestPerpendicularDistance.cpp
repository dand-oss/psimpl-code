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

#include "TestPerpendicularDistance.h"
#include "helper.h"
#include "psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestPerpendicularDistance::TestPerpendicularDistance () {
        TEST_RUN("single pass | incomplete point", TestIncompletePoint_sp ());
        TEST_RUN("single pass | not enough points", TestNotEnoughPoints_sp ());
        TEST_RUN("single pass | invalid tol", TestInvalidTol_sp ());
        TEST_RUN("single pass | valid tol", TestValidTol_sp ());
        TEST_RUN("single pass | basic sanity", TestBasicSanity ());
        TEST_RUN("single pass | random iterator", TestRandomIterator_sp ());
        TEST_RUN("single pass | bidirectional iterator", TestBidirectionalIterator_sp ());
        TEST_DISABLED("single pass | forward iterator", TestForwardIterator_sp ());
        TEST_RUN("single pass | return value", TestReturnValue_sp ());
        TEST_RUN("single pass | signed/unsigned integers", TestIntegers_sp ());

        TEST_RUN("multi pass | incomplete point", TestIncompletePoint_mp ());
        TEST_RUN("multi pass | not enough points", TestNotEnoughPoints_mp ());
        TEST_RUN("multi pass | invalid tol", TestInvalidTol_mp ());
        TEST_RUN("multi pass | valid tol", TestValidTol_mp ());
        TEST_RUN("multi pass | invalid repeat", TestInvalidRepeat_mp ());
        TEST_RUN("multi pass | valid repeat", TestValidRepeat_mp ());
        TEST_RUN("multi pass | return value", TestReturnValue_mp ());
        TEST_RUN("multi pass | signed/unsigned integers", TestIntegers_mp ());
    }

    // incomplete point: coord count % DIM > 1
    void TestPerpendicularDistance::TestIncompletePoint_sp () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 4th point complete
        polyline.push_back (4.f);
        result.clear ();

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_FALSE(polyline == result);
    }

    // not enough points: point count < 3
    void TestPerpendicularDistance::TestNotEnoughPoints_sp () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 0 points
        std::vector <float> polyline;
        std::vector <float> result;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 1 point
        polyline.push_back(1.f);
        polyline.push_back(1.f);
        result.clear ();

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);

        // 2 points
        polyline.push_back(2.f);
        polyline.push_back(2.f);
        result.clear ();

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // invalid: tol == 0
    void TestPerpendicularDistance::TestInvalidTol_sp () {
        const unsigned DIM = 3;
        const unsigned count = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        float tol = 0;
        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol,
            std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    // valid: tol != 0
    void TestPerpendicularDistance::TestValidTol_sp () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> ());
        {
            float tol = 0.5f;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            VERIFY_TRUE(polyline == result);
        }
        {
            float tol = 2.5f;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 9*DIM);
            int keys [] = {0, 2, 4, 5, 6, 7, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 9)));
        }
        {
            float tol = 4.5f;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 7*DIM);
            int keys [] = {0, 2, 4, 6, 8, 9, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        }
        {
            float tol = 6.f;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 6*DIM);
            int keys [] = {0, 2, 4, 6, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        }
    }

    void TestPerpendicularDistance::TestBasicSanity () {
        const unsigned DIM = 2;
        const float tol = 2.f;
        {
            // even num points
            const unsigned count = 6;
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 2, 4, 5};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
        {
            // odd num points
            const unsigned count = 5;
            std::vector <float> polyline;
            std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 2, 4};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    void TestPerpendicularDistance::TestRandomIterator_sp () {
        const unsigned count = 7;
        {
            const unsigned DIM = 2;

            float polyline [count*DIM];
            std::generate_n (polyline, count*DIM, SawToothLine <float, DIM> ());
            float result [count*DIM];
            float tol = 2.5f;

            psimpl::simplify_perpendicular_distance <DIM> (
                    polyline, polyline + count*DIM, tol,
                    result);

            int keys [] = {0, 2, 4, 5, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline, result, std::vector <int> (keys, keys + 5)));
        }
        {
            const unsigned DIM = 3;
            std::vector <double> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <double, DIM> ());
            double tol = 2.5;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 5*DIM);
            int keys [] = {0, 2, 4, 5, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 5)));
        }
        {
            const unsigned DIM = 4;
            std::deque <int> polyline, result;
            std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <int, DIM> ());
            int tol = 2;

            psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.begin (), polyline.end (), tol,
                    std::back_inserter (result));

            ASSERT_TRUE(result.size () == 6*DIM);
            int keys [] = {0, 2, 3, 4, 5, 6};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        }
    }

    void TestPerpendicularDistance::TestBidirectionalIterator_sp () {
        const unsigned count = 10;
        {
            const unsigned DIM = 2;
            std::list <float> polyline, result;
            std::generate_n (std::inserter (polyline, polyline.begin ()), count*DIM, SawToothLine <float, DIM> ());
            float tol = 3.5f;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol,
                std::inserter (result, result.begin ()));

            ASSERT_TRUE(result.size () == 7*DIM);
            int keys [] = {0, 2, 4, 6, 7, 8, 9};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 7)));
        }
    }

    void TestPerpendicularDistance::TestForwardIterator_sp () {
        FAIL("TODO");
    }

    void TestPerpendicularDistance::TestReturnValue_sp () {
        const unsigned DIM = 3;
        const unsigned count = 11;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_perpendicular_distance <DIM> (
                    polyline, polyline + count*DIM, 0.f,
                    result))
            == count*DIM);

        // valid input
        VERIFY_TRUE(
            std::distance (
                result, 
                psimpl::simplify_perpendicular_distance <DIM> (
                    polyline, polyline + count*DIM, 10.f,
                    result))
            == (count/2+1)*DIM);
    }

    void TestPerpendicularDistance::TestIntegers_sp () {
        const unsigned DIM = 2;
        const double tol = 5;

        std::vector <double> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <double, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_perpendicular_distance <DIM> (
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
            psimpl::simplify_perpendicular_distance <DIM> (
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
            psimpl::simplify_perpendicular_distance <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }

    void TestPerpendicularDistance::TestIncompletePoint_mp () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        // 4th point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM-1, StraightLine <float, DIM> ());
        std::vector <float> result;

        unsigned repeat = 1;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 2;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 3;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);
    }

    void TestPerpendicularDistance::TestNotEnoughPoints_mp () {
        const unsigned DIM = 2;
        const float tol = 2.f;

        std::vector <float> polyline;
        std::vector <float> result;

        unsigned repeat = 1;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 2;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 3;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);
    }

    void TestPerpendicularDistance::TestInvalidTol_mp () {
        const unsigned DIM = 3;
        const unsigned count = 10;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        float tol = 0;
        unsigned repeat = 1;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 2;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);

        result.clear ();
        repeat = 3;

        psimpl::simplify_perpendicular_distance <DIM> (
            polyline.begin (), polyline.end (), tol, repeat,
            std::back_inserter (result));
        VERIFY_TRUE(polyline == result);
    }

    void TestPerpendicularDistance::TestValidTol_mp () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        const float tol = 3.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        {
            unsigned repeat = 3;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
        {
            unsigned repeat = 333;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 2*DIM);
            int keys [] = {0, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 2)));
        }
    }

    void TestPerpendicularDistance::TestInvalidRepeat_mp () {
        const unsigned count = 11;
        const unsigned DIM = 2;
        const unsigned repeat = 0;

        std::vector <float> polyline, result;
        std::generate_n (std::back_inserter (polyline), count*DIM, SawToothLine <float, DIM> ());
        float tol = 2.5f;

        psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

        VERIFY_TRUE(polyline == result);
    }

    void TestPerpendicularDistance::TestValidRepeat_mp () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        const float tol = 3.f;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), count*DIM, StraightLine <float, DIM> ());
        {
            unsigned repeat = 1;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 6*DIM);
            int keys [] = {0, 2, 4, 6, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 6)));
        }
        {
            unsigned repeat = 2;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 4*DIM);
            int keys [] = {0, 4, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 4)));
        }
        {
            unsigned repeat = 3;
            std::vector <float> result;

            psimpl::simplify_perpendicular_distance <DIM> (
                polyline.begin (), polyline.end (), tol, repeat,
                std::back_inserter (result));

            ASSERT_TRUE(result.size () == 3*DIM);
            int keys [] = {0, 8, 10};
            VERIFY_TRUE(ComparePoints <DIM> (polyline.begin (), result.begin (), std::vector <int> (keys, keys + 3)));
        }
    }

    void TestPerpendicularDistance::TestReturnValue_mp () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        const float invalidTol = 0.f;
        const float validTol = 10.f;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        {
            unsigned repeat = 0;
            // invalid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, invalidTol, repeat,
                        result))
                == count*DIM);

            // valid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, validTol, repeat,
                        result))
                == count*DIM);
        }
        {
            unsigned repeat = 1;
            // invalid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, invalidTol, repeat,
                        result))
                == count*DIM);

            // valid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, validTol, repeat,
                        result))
                == (count/2+1)*DIM);
        }
        {
            unsigned repeat = 2;
            // invalid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, invalidTol, repeat,
                        result))
                == count*DIM);

            // valid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, validTol, repeat,
                        result))
                == ((count/2+1)/2+1)*DIM);
        }
        {
            unsigned repeat = 3;
            // invalid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, invalidTol, repeat,
                        result))
                == count*DIM);

            // valid input
            VERIFY_TRUE(
                std::distance (
                    result, 
                    psimpl::simplify_perpendicular_distance <DIM> (
                        polyline, polyline + count*DIM, validTol, repeat,
                        result))
                == (((count/2+1)/2+1)/2+1)*DIM);
        }
    }

    void TestPerpendicularDistance::TestIntegers_mp () {
        const unsigned DIM = 2;
        const double tol = 3;
        const short repeat = 5;

        std::vector <float> polyline, expected, rexpected;
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <float, DIM> ());
        std::generate_n (std::back_inserter (polyline), 100*DIM, SquareToothLine <float, DIM> ());
        // simplify -> result = expected
        psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.begin (), polyline.end (),
                    tol, repeat, std::back_inserter (expected));
        // simplify reverse -> result = rexpected
        psimpl::simplify_perpendicular_distance <DIM> (
                    polyline.rbegin (), polyline.rend (),
                    tol, repeat, std::back_inserter (rexpected));
        {
            // integers
            std::vector <int> intPolyline, intResult, intExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (intPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (intExpected));
            // simplify -> result should match expected
            psimpl::simplify_perpendicular_distance <DIM> (
                        intPolyline.begin (), intPolyline.end (),
                        tol, repeat, std::back_inserter (intResult));
            VERIFY_TRUE(intResult == intExpected);
        }
        {
            // unsigned integers
            std::vector <unsigned> uintPolyline, uintResult, uintExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (uintPolyline));
            std::copy (expected.begin (), expected.end (), std::back_inserter (uintExpected));
            // simplify -> result should match expected
            psimpl::simplify_perpendicular_distance <DIM> (
                        uintPolyline.begin (), uintPolyline.end (),
                        tol, repeat, std::back_inserter (uintResult));
            VERIFY_TRUE(uintResult == uintExpected);
        }
        {
            // unsigned integers (reverse)
            std::vector <unsigned> uintPolyline, ruintResult, ruintExpected;
            std::copy (polyline.begin (), polyline.end (), std::back_inserter (uintPolyline));
            std::copy (rexpected.begin (), rexpected.end (), std::back_inserter (ruintExpected));
            // simplify reverse -> result should match rexpected
            psimpl::simplify_perpendicular_distance <DIM> (
                        uintPolyline.rbegin (), uintPolyline.rend (),
                        tol, repeat, std::back_inserter (ruintResult));
            VERIFY_TRUE(ruintResult == ruintExpected);
        }
    }

}}
