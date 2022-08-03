#include<iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "MCF_Skeleton.h"

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
    void cutting_plan(std::vector<std::vector<double>>& points);
};