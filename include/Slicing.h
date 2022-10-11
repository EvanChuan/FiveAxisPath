#ifndef SLICING_H
#define SLICING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Function.h"

using namespace std;

class Slicer
{
public:
    struct line{  // two end points and its vector
        vector<double> N = {0, 0, 0, 0};    // {x,y,z,d}
        vector<double> P1 = {0,0,0};        // {x,y,z}
        vector<double> P2 = {0,0,0};        // {x,y,z}
    };

    struct point{
        double x,y,z,a,c;

        bool operator <(const point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	    }
    };

    struct slice{
        double z_value;
        bool is_surface;
        vector<Slicer::point> countor;
        vector<Slicer::point> infill_part;
    };

    struct plane{
        double n_x, n_y, n_z, n_d;
    };

    vector<vector<Slicer::slice>> subpart_slicing(vector<vector<Tri>>& trianglemesh, vector<Function::Skel_Points>& plane_points,double layerheight);

    pair<double,double> findBoundaries(vector<Tri>& triangles); // find each subpart slicing (bottomZ,topZ)

    void calculate_rotateangle(vector<Tri>& triangles,double& arc_A,double& arc_C,double rotation[3][3]);
    double get_angle(double a, double b);
    //double get_angle_withtwovector(vector<double>& PN,vector<double>& cutPlane_vector);
    void deal_3by1Matrix(double A[3][3], double B[3][1], double Re[3][1]);
    void deal_3by3Matrix(double A[3][3], double B[3][3], double Re[3][3]);
    void update_Matrix(double A[3][3], double B[3][3]);
    void deal_translate_meshpoints(double A[3][3],vector<Tri>& current_tri,vector<Tri>& new_tri);
    void intersectPoint(line& L, double& S,vector<point>& intersectPoint_set);
    
    // Returns a list of points on the convex hull in counter-clockwise order.
    // Note: the last point in the returned list is the same as the first one.
    void convex_hull(vector<point>& P,vector<point>& re_P);
    // 3D cross product of OA and OB vectors, (i.e z-component of their "2D" cross product, but remember that it is not defined in "2D").
    // Returns a positive value, if OAB makes a counter-clockwise turn,
    // negative for clockwise turn, and zero if the points are collinear.
    double cross(const point &O, const point &A, const point &B);

};
#endif