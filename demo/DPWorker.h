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

#ifndef DPWORKER_H
#define DPWORKER_H


#include <QtCore/QObject>
#include <QtCore/QVector>


namespace psimpl {
    typedef enum CONTAINER
    {
        ARRAY_FLOAT,
        QVECTOR_DOUBLE,
        VECTOR_DOUBLE,
        LIST_LONGLONG,
    } Container;

    /*!
        \brief Worker class for generating and simplifying polylines.
        
        Polylines are always generated in a QVector <qreal> container. Before simplification the
        polyline is converted to the specified container type. This allows for easy adding of new 
        container types.
        
    */
    class DPWorker : public QObject
    {
        Q_OBJECT

    public:
        DPWorker (QObject* inParent = 0);
        ~DPWorker () = default;

        DPWorker( const DPWorker& ) = default;
        DPWorker( DPWorker&& ) = default;
        DPWorker& operator =( const DPWorker& ) = default;
        DPWorker& operator =( DPWorker&& ) = default;

        void Generate (int inCount);
        void SimplifyNP (Container cont, int n);
        void SimplifyRD (Container cont, const QString& tol);
        void SimplifyPD (Container cont, const QString& tol, int repeat);
        void SimplifyRW (Container cont, const QString& tol);
        void SimplifyOp (Container cont, const QString& minTol, const QString& maxTol);
        void SimplifyLa (Container cont, const QString& tol, int size);
        void SimplifyDP_classic (Container cont, const QString& tol);
        void SimplifyDP (Container cont, const QString& tol);
        void SimplifyDP_N (Container cont, int count);
        void SimplifyDP_reference (const QString& tol);

        int GetGeneratedPointCount () { return mGeneratedCoords.size () / 2; }
        int GetSimplifiedGeneratedPointCount () { return mSimplifiedCoords.size () / 2; }

    private:
        void DoSignalSimplifiedPolyline (qreal duration);

    signals:
        void SignalGeneratingPolyline ();
        void SignalConvertingPolyline ();
        void SignalSimplifyingPolyline ();
        void SignalCleaningConvertedPolyline ();
        
        void SignalGeneratedPolyline (int duration, const QVector <qreal>& polyline);
        void SignalSimplifiedPolyline (int duration, const QVector <qreal>& polyline);
        void SignalSimplifiedPolyline (int duration, const QVector <qreal>& polyline, double max, double sum, double mean, double std);

    public:
        QVector <qreal> mGeneratedCoords;
        QVector <qreal> mSimplifiedCoords;
    };

} // namespace psimpl


#endif // DPWORKER_H
