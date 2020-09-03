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

#ifndef PSIMPL_TEST_SIMPLIFICATION
#define PSIMPL_TEST_SIMPLIFICATION


#include "test.h"
#include "TestNthPoint.h"
#include "TestRadialDistance.h"
#include "TestPerpendicularDistance.h"
#include "TestReumannWitkam.h"
#include "TestOpheim.h"
#include "TestLang.h"
#include "TestDouglasPeucker.h"


namespace psimpl {
    namespace test
{
    class TestSimplification
    {
    public:
        TestSimplification () {
            TEST_RUN("nth point", TestNthPoint ());
            TEST_RUN("radial distance", TestRadialDistance ());
            TEST_RUN("perpendicular distance", TestPerpendicularDistance ());
            TEST_RUN("reumann witkam", TestReumannWitkam ());
            TEST_RUN("opheim", TestOpheim ());
            TEST_RUN("lang", TestLang ());
            TEST_RUN("douglas peucker classic", TestDouglasPeuckerClassic ());
            TEST_RUN("douglas peucker", TestDouglasPeucker ());
            TEST_RUN("douglas peucker n", TestDouglasPeuckerN ());
        }
    };
}}


#endif // PSIMPL_TEST_SIMPLIFICATION
