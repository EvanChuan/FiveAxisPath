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

};
#endif