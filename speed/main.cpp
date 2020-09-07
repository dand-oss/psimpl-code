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

#include "psimpl.h"
// cheat!
#include "../demo/psimpl_reference.h"
#include "prev/psimpl.h"
#include "cycle_p.h"

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <QDirIterator>
#include <QFileInfo>
#include <QStringList>
#include <QPair>
#include <QVector>
#include <QTextStream>


using Setting = QPair <QString, QStringList>;
using Settings = QVector <Setting> ;


const unsigned DIM = 2;
      unsigned repeat = 5;

// -----------------------------------------------------------------------------

class BenchmarkController
{
public:
    BenchmarkController (unsigned& result) :
        mResult (result),
        mRepeat (repeat)
    {
        mTicks = getticks ();
    }

    ~BenchmarkController () {
        const auto index = repeat / 2;
        qSort (mMeasurements);
        mResult = mMeasurements [index];
    }
    BenchmarkController(const BenchmarkController&) = default;
    BenchmarkController& operator = (const BenchmarkController&) = default;
    BenchmarkController(BenchmarkController&&) = default;
    BenchmarkController& operator = (BenchmarkController&&) = default;

    void StartMeasurement () {
        const auto measurement = static_cast<unsigned>(getticks ()) - mTicks;
        mMeasurements.push_back (measurement);
        --mRepeat;
        mTicks = getticks ();
    }

    bool IsDone () {
        return mRepeat == 0;
    }

private:
    unsigned& mResult;
    unsigned mRepeat;
    CycleCounterTicks mTicks;
    QVector <unsigned> mMeasurements;
};

#define BENCHMARK(elapsed) for (BenchmarkController ctrl (elapsed); !ctrl.IsDone (); ctrl.StartMeasurement ())

// -----------------------------------------------------------------------------

template <class Iterator>
void Benchmark_Ran (
    const std::string& container,
    const Iterator& first,
    const Iterator& last,
    const Settings& settings)
{
    // benchmark all algorithms that support at least random iterators
    const auto polylineSize = std::distance (first, last);
    unsigned newElapsed = 0;
    unsigned oldElapsed = 0;

    for (const Setting& setting : settings)
    {
        psimpl::util::scoped_array <double> newSimplification (polylineSize);
        psimpl::error::statistics newStats;
        int newSimplificationSize = 0;

        psimpl::util::scoped_array <double> oldSimplification (polylineSize);
        psimpl::error::statistics oldStats;
        int oldSimplificationSize = 0;

        if (setting.first == "simplify_nth_point") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_radial_distance") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_perpendicular_distance") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_reumann_witkam") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                   psimpl::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                   psimpl_prev::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_opheim") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_lang") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_douglas_peucker_classic") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_douglas_peucker_classic <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_douglas_peucker") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_douglas_peucker_n") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_douglas_peucker_n <DIM> (first, last, setting.second [0].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_douglas_peucker_n <DIM> (first, last, setting.second [0].toUInt (), oldSimplification.get ()));
            }
        }
        else {
            continue;
        }

        newStats = psimpl::compute_positional_error_statistics <DIM> (first, last, newSimplification.get (), newSimplification.get () + newSimplificationSize);
        oldStats = psimpl::compute_positional_error_statistics <DIM> (first, last, oldSimplification.get (), oldSimplification.get () + oldSimplificationSize);

        std::cout << container << ",";
        QTextStream(stdout) << setting.first << ",";
        std::cout << newSimplificationSize / DIM << "," << (int) (((float) newSimplificationSize / (float) polylineSize) * 100.f) << "%,";
        std::cout << newElapsed << "," << newStats.mean << "," << newStats.std << "," << newStats.max << "," << newStats.sum << ",";
        if (oldElapsed) {
            std::cout << oldElapsed << "," << oldStats.mean << "," << oldStats.std << "," << oldStats.max << "," << oldStats.sum << ",";
            std::cout << (int) ((((float) oldElapsed - (float) newElapsed) / (float) oldElapsed) * 100.f) << "%";
        }
        std::cout << std::endl;
    }
}

// -----------------------------------------------------------------------------

template <class Iterator>
void Benchmark_Bid (
    const std::string& container,
    const Iterator& first,
    const Iterator& last,
    const Settings& settings)
{
    // benchmark all algorithms that support at least bidirectional iterators
    const auto polylineSize = std::distance (first, last);
    unsigned newElapsed = 0;
    unsigned oldElapsed = 0;

    for (const Setting& setting : settings)
    {
        psimpl::util::scoped_array <double> newSimplification (polylineSize);
        psimpl::error::statistics newStats;
        int newSimplificationSize = 0;

        psimpl::util::scoped_array <double> oldSimplification (polylineSize);
        psimpl::error::statistics oldStats;
        int oldSimplificationSize = 0;

        if (setting.first == "simplify_nth_point") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_radial_distance") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_perpendicular_distance") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_reumann_witkam") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                   psimpl::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                   psimpl_prev::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_opheim") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_lang") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), oldSimplification.get ()));
            }
        }
        else if (setting.first == "simplify_douglas_peucker") {
            BENCHMARK(newElapsed) {
                newSimplificationSize =  std::distance (newSimplification.get (),
                    psimpl::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), newSimplification.get ()));
            }
            BENCHMARK(oldElapsed) {
                oldSimplificationSize =  std::distance (oldSimplification.get (),
                    psimpl_prev::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), oldSimplification.get ()));
            }
        }
        else {
            continue;
        }

        newStats = psimpl::compute_positional_error_statistics <DIM> (first, last, newSimplification.get (), newSimplification.get () + newSimplificationSize);
        oldStats = psimpl::compute_positional_error_statistics <DIM> (first, last, oldSimplification.get (), oldSimplification.get () + oldSimplificationSize);

        std::cout << container << ",";
        QTextStream(stdout) << setting.first << ",";
        std::cout << newSimplificationSize / DIM << "," << (int) (((float) newSimplificationSize / (float) polylineSize) * 100.f) << "%,";
        std::cout << newElapsed << "," << newStats.mean << "," << newStats.std << "," << newStats.max << "," << newStats.sum << ",";
        std::cout << oldElapsed << "," << oldStats.mean << "," << oldStats.std << "," << oldStats.max << "," << oldStats.sum << ",";
        std::cout << (int) ((((float) oldElapsed - (float) newElapsed) / (float) oldElapsed) * 100.f) << "%" << std::endl;
    }
}

// -----------------------------------------------------------------------------

template <class Iterator>
void Benchmark_Ref (
    const std::string& container,
    const Iterator& first,
    const Iterator& last,
    const Settings& settings)
{
    // benchmark all reference algorithms
    const auto polylineSize = std::distance (first, last);
    const auto pointCount = polylineSize / DIM;
    int simplificationPointCount = 0;
    unsigned elapsed = 0;

    for (const Setting& setting : settings)
    {
        if (setting.first != "simplify_douglas_peucker_reference") {
            continue;
        }

        psimpl::util::scoped_array <double> simplification (polylineSize);
        int simplificationSize = 0;
        psimpl::error::statistics newStats;

        psimpl::util::scoped_array <psimpl::reference::Point> polylinePoints (pointCount);
        psimpl::util::scoped_array <psimpl::reference::Point> simplifiedPoints (pointCount);

        // convert polyline coords to points
        Iterator curr = first;
        for (int i=0; i<pointCount; i++) {
            polylinePoints [i].x = *curr++;
            polylinePoints [i].y = *curr++;
        }

        BENCHMARK(elapsed) {
            simplificationPointCount =
                psimpl::reference::poly_simplify (
                        setting.second [0].toDouble (),
                        polylinePoints.get (),
                        pointCount,
                        simplifiedPoints.get ());
        }

        // convert simplification points to soords
        simplificationSize = simplificationPointCount * DIM;
        for (int j=0; j<simplificationPointCount; j++) {
            simplification [j*2] = simplifiedPoints [j].x;
            simplification [j*2+1] = simplifiedPoints [j].y;
        }

        newStats = psimpl::compute_positional_error_statistics <DIM> (
                first,
                last,
                simplification.get (),
                simplification.get () + simplificationSize);

        std::cout << container << ",";
        QTextStream(stdout) << setting.first << ",";
        std::cout << simplificationPointCount << "," << (int) (((float) simplificationPointCount / (float) pointCount) * 100.f) << "%," << elapsed << ",";
        std::cout << newStats.mean << "," << newStats.std << "," << newStats.max << "," << newStats.sum << std::endl;
    }
}

// -----------------------------------------------------------------------------

//////template <class Iterator>
//////void Benchmark_Boost (const std::string& container, Iterator first, Iterator last, const Settings& settings)
//////{
//////    // benchmark all boost algorithms
//////    int polylineSize = std::distance (first, last);
//////    int pointCount = polylineSize / DIM;
//////    unsigned elapsed = 0;
//////
//////    for (const Setting& setting : settings)
//////    {
//////        if (setting.first != "simplify_douglas_peucker_boost") {
//////            continue;
//////        }
//////
//////        psimpl::util::scoped_array <double> simplification (polylineSize);
//////        int simplificationSize = 0;
//////        psimpl::error::statistics newStats;
//////
//////        std::cout << container << "," << setting.first.toStdString () << ",";
//////
//////        // convert polyline coords to points
//////        // benchmark
//////        // convert result from points to coords
//////
//////        newStats = psimpl::compute_positional_error_statistics <DIM> (first, last, simplification.get (), simplification.get () + simplificationSize);
//////
//////        std::cout << simplificationPointCount << "," << (int) (((float) simplificationPointCount / (float) pointCount) * 100.f) << "%," << elapsed << ",";
//////        std::cout << newStats.mean << "," << newStats.std << "," << newStats.max << "," << newStats.sum << std::endl;
//////    }
//////}

// -----------------------------------------------------------------------------

void Benchmark (
    const QString& polylinePath,
    const QString& settingsPath)
{
    QVector <double> polyline;
    Settings settings;

    // load polyline
    {
        QFile polylineFile (polylinePath);
        if (!polylineFile.open (QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream(stderr) << "unable to read " << polylinePath << endl;
            return;
        }
        QTextStream in (&polylineFile);
        while (!in.atEnd ()) {
            const auto& line = in.readLine ();
            const auto& list = line.split (',', QString::SkipEmptyParts);
            if (list.size () < 2) {
                QTextStream(stderr) << "invalid line read from " << polylinePath << endl;
                return;
            }
            polyline.push_back (list [0].toDouble ());
            polyline.push_back (list [1].toDouble ());
        }
        std::cout << ", " << polyline.size () / DIM << " points";
    }
    // load settings
    {
        QFile settingsFile (settingsPath);
        if (!settingsFile.open (QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream(stderr) << "unable to read " << settingsPath << endl;
            return;
        }
        QTextStream in (&settingsFile);
        while (!in.atEnd ()) {
            const auto& line = in.readLine ();
            const auto& algo = line.section (',', 0, 0);
            const auto& params = line.section (',', 1).split (',', QString::SkipEmptyParts);
            if (0 == algo.size () || 0 == params.size ()) {
                QTextStream(stderr) << "invalid line read from " << settingsPath << endl;
                return;
            }
            settings.push_back (qMakePair (algo, params));
            if (algo == "simplify_douglas_peucker") {
                settings.push_back (qMakePair (QString ("simplify_douglas_peucker_reference"), params));
            }
            else if (algo == "simplify_douglas_peucker_classic") {
                settings.push_back (qMakePair (QString ("simplify_douglas_peucker_boost"), params));
            }
        }
        std::cout << ", " << settings.size () << " algorithms" << std::endl;
    }
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "cont,algo,size,ratio,ticks,error mean,error std,error max,error sum,ticks,error mean,error std,error max,error sum,diff" << std::endl;
    // test with double[]
    {
        const auto poly = polyline.constData ();
        Benchmark_Ran ("double []", poly, poly + polyline.size (), settings);
    }
    // test with std::vector <double>
    {
        std::vector <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark_Ran ("std::vector <double>", poly.begin (), poly.end (), settings);
    }
    // test with std::deque <double>
    {
        std::deque <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark_Ran ("std::deque <double>", poly.begin (), poly.end (), settings);
    }
    // test with std::list <double>
    {
        std::list <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark_Bid ("std::list <double>", poly.begin (), poly.end (), settings);
    }
    // test with reference::Point[]
    {
        const auto* poly = polyline.constData ();
        Benchmark_Ref ("reference::Point[]", poly, poly + polyline.size (), settings);
    }
    //////// test with boost
    //////{
    //////    std::list <double> poly (polyline.constBegin (), polyline.constEnd ());
    //////    Benchmark_Boost ("boost", poly.begin (), poly.end (), settings);
    //////}
}

// -----------------------------------------------------------------------------

int main (int argc, char* argv [])
{
    std::cout << "================================================================================" << std::endl;
    std::cout << "Starting psimpl benchmark" << std::endl;
    if (argc == 2) {
        repeat = QString (argv [1]).toUInt ();
        std::cout << "Setting repeat count to " << repeat << std::endl;
    }

    QDirIterator it (".", QStringList () << "*.algo", QDir::Files | QDir::Readable);
    std::cout << "Processing *.algo files" << std::endl;
    int num_processed = 0;
    while (it.hasNext ()) {
        num_processed++;
        it.next ();
        const auto& settings = it.fileInfo ();
        QFileInfo polyline (settings.dir (), settings.completeBaseName ().append (".poly"));
        if (polyline.exists () && polyline.isFile () && polyline.isReadable ()) {
            std::cout << "================================================================================" << std::endl;
            QTextStream(stdout) << num_processed << " benchmarking : " << settings.canonicalFilePath() ;
            Benchmark (polyline.absoluteFilePath (), settings.absoluteFilePath ());
        }
        else {
            QTextStream(stderr) << num_processed << " FAILED " << polyline.canonicalFilePath() << endl;
            QTextStream(stderr) << " specified in " << settings.canonicalFilePath() << endl;
        }
    }
    if (!num_processed) {
        std::cout << "WARNING - no *.algo files to process" << std::endl;
    }

    std::cout << "Completed" << std::endl
    << "================================================================================" << std::endl;

    return 0;
}
