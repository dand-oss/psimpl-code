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

#ifndef PSIMPL_HELPER
#define PSIMPL_HELPER


#include <vector>
#include <cmath>


namespace psimpl {
    namespace test
{
    static float float_epsilon = 0.001f;
    static double double_epsilon = 0.00001;

    /*!
        \brief Generates a straight line, one coordinate at a time

        example: (0,0) (1,0) (2,0) (3,0) (4,0) (5,0) etc
    */
    template <typename T, unsigned DIM>
    class StraightLine {
    public:
        StraightLine (T stepSize = 1) :
            mStepSize (stepSize),
            mCoord (0),
            mDimension (0)
        {}
        
        T operator () () {
            mDimension = mDimension % DIM;
            T value = 0;
            if (mDimension == 0) {
                value = mCoord;
                mCoord += mStepSize;
            }
            ++mDimension;
            return value;
        }
        
    private:
        T mStepSize;            //!< distance between points along x-axis
        T mCoord;               //!< x-axis value of the current point
        unsigned mDimension;    //!< dimension of the current point (x-axis = 0)
    };

    /*!
        \brief Generates a saw-tooth line, one coordinate at a time

        example: (0,0) (1,1) (2,0) (3,2) (4,0) (5,3) (6,0) etc
    */
    template <typename T, unsigned DIM>
    class SawToothLine {
    public:
        SawToothLine (T stepSize = 1) :
            mStepSize (stepSize),
            mCoord (0),
            mToothSize (0),
            mDimension (0),
            mToggle (true)
        {}
        
        T operator () () {
            mDimension = mDimension % DIM;
            T value = 0;
            if (mDimension == 0) {
                mToggle = !mToggle;
                value = mCoord;
                mCoord += mStepSize;
            }
            else if (mToggle && mDimension == 1) {
                mToothSize += 1;
                value = mToothSize;
            }
            ++mDimension;
            return value;
        }
        
    private:
        T mStepSize;            //!< distance between points along x-axis
        T mCoord;               //!< x-axis value of the current point
        T mToothSize;           //!< height of the tooth (y-axis), increases by one after each tooth
        unsigned mDimension;    //!< dimension of the current point (x-axis = 0)
        bool mToggle;           //!< indicates if the current point is a tooth
    };

    /*!
        \brief Generates a block-tooth line, one coordinate at a time

        example: (0,0) (1,0) (1,1) (2,1) (2,0) (3,0) (4,0) (4,1) (4,2) (5,2) etc
    */
    template <typename T, unsigned DIM>
    class SquareToothLine {
    public:
        SquareToothLine () :
            mToothSize (1),
            mCurrentSize (0),
            mDimension (0),
            mDirection (0)
        {
            for (unsigned d=0; d<DIM; ++d) {
                mPosition [d] = 0;
            }
        }
            
        T operator () () {
            mDimension = mDimension % DIM;
            mDirection = mDirection % 4;
            
            T value = mPosition [mDimension];

            if (mDirection == 0 || mDirection == 2) {   // foward
                if (mDimension == 0) {
                    mPosition [mDimension] += 1;
                }
            }
            else if (mDirection == 1) {                 // up
                if (mDimension == 1) {
                    mPosition [mDimension] += 1;
                }
            }
            else {                                      // down
                if (mDimension == 1) {
                    mPosition [mDimension] -= 1;
                }
            }
            ++mDimension;

            if (mDimension == DIM) {
                ++mCurrentSize;
                if (mCurrentSize == mToothSize) {
                    if (mDirection == 3) {                  // down
                        // end of tooth
                        ++mToothSize;
                    }
                    // change direction
                    mCurrentSize = 0;
                    ++mDirection;
                }
            }
            return value;
        }
            
    private:
        T mPosition [DIM];      //!< precomputed coordinates for each dimension
        T mToothSize;           //!< height of the tooth (y-axis), increases by one after each tooth
        T mCurrentSize;         //!< height of the current point (y-axis)
        unsigned mDimension;    //!< dimension of the current point (x-axis = 0)
        unsigned mDirection;    //!< direction of the current tooth (0,2=forward, 1=up, 3=down)
    };

    //! \brief exact compare of two values of the same type
    template <class T>
    inline bool CompareValue (T a, T b) {
        return a == b;
    }

    //! \brief compare of two float values using float_epsilon
    inline bool CompareValue (float a, float b) {
        return fabs (a - b) < float_epsilon;
    }

    //! \brief compare of two double values using double_epsilon
    inline bool CompareValue (double a, double b) {
        return fabs (a - b) < double_epsilon;
    }

    //! \brief compares each coordinate of two points
    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool ComparePoint (InputIterator1 p1, InputIterator2 p2) {
        for (unsigned d=0; d <DIM; ++d) {
            if (!CompareValue (*p1, *p2)) {
                return false;
            }
            ++p1;
            ++p2;
        }
        return true;
    }

    //! \brief compares only the end points of two polylines
    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool CompareEndPoints (InputIterator1 polyline_begin, InputIterator1 polyline_end, InputIterator2 simplification_begin, InputIterator1 simplification_end) {
        // compare first point
        if (!ComparePoint <DIM> (polyline_begin, simplification_begin)) {
            return false;
        }

        // compare last point
        std::advance (polyline_begin, std::distance (polyline_begin, polyline_end) - DIM);
        std::advance (simplification_begin, std::distance (simplification_begin, simplification_end) - DIM);

        return ComparePoint <DIM> (polyline_begin, simplification_begin);
    }

    //! \brief compares a polyline and its simplification, based on the given key indices
    template <unsigned DIM, class InputIterator1, class InputIterator2>
    bool ComparePoints(InputIterator1 polyline, InputIterator2 simplification, const std::vector<int>& keys) {
        for (size_t i=0; i<keys.size (); ++i) {
            InputIterator1 it = polyline;
            std::advance (it, keys [i] * DIM);
            if (!ComparePoint <DIM> (it, simplification)) {
                return false;
            }
            std::advance (simplification, DIM);
        }
        return true;
    }

}}

#endif // PSIMPL_HELPER
