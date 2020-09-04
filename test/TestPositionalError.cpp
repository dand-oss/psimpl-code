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

#include "TestPositionalError.h"
#include "helper.h"
#include "psimpl.h"
#include <numeric>
#include <iterator>
#include <vector>
#include <deque>
#include <list>


namespace psimpl {
    namespace test
{
    TestPositionalError::TestPositionalError () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid lines", TestInvalidLines ());
        TEST_RUN("valid lines", TestValidLines ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
        TEST_RUN("mixed iterators", TestMixedIterators ());
        TEST_RUN("return value", TestReturnValue ());
    }

    // incomplete point: coord count % DIM > 1
    void TestPositionalError::TestIncompletePoint () {
        const unsigned DIM = 2;
        bool valid = false;

        // 4th polyline point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        polyline.pop_back ();
        std::vector <float> result;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // 4th simplification point incomplete
        polyline = simplification;
        simplification.pop_back ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);
    }

    // not enough points: point count < 3
    void TestPositionalError::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        bool valid = false;

        // 0 polyline points, 4 simplification points
        std::vector <float> polyline;
        std::vector <float> simplification;
        std::generate_n (std::back_inserter (simplification), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> result;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // 1 polyline points, 4 simplification points
        std::generate_n (std::back_inserter (polyline), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // 4 polyline points, 0 simplification points
        polyline = simplification;
        simplification.clear ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // 4 polyline points, 1 simplification points
        std::generate_n (std::back_inserter (simplification), 1*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // polyline points < simplification points
        simplification.clear ();
        std::generate_n (std::back_inserter (simplification), 10*DIM, StraightLine <float, DIM> ());

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);
    }

    // invalid: tol == 0
    void TestPositionalError::TestInvalidLines () {
        const unsigned DIM = 2;
        bool valid = false;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        std::vector <float> result;

        // first points don't match
        simplification [0] += 10.f;

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);

        // last points don't match
        simplification = polyline;
        simplification [simplification.size () - 1] += 10.f;
        result.clear ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        // can only be detected at the end of the algo, thus result will contain data
        VERIFY_FALSE(result.empty ());
        VERIFY_FALSE(valid);

        // simplification contains more points than polyline
        simplification = polyline;
        simplification.push_back (999);
        simplification.push_back (999);
        result.clear ();

        psimpl::compute_positional_errors2 <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            std::back_inserter (result), &valid);

        VERIFY_TRUE(result.empty ());
        VERIFY_FALSE(valid);
    }

    // valid: tol != 0
    void TestPositionalError::TestValidLines () {
        const unsigned DIM = 2;
        bool valid = false;
        double polyline [14] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5, 6, 3, 7, 0};

        // all points match
        {
            double simplification [14] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5, 6, 3, 7, 0};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 14,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(14/DIM == result.size ());
            VERIFY_TRUE(0.0 == std::accumulate (result.begin (), result.end (), 0.0));
        }
        // end points and one internal point match
        {
            double simplification [6] = {0, 0, 3, 4, 7, 0};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 6,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(14/DIM == result.size ());
            VERIFY_TRUE(0.0 == result [0]);
            VERIFY_TRUE(0.0 != result [1]);
            VERIFY_TRUE(0.0 != result [2]);
            VERIFY_TRUE(0.0 == result [3]);
            VERIFY_TRUE(0.0 != result [4]);
            VERIFY_TRUE(0.0 != result [5]);
            VERIFY_TRUE(0.0 == result [6]);
        }
        // only end points match
        {
            double simplification [4] = {0, 0, 7, 0};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 4,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(14/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(1.0  == result [2]);
            VERIFY_TRUE(16.0 == result [3]);
            VERIFY_TRUE(25.0 == result [4]);
            VERIFY_TRUE(9.0  == result [5]);
            VERIFY_TRUE(0.0  == result [6]);
        }
        // all points match, but polyline contains additional points
        {
            double simplification [10] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 10,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0 == std::accumulate (result.begin (), result.end (), 0.0));
        }
        // end points and one internal point match, but polyline contains additional points
        {
            double simplification [6] = {0, 0, 2, 1, 4, 5};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 6,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0 == result [0]);
            VERIFY_TRUE(0.0 != result [1]);
            VERIFY_TRUE(0.0 == result [2]);
            VERIFY_TRUE(0.0 != result [3]);
            VERIFY_TRUE(0.0 == result [4]);
        }
        // only end points match, but polyline contains additional points
        {
            double simplification [4] = {0, 0, 4, 5};
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 4,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(0.0 != result [1]);
            VERIFY_TRUE(0.0 != result [2]);
            VERIFY_TRUE(0.0 != result [3]);
            VERIFY_TRUE(0.0 == result [4]);
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestPositionalError::TestRandomIterator () {
        const unsigned DIM = 2;
        bool valid = false;
        double polyline [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
        double simplification [6] = {0, 0, 2, 0, 7, 0};
        {
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                polyline, polyline + 10,
                simplification, simplification + 6,
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
        {
            std::vector <double> poly (polyline, polyline + 10);
            std::vector <double> simpl (simplification, simplification + 6);
            std::vector <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
        {
            std::deque <double> poly (polyline, polyline + 10);
            std::deque <double> simpl (simplification, simplification + 6);
            std::deque <double> result;

            psimpl::compute_positional_errors2 <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestPositionalError::TestBidirectionalIterator () {
        const unsigned DIM = 2;
        bool valid = false;
        float polyline [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
        float simplification [6] = {0, 0, 2, 0, 7, 0};
        {
            std::list <float> poly (polyline, polyline + 10);
            std::list <float> simpl (simplification, simplification + 6);
            std::vector <float> result;

            psimpl::compute_positional_errors2 <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                std::back_inserter (result), &valid);

            VERIFY_TRUE(valid);
            ASSERT_TRUE(10/DIM == result.size ());
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
    }

    // forward iterator, different value types, different dimensions
    void TestPositionalError::TestForwardIterator () {
        FAIL("TODO");
    }

    // different iterators and different value types
    void TestPositionalError::TestMixedIterators () {
        {
            const unsigned DIM = 2;
            bool valid = false;
            float p [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
            int s [6] = {0, 0, 2, 0, 7, 0};
            std::vector <float> poly (p, p + 10);
            std::list <int> simpl (s, s + 6);
            double result [10/DIM];

            psimpl::compute_positional_errors2 <DIM> (
                    poly.begin (), poly.end (),
                    simpl.begin (), simpl.end (),
                    result, &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
        {
            const unsigned DIM = 2;
            bool valid = false;
            int p [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
            float s [6] = {0, 0, 2, 0, 7, 0};
            std::vector <int> poly (p, p + 10);
            std::list <float> simpl (s, s + 6);
            double result [10/DIM];

            psimpl::compute_positional_errors2 <DIM> (
                    poly.begin (), poly.end (),
                    simpl.begin (), simpl.end (),
                    result, &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0  == result [0]);
            VERIFY_TRUE(4.0  == result [1]);
            VERIFY_TRUE(0.0  == result [2]);
            VERIFY_TRUE(25.0 == result [3]);
            VERIFY_TRUE(0.0  == result [4]);
        }
    }

    void TestPositionalError::TestReturnValue () {
        const unsigned DIM = 3;
        const unsigned count = 11;
        bool valid = false;

        float polyline [count*DIM];
        std::generate_n (polyline, count*DIM, StraightLine <float, DIM> ());
        float simplification [count*DIM];
        std::generate_n (simplification, count*DIM, StraightLine <float, DIM> ());
        float result [count*DIM];

        // invalid input
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification,
                    result, &valid))
            == 0);
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification,
                    result, NULL))
            == 0);
        VERIFY_FALSE(valid);

        // valid input
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification + count*DIM,
                    result, &valid))
            == count);
        VERIFY_TRUE (
            std::distance (
                result, 
                psimpl::compute_positional_errors2 <DIM> (
                    polyline, polyline + count*DIM,
                    simplification, simplification + count*DIM,
                    result, NULL))
            == count);
        VERIFY_TRUE(valid);
    }

    // --------------------------------------------------------------------------------------------

    TestPositionalErrorStatistics::TestPositionalErrorStatistics () {
        TEST_RUN("incomplete point", TestIncompletePoint ());
        TEST_RUN("not enough points", TestNotEnoughPoints ());
        TEST_RUN("invalid lines", TestInvalidLines ());
        TEST_RUN("valid lines", TestValidLines ());
        TEST_RUN("random iterator", TestRandomIterator ());
        TEST_RUN("bidirectional iterator", TestBidirectionalIterator ());
        TEST_DISABLED("forward iterator", TestForwardIterator ());
        TEST_RUN("mixed iterators", TestMixedIterators ());
    }

    // incomplete point: coord count % DIM > 1
    void TestPositionalErrorStatistics::TestIncompletePoint () {
        const unsigned DIM = 2;
        bool valid = false;

        // 4th polyline point incomplete
        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        polyline.pop_back ();
        error::statistics stats;

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (), &valid);

        VERIFY_FALSE(valid);

        // 4th simplification point incomplete
        polyline = simplification;
        simplification.pop_back ();

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);
    }

    // not enough points: point count < 3
    void TestPositionalErrorStatistics::TestNotEnoughPoints () {
        const unsigned DIM = 2;
        bool valid = false;

        // 0 polyline points, 4 simplification points
        std::vector <float> polyline;
        std::vector <float> simplification;
        std::generate_n (std::back_inserter (simplification), 4*DIM, StraightLine <float, DIM> ());
        error::statistics stats;

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);

        // 1 polyline points, 4 simplification points
        std::generate_n (std::back_inserter (polyline), 1*DIM, StraightLine <float, DIM> ());

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);

        // 4 polyline points, 0 simplification points
        polyline = simplification;
        simplification.clear ();

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);

        // 4 polyline points, 1 simplification points
        std::generate_n (std::back_inserter (simplification), 1*DIM, StraightLine <float, DIM> ());

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);

        // polyline points < simplification points
        simplification.clear ();
        std::generate_n (std::back_inserter (simplification), 10*DIM, StraightLine <float, DIM> ());

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);
    }

    // invalid: tol == 0
    void TestPositionalErrorStatistics::TestInvalidLines () {
        const unsigned DIM = 2;
        bool valid = false;

        std::vector <float> polyline;
        std::generate_n (std::back_inserter (polyline), 4*DIM, StraightLine <float, DIM> ());
        std::vector <float> simplification = polyline;
        error::statistics stats;

        // first points don't match
        simplification [0] += 10.f;

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);

        // last points don't match
        simplification = polyline;
        simplification [simplification.size () - 1] += 10.f;

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        // can only be detected at the end of the algo, thus result will contain data
        VERIFY_FALSE(valid);

        // simplification contains more points than polyline
        simplification = polyline;
        simplification.push_back (999);
        simplification.push_back (999);

        stats = psimpl::compute_positional_error_statistics <DIM> (
            polyline.begin (), polyline.end (),
            simplification.begin (), simplification.end (),
            &valid);

        VERIFY_FALSE(valid);
    }

    // valid: tol != 0
    void TestPositionalErrorStatistics::TestValidLines () {
        const unsigned DIM = 2;
        bool valid = false;
        double polyline [14] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5, 6, 3, 7, 0};
        error::statistics stats;

        // all points match
        {
            double simplification [14] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5, 6, 3, 7, 0};
            std::vector <double> result;

            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 14,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0 == stats.max);
            VERIFY_TRUE(0.0 == stats.mean);
            VERIFY_TRUE(0.0 == stats.std);
            VERIFY_TRUE(0.0 == stats.sum);
        }
        // end points and one internal point match
        {
            double simplification [6] = {0, 0, 3, 4, 7, 0};

            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 6,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0 != stats.max);
            VERIFY_TRUE(0.0 != stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
            VERIFY_TRUE(0.0 != stats.sum);
        }
        // only end points match
        {
            double simplification [4] = {0, 0, 7, 0};

            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 4,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0  == stats.max);
            VERIFY_TRUE(15.0  == stats.sum);
            VERIFY_TRUE(15.0/7.0  == stats.mean);
            VERIFY_TRUE(0.0  != stats.std);
        }
        // all points match, but polyline contains additional points
        {
            double simplification [10] = {0, 0, 1, 2, 2, 1, 3, 4, 4, 5};

            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 10,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0 == stats.max);
            VERIFY_TRUE(0.0 == stats.mean);
            VERIFY_TRUE(0.0 == stats.std);
            VERIFY_TRUE(0.0 == stats.sum);
        }
        // end points and one internal point match, but polyline contains additional points
        {
            double simplification [6] = {0, 0, 2, 1, 4, 5};
            
            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 6,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0 != stats.max);
            VERIFY_TRUE(0.0 != stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
            VERIFY_TRUE(0.0 != stats.sum);
        }
        // only end points match, but polyline contains additional points
        {
            double simplification [4] = {0, 0, 4, 5};
            
            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 14,
                simplification, simplification + 4,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(0.0 != stats.max);
            VERIFY_TRUE(0.0 != stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
            VERIFY_TRUE(0.0 != stats.sum);
        }
    }

    // different random access iterators, different value types, different dimensions
    void TestPositionalErrorStatistics::TestRandomIterator () {
        const unsigned DIM = 2;
        bool valid = false;
        double polyline [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
        double simplification [6] = {0, 0, 2, 0, 7, 0};
        error::statistics stats;
        {
            stats = psimpl::compute_positional_error_statistics <DIM> (
                polyline, polyline + 10,
                simplification, simplification + 6,
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
        }
        {
            std::vector <double> poly (polyline, polyline + 10);
            std::vector <double> simpl (simplification, simplification + 6);

            stats = psimpl::compute_positional_error_statistics <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
        }
        {
            std::deque <double> poly (polyline, polyline + 10);
            std::deque <double> simpl (simplification, simplification + 6);

            stats = psimpl::compute_positional_error_statistics <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
        }
    }

    // bidirectional iterator, different value types, different dimensions
    void TestPositionalErrorStatistics::TestBidirectionalIterator () {
        const unsigned DIM = 2;
        bool valid = false;
        float polyline [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
        float simplification [6] = {0, 0, 2, 0, 7, 0};
        error::statistics stats;
        {
            std::list <float> poly (polyline, polyline + 10);
            std::list <float> simpl (simplification, simplification + 6);

            stats = psimpl::compute_positional_error_statistics <DIM> (
                poly.begin (), poly.end (),
                simpl.begin (), simpl.end (),
                &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
        }
    }

    // forward iterator, different value types, different dimensions
    void TestPositionalErrorStatistics::TestForwardIterator () {
        FAIL("TODO");
    }

    void TestPositionalErrorStatistics::TestMixedIterators () {
        error::statistics stats;
        {
            const unsigned DIM = 2;
            bool valid = false;
            float p [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
            int s [6] = {0, 0, 2, 0, 7, 0};
            std::vector <float> poly (p, p + 10);
            std::list <int> simpl (s, s + 6);

            stats = psimpl::compute_positional_error_statistics <DIM> (
                    poly.begin (), poly.end (),
                    simpl.begin (), simpl.end (),
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
    }
        {
            const unsigned DIM = 2;
            bool valid = false;
            int p [10] = {0, 0, 1, 2, 2, 0, 4, 5, 7, 0};
            float s [6] = {0, 0, 2, 0, 7, 0};
            std::vector <int> poly (p, p + 10);
            std::list <float> simpl (s, s + 6);

            stats = psimpl::compute_positional_error_statistics <DIM> (
                    poly.begin (), poly.end (),
                    simpl.begin (), simpl.end (),
                    &valid);

            VERIFY_TRUE(valid);
            VERIFY_TRUE(5.0 == stats.max);
            VERIFY_TRUE(7.0 == stats.sum);
            VERIFY_TRUE(7.0/5.0 == stats.mean);
            VERIFY_TRUE(0.0 != stats.std);
        }
    }

}}
