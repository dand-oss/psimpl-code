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

#include "DPWorker.h"
#include "psimpl_reference.h"
#include <psimpl.h>
#include <cmath>
#include <QtCore/QTime>

namespace psimpl {

    DPWorker::DPWorker (QObject* inParent) :
        QObject (inParent)
    {
        // note: disable this line during testing
        srand ((unsigned)QTime::currentTime ().msec ());
    }

    void DPWorker::Generate (int inCount) {
        emit SignalGeneratingPolyline ();

        QTime t;
        t.start ();

        mGeneratedCoords.resize (inCount*2);
        mSimplifiedCoords.clear ();

        qreal miny = inCount;
        qreal maxy = -inCount;

        const qreal PI = std::atan (1.0) * 4;
        qreal step = 2 * PI / inCount;

        int a = 1 + (rand () % 2);    // [1, 3]
        int b = 2 + (rand () % 3);    // [2, 5]
        int c = 3 + (rand () % 7);    // [3, 10]

        // generate a random line
        for (int i=0; i<inCount; i++) {
            mGeneratedCoords [i*2] = i;
            qreal tmp1 = cos (step * i * a) / 3;
            qreal tmp2 = sin (step * i * b) / 5;
            qreal tmp3 = sin (step * i * c) / 10;
            mGeneratedCoords [i*2+1] = tmp1 + tmp2 + tmp3;
            miny = qMin (miny, mGeneratedCoords [i*2+1]);
            maxy = qMax (maxy, mGeneratedCoords [i*2+1]);
        }
        // translate the line to (0,0) and scale the line to (2 * inCount, inCount)
        qreal scaley = (inCount-1) / (maxy-miny);
        for (int i=0; i<inCount; i++) {
            mGeneratedCoords [i*2+1] = (mGeneratedCoords [i*2+1] - miny) * 0.5 * scaley;
        }
        //mGeneratedCoords.push_back(0);
        emit SignalGeneratedPolyline (t.elapsed (), mGeneratedCoords);
    }

    void DPWorker::SimplifyNP (Container cont, int n) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_nth_point <2> (begin, end, n,
                                    std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_nth_point <2> (begin, end, n,
                                    std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_nth_point <2> (begin, end, n,
                                    std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_nth_point <2> (begin, end, n,
                                    std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyRD (Container cont, QString tol) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_radial_distance <2> (begin, end, tol.toFloat(),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_radial_distance <2> (begin, end, tol.toDouble (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_radial_distance <2> (begin, end, tol.toDouble (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_radial_distance <2> (begin, end, tol.toLongLong (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyPD (Container cont, QString tol, int repeat) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_perpendicular_distance <2> (begin, end, tol.toFloat(), repeat,
                                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_perpendicular_distance <2> (begin, end, tol.toDouble (), repeat,
                                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_perpendicular_distance <2> (begin, end, tol.toDouble (), repeat,
                                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_perpendicular_distance <2> (begin, end, tol.toLongLong (), repeat,
                                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyRW (Container cont, QString tol)
    {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_reumann_witkam <2> (begin, end, tol.toFloat(),
                                         std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_reumann_witkam <2> (begin, end, tol.toDouble (),
                                         std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_reumann_witkam <2> (begin, end, tol.toDouble (),
                                         std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_reumann_witkam <2> (begin, end, tol.toLongLong (),
                                         std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyOp (Container cont, QString minTol, QString maxTol) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_opheim <2> (begin, end, minTol.toFloat(), maxTol.toFloat(),
                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_opheim <2> (begin, end, minTol.toDouble(), maxTol.toDouble(),
                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_opheim <2> (begin, end, minTol.toDouble(), maxTol.toDouble(),
                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_opheim <2> (begin, end, minTol.toLongLong(), maxTol.toLongLong(),
                                 std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyLa (Container cont, QString tol, int size) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_lang <2> (begin, end, tol.toFloat(), size,
                               std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_lang <2> (begin, end, tol.toDouble(), size,
                               std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_lang <2> (begin, end, tol.toDouble(), size,
                               std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_lang <2> (begin, end, tol.toLongLong(), size,
                               std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyDP_classic (Container cont, QString tol) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_douglas_peucker_classic <2> (begin, end, tol.toFloat(),
                                                  std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_douglas_peucker_classic <2> (begin, end, tol.toDouble (),
                                                  std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker_classic <2> (begin, end, tol.toDouble (),
                                                  std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker_classic <2> (begin, end, tol.toLongLong (),
                                                  std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyDP (Container cont, QString tol) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_douglas_peucker <2> (begin, end, tol.toFloat(),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_douglas_peucker <2> (begin, end, tol.toDouble (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker <2> (begin, end, tol.toDouble (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker <2> (begin, end, tol.toLongLong (),
                                          std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyDP_N (Container cont, int count) {
        QTime t;
        int duration = 0;

        mSimplifiedCoords.clear ();

        switch (cont) {
        case ARRAY_FLOAT:
        {
            // convert
            emit SignalConvertingPolyline ();
            auto* generatedPoints = new double [mGeneratedCoords.size ()];
            for (int c=0; c<mGeneratedCoords.size (); c++) {
                generatedPoints [c] = mGeneratedCoords [c];
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            const auto* begin = generatedPoints;
            const auto* end = generatedPoints + mGeneratedCoords.size ();
            t.start ();
            simplify_douglas_peucker_n <2> (begin, end, count,
                                            std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            // done
            emit SignalCleaningConvertedPolyline ();
            delete [] generatedPoints;
            break;
        }
        case QVECTOR_DOUBLE:
        {
            // simplify
            emit SignalSimplifyingPolyline ();
            QVector <qreal>::const_iterator begin = mGeneratedCoords.constBegin ();
            QVector <qreal>::const_iterator end = mGeneratedCoords.constEnd ();
            t.start ();
            simplify_douglas_peucker_n <2> (begin, end, count,
                                            std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            break;
        }
        case VECTOR_DOUBLE:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::vector <double> generatedPoints = mGeneratedCoords.toStdVector ();
            // simplify
            emit SignalSimplifyingPolyline ();
            std::vector <double>::const_iterator begin = generatedPoints.begin ();
            std::vector <double>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker_n <2> (begin, end, count,
                                            std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        case LIST_LONGLONG:
        {
            // convert
            emit SignalConvertingPolyline ();
            std::list <long long> generatedPoints;
            for (auto coord : mGeneratedCoords) {
                generatedPoints.push_back (static_cast<long long>(coord));
            }
            // simplify
            emit SignalSimplifyingPolyline ();
            std::list <long long>::const_iterator begin = generatedPoints.begin ();
            std::list <long long>::const_iterator end = generatedPoints.end ();
            t.start ();
            simplify_douglas_peucker_n <2> (begin, end, count,
                                            std::back_inserter (mSimplifiedCoords));
            duration = t.elapsed ();
            emit SignalCleaningConvertedPolyline ();
            break;
        }
        default:
            break;
        }
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::SimplifyDP_reference (QString tol) {
        mSimplifiedCoords.clear();
        // convert generated polyline to Point array
        emit SignalConvertingPolyline ();
        int pointCount = mGeneratedCoords.size () / 2;
        reference::Point* generatedPoints = new reference::Point [pointCount];
        reference::Point* simplifiedPoints = new reference::Point [pointCount];
        for (int i=0; i<pointCount; i++) {
            generatedPoints [i].x = mGeneratedCoords [i*2];
            generatedPoints [i].y = mGeneratedCoords [i*2+1];
        }
        emit SignalSimplifyingPolyline ();
        QTime t;
        t.start ();
        // simplify
        int simplCount = reference::poly_simplify (tol.toDouble (), generatedPoints,
                                                 pointCount, simplifiedPoints);
        int duration = t.elapsed ();
        // convert simplified Point array to simplified polyline
        mSimplifiedCoords.resize (simplCount * 2);
        for (int i=0; i<simplCount; i++) {
            mSimplifiedCoords [i*2] = simplifiedPoints [i].x;
            mSimplifiedCoords [i*2+1] = simplifiedPoints [i].y;
        }
        // done
        delete [] generatedPoints;
        delete [] simplifiedPoints;
        
        DoSignalSimplifiedPolyline (duration);
    }

    void DPWorker::DoSignalSimplifiedPolyline (qreal duration) {
        bool validStatistics = false;

        error::statistics stats = 
            compute_positional_error_statistics <2> (
                mGeneratedCoords.constBegin (), mGeneratedCoords.constEnd (),
                mSimplifiedCoords.constBegin (), mSimplifiedCoords.constEnd (), &validStatistics);

        if (validStatistics) {
            emit SignalSimplifiedPolyline (static_cast<int>(duration), mSimplifiedCoords, stats.max, stats.sum, stats.mean, stats.std);
        }
        else {
            emit SignalSimplifiedPolyline (static_cast<int>(duration), mSimplifiedCoords);
        }
    }
} // namespace psimpl
