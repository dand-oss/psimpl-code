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
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010-2011
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/*
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl, and is hosted at SourceForge:
    http://sourceforge.net/projects/psimpl/
*/

#include "RenderArea.h"
#include <QPainter>


namespace psimpl {

    void RenderArea::paintEvent(QPaintEvent * /*inEvent*/) {
        // exit if no generated line
        if (!mGeneratedPolyline.elementCount ()) {
            return;
        }

        // exit if no rect to paint
        const auto& rect = mGeneratedPolyline.boundingRect ();
        if (!rect.isValid ()) {
            return;
        }

        QPainter painter (this);

        // find width / height ratio
        const auto scalew = ! mKeepAspectRatio
            ? (width () - 1) / rect.width ()
            : qMin(
                (width () - 1) / rect.width (),
                (height () - 1) / rect.height ());

        if (! mKeepAspectRatio) {
            painter.scale (scalew, (height () - 1) / rect.height ());
        }
        else {
            painter.translate(
                (width () - (rect.width () * scalew)) / 2.0,
                (height () - (rect.height () * scalew)) / 2.0);
            painter.scale (scalew, scalew);
        }
        painter.translate (-rect.left (), -rect.top ());

        // Draw the blue generated line
        if (mDrawGeneratedPolyline) {
            painter.setPen (QPen(Qt::darkBlue, 1.0/scalew));
            painter.drawPath (mGeneratedPolyline);
        }

        // Draw the red simplified line
        if (mDrawSimplifiedPolyline && mSimplifiedPolyline.elementCount ()) {
            painter.setPen (QPen(Qt::darkRed, 1.0/scalew));
            painter.drawPath (mSimplifiedPolyline);
        }
    }

    QPainterPath RenderArea::Convert (const QVector <qreal>& polyline)
    {
        // limit paths to max 100.000 points to speed up drawing
        const int threshold = 100000;

        QPainterPath path;
        if (polyline.empty ()) {
            return path;
        }
        const auto pointCount = polyline.size () / 2;
        const auto skipStep = (qreal) (pointCount - threshold) / (qreal) threshold;
        auto skipValue = skipStep;

        path.moveTo(polyline [0], polyline [1]);
        for (int i=1; i<pointCount; i++) {
            if (skipValue > 1.0) {
                skipValue -= 1.0;
            }
            else {
                path.lineTo (polyline [i*2], polyline [i*2+1]);
                skipValue += skipStep;
            }
        }
        return path;
    }

    void RenderArea::SetGeneratedPolyline (const QVector <qreal>& polyline)
    {
        mSimplifiedPolyline = QPainterPath ();
        mGeneratedPolyline = Convert (polyline);
    }

    void RenderArea::SetSimplifiedPolyline (const QVector <qreal>& polyline)
    {
        mSimplifiedPolyline = Convert (polyline);
    }

} // namespace psimpl
