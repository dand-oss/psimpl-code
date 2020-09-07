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

#ifndef PSIMPL_REFERENCE_H
#define PSIMPL_REFERENCE_H

#include <stack>

namespace psimpl {
    namespace reference {
        // Contains a minimal modified version of the Douglas-Peucker recursive simplification
        // routine as available from www.softsurfer.com. Modifications include:
        // - Removed recursion by using a stack
        // - Added minimal Point, Vector, and Segment classes as needed by the algorithm
        // - Replaced all occurences of float with double


        // minimal point class
        class Point {
        public:
            Point () :
                x (0.f),
                y (0.f)
            {}

            Point (double x, double y) :
                x (x),
                y (y)
            {}

            double x, y;
        };

        // reuse the point class for a vector
        typedef Point Vector;

        // operators as needed by the algorithm
        Point operator+ (const Point& P, const Vector& V) {
            return Point (P.x+V.x, P.y+V.y);
        }

        Vector operator- (const Point& P0, const Point& P1) {
            return Vector (P0.x-P1.x, P0.y-P1.y);
        }

        Vector operator* (double s, const Vector& V) {
            return Vector (s*V.x, s*V.y);
        }

        // minimal segment class
        class Segment {
        public:
            Segment (const Point& P0, const Point& P1) :
                P0(P0),
                P1(P1)
            {}

            Point P0;
            Point P1;
        };

        // define the stack and sub poly that are used to replace the original recusion
        typedef std::pair< int, int > SubPoly;
        typedef std::stack< SubPoly > Stack;


        // Copyright 2002, softSurfer (www.softsurfer.com)
        // This code may be freely used and modified for any purpose
        // providing that this copyright notice is included with it.
        // SoftSurfer makes no warranty for this code, and cannot be held
        // liable for any real or imagined damage resulting from its use.
        // Users of this code must verify correctness for their application.

        // Assume that classes are already given for the objects:
        //    Point and Vector with
        //        coordinates {double x, y, z;}    // as many as are needed
        //        operators for:
        //            == to test equality
        //            != to test inequality
        //            (Vector)0 = (0,0,0)         (null vector)
        //            Point  = Point ± Vector
        //            Vector = Point - Point
        //            Vector = Vector ± Vector
        //            Vector = Scalar * Vector    (scalar product)
        //            Vector = Vector * Vector    (cross product)
        //    Segment with defining endpoints {Point P0, P1;}
        //===================================================================

        // dot product (3D) which allows vector operations in arguments
        #define __pdot(u,v)   ((u).x * (v).x + (u).y * (v).y)
        #define __pnorm2(v)   __pdot(v,v)        // norm2 = squared length of vector
        #define __pnorm(v)    sqrt(__norm2(v))   // norm = length of vector
        #define __pdist2(u,v) __pnorm2(u-v)      // distance squared = norm2 of difference
        #define __pdist(u,v)  __pnorm(u-v)       // distance = norm of difference

        // simplifyDP():
        //  This is the Douglas-Peucker recursive simplification routine
        //  It just marks vertices that are part of the simplified polyline
        //  for approximating the polyline subchain v[j] to v[k].
        //    Input:  tol = approximation tolerance
        //            v[] = polyline array of vertex points
        //            j,k = indices for the subchain v[j] to v[k]
        //    Output: mk[] = array of markers matching vertex array v[]
        void
        simplifyDP( Stack& stack, double tol, const std::vector<Point>& v, int j, int k, std::vector<int>& mk )
        {
            if (k <= j+1) { // there is nothing to simplify
                return;
            }

            // check for adequate approximation by segment S from v[j] to v[k]
            int     maxi = j;          // index of vertex farthest from S
            double   maxd2 = 0;        // distance squared of farthest vertex
            double   tol2 = tol * tol; // tolerance squared
            Segment S (v[j], v[k]);  // segment from v[j] to v[k]
            Vector  u = S.P1 - S.P0;   // segment direction vector
            double  cu = __pdot(u,u);     // segment length squared

            // test each vertex v[i] for max distance from S
            // compute using the Feb 2001 Algorithm's dist_Point_to_Segment()
            // Note: this works in any dimension (2D, 3D, ...)
            Vector  w;
            Point   Pb;                // base of perpendicular from v[i] to S
            double  b, cw, dv2;        // dv2 = distance v[i] to S squared

            for (int i=j+1; i<k; i++)
            {
                // compute distance squared
                w = v[i] - S.P0;
                cw = __pdot(w,u);
                if ( cw <= 0 )
                    dv2 = __pdist2(v[i], S.P0);
                else if ( cu <= cw )
                    dv2 = __pdist2(v[i], S.P1);
                else {
                    b = cw / cu;
                    Pb = S.P0 + b * u;
                    dv2 = __pdist2(v[i], Pb);
                }
                // test with current max distance squared
                if (dv2 <= maxd2)
                    continue;
                // v[i] is a new max vertex
                maxi = i;
                maxd2 = dv2;
            }
            if (maxd2 > tol2)        // error is worse than the tolerance
            {
                // split the polyline at the farthest vertex from S
                mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
                // recursively simplify the two subpolylines at v[maxi]
                stack.push( std::make_pair (j, maxi));
                stack.push( std::make_pair (maxi, k));
            }
            // else the approximation is OK, so ignore intermediate vertices
            return;
        }

        // poly_simplify():
        //    Input:  tol = approximation tolerance
        //            V[] = polyline array of vertex points
        //            n   = the number of points in V[]
        //    Output: sV[]= simplified polyline vertices (max is n)
        //    Return: m   = the number of points in sV[]
        int
        poly_simplify(double tol, Point* V, int n, Point* sV )
        {
            int    i, k, m, pv;            // misc counters
            double  tol2 = tol * tol;      // tolerance squared
            std::vector<Point> vt(n);      // vertex buffer
            std::vector<int> mk(n);        // marker buffer
            for (i=0; i<n; i++) {
                mk[i] = 0;
            }

            // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
            vt[0] = V[0];              // start at the beginning
            for (i=k=1, pv=0; i<n; i++) {
                if (__pdist2(V[i], V[pv]) < tol2) {
                    continue;
                }
                vt[k++] = V[i];
                pv = i;
            }
            if (pv < n-1) {
                vt[k++] = V[n-1];      // finish at the end
            }

            // STAGE 2.  Douglas-Peucker polyline simplification
            mk[0] = mk[k-1] = 1;                    // mark the first and last vertices
            Stack stack;                            // use a stack i.s.o. recursion (NEW)
            stack.push( std::make_pair( 0, k-1 ));  // add complete poly
            while (!stack.empty()) {
                SubPoly subPoly = stack.top();      // take a sub poly
                stack.pop();                        // and simplify it
                simplifyDP( stack, tol, vt, subPoly.first, subPoly.second, mk );
            }

            // copy marked vertices to the output simplified polyline
            for (i=m=0; i<k; i++) {
                if (mk[i]) {
                    sV[m++] = vt[i];
                }
            }
            return m;         // m vertices in simplified polyline
        }
        //===================================================================
    }
}

#endif // PSIMPL_REFERENCE_H
