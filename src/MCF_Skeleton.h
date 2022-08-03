#pragma once
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include <fstream>

typedef CGAL::Simple_cartesian<double>                        Kernel;
typedef Kernel::Point_3                                       Point;
typedef CGAL::Surface_mesh<Point>                             Triangle_mesh;
//typedef CGAL::Polyhedron_3<Kernel>                            Polyhedron;

typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh> Skeletonization;
typedef Skeletonization::Skeleton                             Skeleton;
typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
typedef Skeleton::edge_descriptor                             Skeleton_edge;

using namespace std;

class MCF_Skeleton
{
public:
    void MCF_Skeleton_Fun(Skeletonization::Skeleton& skeleton, CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh>& mcs);
};