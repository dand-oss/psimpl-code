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

#include "TestMath.h"
#include "test.h"
#include "helper.h"
#include "../lib/psimpl.h"
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <limits>


namespace psimpl {
    namespace test
{

    TestMath::TestMath () {
        TEST_RUN("equal | random iterator", TestEqual_RandomIterator ());
        TEST_RUN("equal | bidirectional iterator", TestEqual_BidirectionalIterator ());
        TEST_DISABLED("equal | forward iterator", TestEqual_ForwardIterator ());
        TEST_RUN("equal | mixed iterators", TestEqual_MixedIterators ());

        TEST_RUN("add | random iterator", TestAdd_RandomIterator ());
        TEST_RUN("add | bidirectional iterator", TestAdd_BidirectionalIterator ());
        TEST_DISABLED("add | forward iterator", TestAdd_ForwardIterator ());

        TEST_RUN("subtract | random iterator", TestSubtract_RandomIterator ());
        TEST_RUN("subtract | bidirectional iterator", TestSubtract_BidirectionalIterator ());
        TEST_DISABLED("subtract | forward iterator", TestSubtract_ForwardIterator ());
        TEST_RUN("subtract | mixed iterators", TestSubtract_MixedIterators ());
        TEST_RUN("subtract | unsigned", TestSubtract_Unsigned ());

        TEST_RUN("multiply", TestMultiply ());

        TEST_RUN("dot", TestDot ());
        TEST_RUN("dot", TestDot_Unsigned ());

        TEST_RUN("point_distance2", TestPointDistance ());
        TEST_RUN("point_distance2 | random iterator", TestPointDistance_RandomIterator ());
        TEST_RUN("point_distance2 | bidirectional iterator", TestPointDistance_BidirectionalIterator ());
        TEST_DISABLED("point_distance2 forward iterator", TestPointDistance_ForwardIterator ());
        TEST_RUN("point_distance2 | mixed iterators", TestPointDistance_MixedIterators ());
        TEST_RUN("point_distance2 | unisgned", TestPointDistance_Unsigned ());

        TEST_RUN("line_distance2", TestLineDistance ());
        TEST_RUN("line_distance2 | random iterator", TestLineDistance_RandomIterator ());
        TEST_RUN("line_distance2 | bidirectional iterator", TestLineDistance_BidirectionalIterator ());
        TEST_DISABLED("line_distance2 | forward iterator", TestLineDistance_ForwardIterator ());
        TEST_RUN("line_distance2 | unsigned", TestLineDistance_Unisgned ());

        TEST_RUN("segment_distance2", TestSegmentDistance ());
        TEST_RUN("segment_distance2 | random iterator", TestSegmentDistance_RandomIterator ());
        TEST_RUN("segment_distance2 | bidirectional iterator", TestSegmentDistance_BidirectionalIterator ());
        TEST_DISABLED("segment_distance2 | forward iterator", TestSegmentDistance_ForwardIterator ());
        TEST_RUN("segment_distance2 | mixed iterators", TestSegmentDistance_MixedIterators ());
        TEST_RUN("segment_distance2 | unsigned", TestSegmentDistance_Unsigned ());

        TEST_RUN("ray_distance2", TestRayDistance ());
        TEST_RUN("ray_distance2 | random iterator", TestRayDistance_RandomIterator ());
        TEST_RUN("ray_distance2 | bidirectional iterator", TestRayDistance_BidirectionalIterator ());
        TEST_DISABLED("ray_distance2 | forward iterator", TestRayDistance_ForwardIterator ());
        TEST_RUN("ray_distance2 | unsigned", TestRayDistance_Unsigned ());
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestEqual_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {1, 2};
            double p3 [dim] = {2, 3};
            VERIFY_TRUE(psimpl::math::equal <dim> (p1, p2));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1, p3));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            std::vector <float> p1 (v, v + dim);
            std::vector <float> p2 (v, v + dim);
            std::vector <float> p3 (w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {2, 3, 4, 5};
            std::deque <int> p1 (v, v + dim);
            std::deque <int> p2 (v, v + dim);
            std::deque <int> p3 (w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
    }

    void TestMath::TestEqual_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float w [dim] = {2, 3};
            std::list <float> p1 (v, v + dim);
            std::list <float> p2 (v, v + dim);
            std::list <float> p3 (w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
        {
            const unsigned dim = 3;
            int v [dim] = {1, 2, 3};
            int w [dim] = {2, 3, 4};
            std::set <int> p1 (v, v + dim);
            std::set <int> p2 (v, v + dim);
            std::set <int> p3 (w, w + dim);
            VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
            VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        }
    }

    void TestMath::TestEqual_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestEqual_MixedIterators () {
        const unsigned dim = 3;
        int v [dim] = {1, 2, 3};
        float w [dim] = {1, 2, 3};
        unsigned long u [dim] = {2, 3, 4};
        std::vector <int> p1 (v, v + dim);
        std::list <float> p2 (w, w + dim);
        std::deque <unsigned long> p3 (u, u + dim);
        VERIFY_TRUE(psimpl::math::equal <dim> (p1.begin (), p2.begin ()));
        VERIFY_TRUE(psimpl::math::equal <dim> (p2.begin (), p1.begin ()));
        VERIFY_FALSE(psimpl::math::equal <dim> (p1.begin (), p3.begin ()));
        VERIFY_FALSE(psimpl::math::equal <dim> (p3.begin (), p1.begin ()));
        VERIFY_FALSE(psimpl::math::equal <dim> (p2.begin (), p3.begin ()));
        VERIFY_FALSE(psimpl::math::equal <dim> (p3.begin (), p2.begin ()));
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestAdd_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {3, 5};
            double vec [dim];
            psimpl::math::add <dim> (p1, p2, vec);

            double expect [dim] = {4, 7};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float p2 [dim] = {4, 6, 8};
            std::vector <float> p1 (v, v + dim);
            float vec [dim];
            psimpl::math::add <dim> (p1.begin (), p2, vec);

            float expect [dim] = {5, 8, 11};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int p2 [dim] = {4, 6, 8, 10};
            std::deque <int> p1 (v, v + dim);
            int vec [dim];
            psimpl::math::add <dim> (p1.begin (), p2, vec);

            int expect [dim] = {5, 8, 11, 14};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
    }

    void TestMath::TestAdd_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float p2 [dim] = {3, 5};
            std::list <float> p1 (v, v + dim);
            float vec [dim];
            psimpl::math::add <dim> (p1.begin (), p2, vec);

            float expect [dim] = {4, 7};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int p2 [dim] = {4, 6, 8, 10};
            std::set <int> p1 (v, v + dim);
            int vec [dim];
            psimpl::math::add <dim> (p1.begin (), p2, vec);

            int expect [dim] = {5, 8, 11, 14};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
    }

    void TestMath::TestAdd_ForwardIterator () {
        FAIL("TODO");
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestSubtract_RandomIterator () {
        {
            const unsigned dim = 2;
            double p1 [dim] = {1, 2};
            double p2 [dim] = {3, 5};
            double vec [dim];
            psimpl::math::subtract <dim> (p1, p2, vec);

            double expect [dim] = {-2, -3};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {4, 6, 8};
            std::vector <float> p1 (v, v + dim);
            std::vector <float> p2 (w, w + dim);
            float vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            float expect [dim] = {-3, -4, -5};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::deque <int> p1 (v, v + dim);
            std::deque <int> p2 (w, w + dim);
            int vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            int expect [dim] = {-3, -4, -5, -6};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
    }

    void TestMath::TestSubtract_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [dim] = {1, 2};
            float w [dim] = {3, 5};
            std::list <float> p1 (v, v + dim);
            std::list <float> p2 (w, w + dim);
            float vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            float expect [dim] = {-2, -3};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {4, 6, 8, 10};
            std::set <int> p1 (v, v + dim);
            std::set <int> p2 (w, w + dim);
            int vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            int expect [dim] = {-3, -4, -5, -6};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
    }

    void TestMath::TestSubtract_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestSubtract_MixedIterators () {
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            int w [dim] = {4, 6, 8};
            std::vector <float> p1 (v, v + dim);
            std::list <int> p2 (w, w + dim);
            double vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            double expect [dim] = {-3, -4, -5};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
        {
            const unsigned dim = 3;
            int v [dim] = {1, 2, 3};
            float w [dim] = {4, 6, 8};
            std::vector <int> p1 (v, v + dim);
            std::list <float> p2 (w, w + dim);
            double vec [dim];
            psimpl::math::subtract <dim> (p1.begin (), p2.begin (), vec);

            double expect [dim] = {-3, -4, -5};
            VERIFY_TRUE(ComparePoint <dim> (vec, expect));
        }
    }

    void TestMath::TestSubtract_Unsigned () {
        const unsigned dim = 3;
        unsigned long v [dim] = {1, 2, 3};
        unsigned long w [dim] = {4, 6, 8};
        double vec [dim];
        psimpl::math::subtract <dim> (v, w, vec);

        double expect [dim] = {-3, -4, -5};
        VERIFY_TRUE(ComparePoint <dim> (vec, expect));
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestMultiply () {
        {
            const unsigned dim = 3;
            float p [dim] = {1, 2, 3};
            float v = 3;
            psimpl::math::multiply <dim> (p, v);

            float expect [dim] = {3, 6, 9};
            VERIFY_TRUE(ComparePoint <dim> (p, expect));
        }
        {
            const unsigned dim = 4;
            int p [dim] = {1, 2, 3, 4};
            int v = 2;
            psimpl::math::multiply <dim> (p, v);

            int expect [dim] = {2, 4, 6, 8};
            VERIFY_TRUE(ComparePoint <dim> (p, expect));
        }
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestDot () {
        {
            const unsigned dim = 2;
            double v [dim] = {1, 2};
            double w [dim] = {2, 3};
            VERIFY_TRUE(psimpl::math::dot <dim> (v, w) == 8.0);
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            float w [dim] = {2, 3, 4};
            VERIFY_TRUE(psimpl::math::dot <dim> (v, w) == 20.f);
        }
        {
            const unsigned dim = 4;
            int v [dim] = {1, 2, 3, 4};
            int w [dim] = {2, 3, 4, 5};
            VERIFY_TRUE(psimpl::math::dot <dim> (v, w) == 40);
    }
    }

    void TestMath::TestDot_Unsigned () {
        const unsigned dim = 4;
        unsigned int v [dim] = {1, 2, 3, 4};
        unsigned int w [dim] = {2, 3, 4, 5};
        VERIFY_TRUE(psimpl::math::dot <dim> (v, w) == 40);
        VERIFY_TRUE(psimpl::math::dot <dim> (w, v) == 40);
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestPointDistance () {
        const unsigned dim = 2;
        // identical points
        {
            float p1 [] = {1.f, 3.f};
            float p2 [] = {1.f, 3.f};
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::point_distance2 <dim> (p1, p2)));
        }
        // vertical difference
        {
            float p1 [] = {0.f, 1.f};
            float p2 [] = {0.f, 3.f};
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::point_distance2 <dim> (p1, p2)));
        }
        // horizontal distance
        {
            float p1 [] = {2.f, 0.f};
            float p2 [] = {5.f, 0.f};
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::point_distance2 <dim> (p1, p2)));
        }
    }

    void TestMath::TestPointDistance_RandomIterator () {
        {
            const unsigned dim = 2;
            float p1 [] = {1.f, 3.f};
            float p2 [] = {2.f, 5.f};
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::point_distance2 <dim> (p1, p2)));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 3.0, 4.0};
            double w [] = {2.0, 5.0, 7.0};
            std::vector <double> p1 (v, v + dim);
            std::vector <double> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(14.0, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
        {
            const unsigned dim = 4;
            int v [] = {1, 3, 4, 6};
            int w [] = {2, 5, 7, 8};
            std::deque <int> p1 (v, v + dim);
            std::deque <int> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(18, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
    }

    void TestMath::TestPointDistance_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [] = {1.f, 3.f};
            float w [] = {2.f, 5.f};
            std::list <float> p1 (v, v + dim);
            std::list <float> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 3.0, 4.0};
            double w [] = {2.0, 5.0, 7.0};
            std::set <double> p1 (v, v + dim);
            std::set <double> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(14.0, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
    }

    void TestMath::TestPointDistance_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestPointDistance_MixedIterators () {
        {
            const unsigned dim = 3;
            int v [dim] = {1, 2, 3};
            float w [dim] = {4, 6, 8};
            std::vector <int> p1 (v, v + dim);
            std::list <float> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(50.0, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
        {
            const unsigned dim = 3;
            float v [dim] = {1, 2, 3};
            int w [dim] = {4, 6, 8};
            std::vector <float> p1 (v, v + dim);
            std::list <int> p2 (w, w + dim);
            VERIFY_TRUE(CompareValue(50.f, psimpl::math::point_distance2 <dim> (p1.begin (), p2.begin ())));
        }
    }

    void TestMath::TestPointDistance_Unsigned () {
        const unsigned dim = 3;
        unsigned long v [dim] = {1, 2, 3};
        unsigned long w [dim] = {4, 6, 8};
        VERIFY_TRUE(CompareValue(50.0, psimpl::math::point_distance2 <dim> (v, w)));
        VERIFY_TRUE(CompareValue(50.0, psimpl::math::point_distance2 <dim> (w, v)));        
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestLineDistance () {
        const unsigned dim = 2;
        // zero length line
        {
            int l1 [] = {0, 0};
            int l2 [] = {0, 0};
            int p1 [] = {0, 0};
            VERIFY_TRUE(CompareValue(0, psimpl::math::line_distance2 <dim> (l1, l2, p1)));
            int p2 [] = {4, 3};
            VERIFY_TRUE(CompareValue(25, psimpl::math::line_distance2 <dim> (l1, l2, p2)));
        }
        // tiny line
        {
            float l1 [] = {0.f, 0.f};
            float l2 [] = {0.f, std::numeric_limits <float>::min ()};
            float p1 [] = {std::numeric_limits <float>::min (), 0.f};
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {4.f, 3.f};
            VERIFY_TRUE(CompareValue(25.f, psimpl::math::line_distance2 <dim> (l1, l2, p2)));
        }
        // end points
        {
            float l1 [] = {4.f, 2.f};
            float l2 [] = {1.f, 7.f};
            float p1 [] = {4.f, 2.f};    // start
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {1.f, 7.f};    // end
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p2)));
        }
        // vertical line
        {
            float l1 [] = {0.f, 1.f};
            float l2 [] = {0.f, 3.f};
            float p1 [] = {0.f, 0.f};    // below
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {0.f, 9.f};    // above
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {10.f, -6.f};    // to the right
            VERIFY_TRUE(CompareValue(100.f, psimpl::math::line_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {-4.f, 8.f};    // to the left
            VERIFY_TRUE(CompareValue(16.f, psimpl::math::line_distance2 <dim> (l1, l2, p4)));
        }
        // horizontal line
        {
            float l1 [] = {2.f, 0.f};
            float l2 [] = {3.f, 0.f};
            float p1 [] = {1.f, 0.f};    // to the left
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {7.f, 0.f};    // to the left
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::line_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {9.f, 2.f};    // above
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::line_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {4.f, -3.f};    // below
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::line_distance2 <dim> (l1, l2, p4)));
        }
    }

    void TestMath::TestLineDistance_RandomIterator () {
        {
            const unsigned dim = 2;
            float l1 [] = {1.f, 1.f};
            float l2 [] = {2.f, 2.f};
            float p [] = {1.f, 3.f};
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::line_distance2 <dim> (l1, l2, p)));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 0.0, 1.0};
            double w [] = {2.0, 0.0, 2.0};
            double u [] = {1.0, 0.0, 3.0};
            std::vector <double> l1 (v, v + dim);
            std::vector <double> l2 (w, w + dim);
            std::vector <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::line_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 4;
            int v [] = {1, 0, 3, 0};
            int w [] = {0, 0, 0, 0};
            int u [] = {4, 0, 2, 0};
            std::deque  <int> l1 (v, v + dim);
            std::deque  <int> l2 (w, w + dim);
            std::deque  <int> p (u, u + dim);
            VERIFY_TRUE(CompareValue(10, psimpl::math::line_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestLineDistance_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [] = {1.f, 1.f};
            float w [] = {2.f, 2.f};
            float u [] = {1.f, 3.f};
            std::list <float> l1 (v, v + dim);
            std::list <float> l2 (w, w + dim);
            std::list <float> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::line_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 0.0, 2.0};
            double w [] = {2.0, 0.0, 3.0};
            double u [] = {1.0, 0.0, 4.0};
            std::set <double> l1 (v, v + dim);
            std::set <double> l2 (w, w + dim);
            std::set <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::line_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestLineDistance_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestLineDistance_Unisgned () {
        const unsigned dim = 3;
        unsigned int v [] = {1, 0, 1};
        unsigned int w [] = {4, 0, 4};
        unsigned int u [] = {1, 0, 3};

        std::vector <unsigned int> l1 (v, v + dim);
        std::vector <unsigned int> l2 (w, w + dim);
        std::vector <unsigned int> p (u, u + dim);

        VERIFY_TRUE(CompareValue(2.0, psimpl::math::line_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        VERIFY_TRUE(CompareValue(2.0, psimpl::math::line_distance2 <dim> (l2.begin (), l1.begin (), p.begin ())));
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestSegmentDistance () {
        const unsigned dim = 2;
        // zero length segment
        {
            int l1 [] = {0, 0};
            int l2 [] = {0, 0};
            int p1 [] = {0, 0};
            VERIFY_TRUE(CompareValue(0, psimpl::math::segment_distance2 <dim> (l1, l2, p1)));
            int p2 [] = {4, 3};
            VERIFY_TRUE(CompareValue(25, psimpl::math::segment_distance2 <dim> (l1, l2, p2)));
        }
        // tiny segment
        {
            float l1 [] = {0.f, 0.f};
            float l2 [] = {0.f, std::numeric_limits <float>::min ()};
            float p1 [] = {std::numeric_limits <float>::min (), 0.f};
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::segment_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {4.f, 3.f};
            VERIFY_TRUE(CompareValue(25.f, psimpl::math::segment_distance2 <dim> (l1, l2, p2)));
        }
        // end points
        {
            float l1 [] = {4.f, 2.f};
            float l2 [] = {1.f, 7.f};
            float p1 [] = {4.f, 2.f};    // start
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::segment_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {1.f, 7.f};    // end
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::segment_distance2 <dim> (l1, l2, p2)));
        }
        // vertical segment
        {
            float l1 [] = {0.f, 1.f};
            float l2 [] = {0.f, 3.f};
            float p1 [] = {0.f, 0.f};    // below
            VERIFY_TRUE(CompareValue(1.f, psimpl::math::segment_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {0.f, 9.f};    // above
            VERIFY_TRUE(CompareValue(36.f, psimpl::math::segment_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {10.f, 2.f};   // to the right
            VERIFY_TRUE(CompareValue(100.f, psimpl::math::segment_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {-4.f, 2.f};   // to the left
            VERIFY_TRUE(CompareValue(16.f, psimpl::math::segment_distance2 <dim> (l1, l2, p4)));

            float p5 [] = {2.f, 0.f};    // below right
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::segment_distance2 <dim> (l1, l2, p5)));
            float p6 [] = {-2.f, 0.f};   // below left
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::segment_distance2 <dim> (l1, l2, p6)));
            float p7 [] = {3.f, 5.f};    // above right
            VERIFY_TRUE(CompareValue(13.f, psimpl::math::segment_distance2 <dim> (l1, l2, p7)));
            float p8 [] = {-3.f, 5.f};   // above left
            VERIFY_TRUE(CompareValue(13.f, psimpl::math::segment_distance2 <dim> (l1, l2, p8)));
        }
        // horizontal segment
        {
            float l1 [] = {2.f, 0.f};
            float l2 [] = {5.f, 0.f};
            float p1 [] = {1.f, 0.f};    // to the left
            VERIFY_TRUE(CompareValue(1.f, psimpl::math::segment_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {7.f, 0.f};    // to the right
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::segment_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {4.f, 2.f};    // above
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::segment_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {3.f, -3.f};   // below
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::segment_distance2 <dim> (l1, l2, p4)));

            float p5 [] = {1.f, 2.f};    // left above
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::segment_distance2 <dim> (l1, l2, p5)));
            float p6 [] = {1.f, -2.f};   // left below
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::segment_distance2 <dim> (l1, l2, p6)));
            float p7 [] = {7.f, 2.f};    // right above
            VERIFY_TRUE(CompareValue(8.f, psimpl::math::segment_distance2 <dim> (l1, l2, p7)));
            float p8 [] = {7.f, -2.f};   // right below
            VERIFY_TRUE(CompareValue(8.f, psimpl::math::segment_distance2 <dim> (l1, l2, p8)));
        }
    }

    void TestMath::TestSegmentDistance_RandomIterator () {
        {
            const unsigned dim = 2;
            float l1 [] = {1.f, 1.f};
            float l2 [] = {2.f, 2.f};
            float p [] = {1.f, 3.f};
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::segment_distance2 <dim> (l1, l2, p)));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 0.0, 1.0};
            double w [] = {2.0, 0.0, 2.0};
            double u [] = {1.0, 0.0, 3.0};
            std::vector <double> l1 (v, v + dim);
            std::vector <double> l2 (w, w + dim);
            std::vector <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 4;
            int v [] = {1, 0, 3, 0};
            int w [] = {0, 0, 0, 0};
            int u [] = {4, 0, 2, 0};
            std::deque  <int> l1 (v, v + dim);
            std::deque  <int> l2 (w, w + dim);
            std::deque  <int> p (u, u + dim);
            VERIFY_TRUE(CompareValue(10, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestSegmentDistance_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [] = {1.f, 1.f};
            float w [] = {2.f, 2.f};
            float u [] = {1.f, 3.f};
            std::list <float> l1 (v, v + dim);
            std::list <float> l2 (w, w + dim);
            std::list <float> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 0.0, 2.0};
            double w [] = {2.0, 0.0, 3.0};
            double u [] = {1.0, 0.0, 4.0};
            std::set <double> l1 (v, v + dim);
            std::set <double> l2 (w, w + dim);
            std::set <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestSegmentDistance_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestSegmentDistance_MixedIterators () {
        {
            const unsigned dim = 3;
            float v [] = {1, 0, 1};
            float w [] = {4, 0, 4};
            int u [] = {1, 0, 3};

            std::vector <float> l1 (v, v + dim);
            std::vector <float> l2 (w, w + dim);
            std::vector <int> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 3;
            int v [] = {1, 0, 1};
            int w [] = {4, 0, 4};
            float u [] = {1, 0, 3};

            std::vector <int> l1 (v, v + dim);
            std::vector <int> l2 (w, w + dim);
            std::vector <float> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::segment_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestSegmentDistance_Unsigned () {
        const unsigned dim = 3;
        unsigned int v [] = {1, 0, 1};
        unsigned int w [] = {4, 0, 4};
        unsigned int u [] = {1, 0, 3};

        VERIFY_TRUE(CompareValue(2.0, psimpl::math::segment_distance2 <dim> (v, w, u)));
        VERIFY_TRUE(CompareValue(2.0, psimpl::math::segment_distance2 <dim> (w, v, u)));
    }

    // ---------------------------------------------------------------------------------------------

    void TestMath::TestRayDistance () {
        const unsigned dim = 2;
        // zero length segment
        {
            int l1 [] = {0, 0};
            int l2 [] = {0, 0};
            int p1 [] = {0, 0};
            VERIFY_TRUE(CompareValue(0, psimpl::math::ray_distance2 <dim> (l1, l2, p1)));
            int p2 [] = {4, 3};
            VERIFY_TRUE(CompareValue(25, psimpl::math::ray_distance2 <dim> (l1, l2, p2)));
        }
        // tiny segment
        {
            float l1 [] = {0.f, 0.f};
            float l2 [] = {0.f, std::numeric_limits <float>::min ()};
            float p1 [] = {std::numeric_limits <float>::min (), 0.f};
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::ray_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {4.f, 3.f};
            VERIFY_TRUE(CompareValue(25.f, psimpl::math::ray_distance2 <dim> (l1, l2, p2)));
        }
        // end points
        {
            float l1 [] = {4.f, 2.f};
            float l2 [] = {1.f, 7.f};
            float p1 [] = {4.f, 2.f};    // start
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::ray_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {1.f, 7.f};    // end
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::ray_distance2 <dim> (l1, l2, p2)));
        }
        // vertical ray
        {
            float l1 [] = {0.f, 1.f};
            float l2 [] = {0.f, 3.f};
            float p1 [] = {0.f, 0.f};    // below
            VERIFY_TRUE(CompareValue(1.f, psimpl::math::ray_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {0.f, 9.f};    // above
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::ray_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {10.f, 2.f};   // to the right
            VERIFY_TRUE(CompareValue(100.f, psimpl::math::ray_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {-4.f, 2.f};   // to the left
            VERIFY_TRUE(CompareValue(16.f, psimpl::math::ray_distance2 <dim> (l1, l2, p4)));

            float p5 [] = {2.f, 0.f};    // below right
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::ray_distance2 <dim> (l1, l2, p5)));
            float p6 [] = {-2.f, 0.f};   // below left
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::ray_distance2 <dim> (l1, l2, p6)));
            float p7 [] = {3.f, 5.f};    // above right
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::ray_distance2 <dim> (l1, l2, p7)));
            float p8 [] = {-3.f, 5.f};   // above left
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::ray_distance2 <dim> (l1, l2, p8)));
        }
        // horizontal ray
        {
            float l1 [] = {2.f, 0.f};
            float l2 [] = {5.f, 0.f};
            float p1 [] = {1.f, 0.f};    // to the left
            VERIFY_TRUE(CompareValue(1.f, psimpl::math::ray_distance2 <dim> (l1, l2, p1)));
            float p2 [] = {7.f, 0.f};    // to the right
            VERIFY_TRUE(CompareValue(0.f, psimpl::math::ray_distance2 <dim> (l1, l2, p2)));
            float p3 [] = {4.f, 2.f};    // above
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::ray_distance2 <dim> (l1, l2, p3)));
            float p4 [] = {3.f, -3.f};   // below
            VERIFY_TRUE(CompareValue(9.f, psimpl::math::ray_distance2 <dim> (l1, l2, p4)));

            float p5 [] = {1.f, 2.f};    // left above
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::ray_distance2 <dim> (l1, l2, p5)));
            float p6 [] = {1.f, -2.f};   // left below
            VERIFY_TRUE(CompareValue(5.f, psimpl::math::ray_distance2 <dim> (l1, l2, p6)));
            float p7 [] = {7.f, 2.f};    // right above
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::ray_distance2 <dim> (l1, l2, p7)));
            float p8 [] = {7.f, -2.f};   // right below
            VERIFY_TRUE(CompareValue(4.f, psimpl::math::ray_distance2 <dim> (l1, l2, p8)));
        }
    }

    void TestMath::TestRayDistance_RandomIterator () {
        {
            const unsigned dim = 2;
            float l1 [] = {1.f, 1.f};
            float l2 [] = {2.f, 2.f};
            float p [] = {1.f, 3.f};
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::ray_distance2 <dim> (l1, l2, p)));
        }
        {
            const unsigned dim = 3;
            double v [] = {1.0, 0.0, 1.0};
            double w [] = {2.0, 0.0, 2.0};
            double u [] = {9.0, 0.0, 11.0};
            std::vector <double> l1 (v, v + dim);
            std::vector <double> l2 (w, w + dim);
            std::vector <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.0, psimpl::math::ray_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 4;
            int v [] = {1, 2, 3, 4};
            int w [] = {2, 3, 4, 5};
            int u [] = {0, 1, 2, 3};
            std::deque  <int> l1 (v, v + dim);
            std::deque  <int> l2 (w, w + dim);
            std::deque  <int> p (u, u + dim);
            VERIFY_TRUE(CompareValue(4, psimpl::math::ray_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestRayDistance_BidirectionalIterator () {
        {
            const unsigned dim = 2;
            float v [] = {1.f, 1.f};
            float w [] = {2.f, 2.f};
            float u [] = {9.f, 11.f};
            std::list <float> l1 (v, v + dim);
            std::list <float> l2 (w, w + dim);
            std::list <float> p (u, u + dim);
            VERIFY_TRUE(CompareValue(2.f, psimpl::math::ray_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
        {
            const unsigned dim = 3;
            double v [] = {5.0, 0.0, 8.0};
            double w [] = {7.0, 0.0, 9.0};
            double u [] = {1.0, 0.0, 4.0};
            std::set <double> l1 (v, v + dim);
            std::set <double> l2 (w, w + dim);
            std::set <double> p (u, u + dim);
            VERIFY_TRUE(CompareValue(32.0, psimpl::math::ray_distance2 <dim> (l1.begin (), l2.begin (), p.begin ())));
        }
    }

    void TestMath::TestRayDistance_ForwardIterator () {
        FAIL("TODO");
    }

    void TestMath::TestRayDistance_Unsigned () {
        const unsigned dim = 3;
        double v [] = {1, 0, 1};
        double w [] = {4, 0, 4};
        double u [] = {9, 0, 11};

        VERIFY_TRUE(CompareValue(2.0, psimpl::math::ray_distance2 <dim> (v, w, u)));
        VERIFY_TRUE(CompareValue(74.0, psimpl::math::ray_distance2 <dim> (w, v, u)));
    }

}}
