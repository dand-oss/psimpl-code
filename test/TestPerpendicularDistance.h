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

#ifndef PSIMPL_TEST_PERPENDICULAR_DISTANCE
#define PSIMPL_TEST_PERPENDICULAR_DISTANCE


#include "test.h"


namespace psimpl {
    namespace test
{
    //! Tests function psimpl::simplify_perpendicular_distance
    class TestPerpendicularDistance
    {
    public:
        TestPerpendicularDistance ();

    private:
        void TestIncompletePoint_sp ();
        void TestNotEnoughPoints_sp ();
        void TestInvalidTol_sp ();
        void TestValidTol_sp ();
        void TestBasicSanity ();
        void TestRandomIterator_sp ();
        void TestBidirectionalIterator_sp ();
        void TestForwardIterator_sp ();
        void TestReturnValue_sp ();
        void TestIntegers_sp ();

        void TestIncompletePoint_mp ();
        void TestNotEnoughPoints_mp ();
        void TestInvalidTol_mp ();
        void TestValidTol_mp ();
        void TestInvalidRepeat_mp ();
        void TestValidRepeat_mp ();
        void TestReturnValue_mp ();
        void TestIntegers_mp ();
    };
}}


#endif // PSIMPL_TEST_PERPENDICULAR_DISTANCE
