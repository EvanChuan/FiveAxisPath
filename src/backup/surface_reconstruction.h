#ifndef surface_reconstruction
#define surface_reconstruction

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/facets_in_complex_2_to_triangle_mesh.h>
#include <CGAL/Poisson_reconstruction_function.h>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <boost/iterator/transform_iterator.hpp>
#include "CustomisedPolyhedron.h"

typedef std::pair<Point_3, Vector3> Point_with_normal;
typedef CGAL::First_of_pair_property_map<Point_with_normal> Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;
typedef std::vector<Point_with_normal> PointList;
typedef CGAL::Poisson_reconstruction_function<Kernel> Poisson_reconstruction_function;
typedef CGAL::Surface_mesh_default_triangulation_3 STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
typedef CGAL::Implicit_surface_3<Kernel, Poisson_reconstruction_function> Surface_3;

class surface_RC
{
public:
   static void reconstruction(std::string& input,Polyhedron& tmesh);
};

#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/fair.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>

typedef Polyhedron::Vertex_handle                           Vertex_handle;
class polygon_refine
{
public:
   static void extract_k_ring(Vertex_handle v,int k,std::vector<Vertex_handle>& qv);
   static void mesh_refine(Polyhedron& poly);
};
#endif //surface_reconstruction