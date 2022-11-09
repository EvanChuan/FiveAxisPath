#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "MCF_Skeleton.h"
#include "CustomisedPolyhedron.h"
#include "PlaneCut.h"

using namespace std;

typedef struct tri{
    double n_x, n_y, n_z, n_d;
    double P1_x,P1_y,P1_z;
    double P2_x,P2_y,P2_z;
    double P3_x,P3_y,P3_z;
}Tri;

class Function
{
public:
    struct Skel_Points {  // skel_points
        double X,Y,Z;
    };
    //std::vector<Skel_Points> SK_points;

    static bool compareZ(std::vector<double> p1, std::vector<double> p2);
    void Sorting(std::vector<std::vector<double>>& points);
    std::vector<std::vector<double>> ExtractSkelPoints(Skeletonization::Skeleton& skeleton);
    
    static double angle(std::vector<double> v1,std::vector<double> v2);
    static std::vector<double> Normal(std::vector<double> v);
    std::vector<double> OuterProduct(std::vector<double>& v1, std::vector<double>& v2);
    std::vector<std::vector<double>> cutting_plan(std::vector<std::vector<double>>& points,vector<Function::Skel_Points>& plane_points);
    
    bool set_poly(Polyhedron& p);
    //bool plane_cut(Polyhedron& p,std::vector<std::vector<double>>& input);
    vector<Polyhedron> plane_cut(Polyhedron& p,std::vector<std::vector<double>>& input);
    static std::string get_poly(Polyhedron& poly);

    void PolytoTri(std::vector<std::vector<double>>& newpoly,vector<Tri>& tri);
    vector<Tri> PolytoTri_for_Print(std::vector<std::vector<double>>& newpoly);
    vector< vector<Tri> > TransfertoTriMesh(Polyhedron& tmesh,vector<Polyhedron>& cut_res, std::vector<std::vector<double>>& input);

    Polyhedron poly;
    vector<Polyhedron> cut_res;

    void Show_Vector(double vector[3][1]);
    void Show_Matrix(double M[3][3]);
};
#endif