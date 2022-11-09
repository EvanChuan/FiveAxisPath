#pragma once
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include <fstream>
#include "CustomisedPolyhedron.h"

using namespace std;

class MCF_Skeleton
{
public:
    void MCF_Skeleton_Fun(Skeletonization::Skeleton& skeleton, CGAL::Mean_curvature_flow_skeletonization<Polyhedron>& mcs);
};