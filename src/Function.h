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

    static bool compareZ(Skel_Points p1, Skel_Points p2);
    void Sorting(std::vector<Function::Skel_Points> points);
    std::vector<Function::Skel_Points> ExtractSkelPoints(Skeletonization::Skeleton& skeleton);
};