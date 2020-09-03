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

#ifndef PSIMPL_TEST_MATH
#define PSIMPL_TEST_MATH


namespace psimpl {
    namespace test
{
    class TestMath
    {
    public:
        TestMath ();

    private:
        void TestEqual_RandomIterator ();
        void TestEqual_BidirectionalIterator ();
        void TestEqual_ForwardIterator ();
        void TestEqual_MixedIterators ();

        void TestAdd_RandomIterator ();
        void TestAdd_BidirectionalIterator ();
        void TestAdd_ForwardIterator ();

        void TestSubtract_RandomIterator ();
        void TestSubtract_BidirectionalIterator ();
        void TestSubtract_ForwardIterator ();
        void TestSubtract_MixedIterators ();
        void TestSubtract_Unsigned ();

        void TestMultiply ();

        void TestDot ();
        void TestDot_Unsigned ();

        void TestPointDistance ();
        void TestPointDistance_RandomIterator ();
        void TestPointDistance_BidirectionalIterator ();
        void TestPointDistance_ForwardIterator ();
        void TestPointDistance_MixedIterators ();
        void TestPointDistance_Unsigned ();

        void TestLineDistance ();
        void TestLineDistance_RandomIterator ();
        void TestLineDistance_BidirectionalIterator ();
        void TestLineDistance_ForwardIterator ();
        void TestLineDistance_Unisgned ();

        void TestSegmentDistance ();
        void TestSegmentDistance_RandomIterator ();
        void TestSegmentDistance_BidirectionalIterator ();
        void TestSegmentDistance_ForwardIterator ();
        void TestSegmentDistance_MixedIterators ();
        void TestSegmentDistance_Unsigned ();

        void TestRayDistance ();
        void TestRayDistance_RandomIterator ();
        void TestRayDistance_BidirectionalIterator ();
        void TestRayDistance_ForwardIterator ();
        void TestRayDistance_Unsigned  ();
    };
}}


#endif // PSIMPL_TEST_MATH
