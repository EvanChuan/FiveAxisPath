#include<iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "MCF_Skeleton.h"
#include "CustomisedPolyhedron.h"
#include "PlaneCut.h"

using namespace std;

class Function
{
public:
    struct Skel_Points {  // skel_points
        double X;
        double Y;
        double Z;
    };
    std::vector<Skel_Points> SK_points;

    static bool compareZ(std::vector<double> p1, std::vector<double> p2);
    void Sorting(std::vector<std::vector<double>>& points);
    std::vector<std::vector<double>> ExtractSkelPoints(Skeletonization::Skeleton& skeleton);
    
    static double angle(std::vector<double> v1,std::vector<double> v2);
    static std::vector<double> Normal(std::vector<double> v);
    std::vector<std::vector<double>> cutting_plan(std::vector<std::vector<double>>& points);
    
    bool set_poly(Polyhedron& p);
    bool plane_cut(Polyhedron& p,std::vector<std::vector<double>>& input);
    std::string get_poly(Polyhedron& poly);

    Polyhedron poly;
};