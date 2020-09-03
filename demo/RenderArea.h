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

#ifndef RENDERAREA_H
#define RENDERAREA_H


#include <QFrame>
#include <QPainterPath>


namespace psimpl {

    /*!
        \brief A frame that can draw polylines and their simplification.
        
        Note that the point count of each polyline is always limited to 100.000 to speed up drawing.
    */
    class RenderArea : public QFrame
    {
    public:
        RenderArea (QWidget *inParent = 0, Qt::WindowFlags inFlags = 0);
        void SetGeneratedPolyline (QVector <qreal>& polyline);
        void SetSimplifiedPolyline (QVector <qreal>& polyline);
        void SetVisibleGeneratedPolyline (bool visible) { mDrawGeneratedPolyline = visible; }
        void SetVisibleSimplifiedPolyline (bool visible) { mDrawSimplifiedPolyline = visible; }
        void SetKeepAspectRatio (bool keep) { mKeepAspectRatio = keep; }

    protected:
        void paintEvent (QPaintEvent *inEvent);

    private:
        QPainterPath Convert (QVector <qreal>& polyline);

    private:
        QPainterPath mGeneratedPolyline;
        QPainterPath mSimplifiedPolyline;
        bool mDrawGeneratedPolyline;
        bool mDrawSimplifiedPolyline;
        bool mKeepAspectRatio;
    };

} // namespace psimpl


#endif // RENDERAREA_H
