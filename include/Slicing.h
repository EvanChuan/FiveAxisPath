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
        double n_x, n_y, n_z, n_d;
        double P1_x,P1_y,P1_z;
        double P2_x,P2_y,P2_z;
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

    vector<vector<Slicer::slice>> subpart_slicing(vector<vector<Tri>>& trianglemesh, double layerheight);

    pair<double,double> findBoundaries(vector<Tri>& triangles); // find each subpart slicing (bottomZ,topZ)

    void calculate_rotateangle(vector<Tri>& triangles,Slicer::plane& pl,double& arc_A,double& arc_C);
    double get_angle(double a, double b);

};
#endif