#include "surface_reconstruction.h"
#include "CustomisedPolyhedron.h"
// not used the function
void surface_RC::reconstruction(std::string& input,Polyhedron& tmesh)
{   std::cout << "reconstruction" << std::endl;
    // Poisson options
    FT sm_angle = 20.0; // Min triangle angle in degrees.
    FT sm_radius = 30; // Max triangle size w.r.t. point set average spacing.
    FT sm_distance = 0.375; // Surface Approximation error w.r.t. point set average spacing.
    // Reads the point set file in points[].
    // Note: read_points() requires an iterator over points + property maps to access each point's position and normal.
    PointList points;
    if(!CGAL::IO::read_points(CGAL::data_file_path(input), std::back_inserter(points),
                          CGAL::parameters::point_map(Point_map())
                                           .normal_map (Normal_map())))
    {
      std::cerr << "Error: cannot read file input file!" << std::endl;
      return;
    }
    std::cout << "point list len :" << points.size() << std::endl;
    std::cout << "AA" << std::endl;
    // Creates implicit function from the read points using the default solver.
    // Note: this method requires an iterator over points + property maps to access each point's position and normal.
    Poisson_reconstruction_function function(points.begin(), points.end(), Point_map(), Normal_map());
    std::cout << "BB" << std::endl;
    // Computes the Poisson indicator function f() at each vertex of the triangulation.
    if ( ! function.compute_implicit_function() )
      return;
    
    // Computes average spacing
    FT average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
      (points, 6 ,CGAL::parameters::point_map (Point_map()));
    std::cout << "CC" << std::endl;
    // Gets one point inside the implicit surface and computes implicit function bounding sphere radius.
    Point_3 inner_point = function.get_inner_point();
    Sphere_3 bsphere = function.bounding_sphere();
    FT radius = std::sqrt(bsphere.squared_radius());
    std::cout << "DD" << std::endl;
    // Defines the implicit surface: requires defining a conservative bounding sphere centered at inner point.
    FT sm_sphere_radius = 5.0 * radius;
    FT sm_dichotomy_error = sm_distance*average_spacing/1000.0; // Dichotomy error must be << sm_distance
    Surface_3 surface(function,
                      Sphere_3(inner_point,sm_sphere_radius*sm_sphere_radius),
                      sm_dichotomy_error/sm_sphere_radius);
    // Defines surface mesh generation criteria
    CGAL::Surface_mesh_default_criteria_3<STr> criteria(sm_angle,  // Min triangle angle (degrees)
                                                        sm_radius*average_spacing,  // Max triangle size
                                                        sm_distance*average_spacing); // Approximation error
    std::cout << "EE" << std::endl;
    // Generates surface mesh with manifold option
    STr tr; // 3D Delaunay triangulation for surface mesh generation
    C2t3 c2t3(tr); // 2D complex in 3D Delaunay triangulation
    std::cout << "FF" << std::endl;
    CGAL::make_surface_mesh(c2t3,                                 // reconstructed mesh
                            surface,                              // implicit surface
                            criteria,                             // meshing criteria
                            CGAL::Manifold_with_boundary_tag());  // require manifold mesh
    if(tr.number_of_vertices() == 0)
      return;
    std::cout << "gg" << std::endl;
    // saves reconstructed surface mesh to Polyhedron& tmesh
    CGAL::facets_in_complex_2_to_triangle_mesh(c2t3, tmesh);
    std::cout << "hh" << std::endl;
    // computes the approximation error of the reconstruction
    double max_dist =
      CGAL::Polygon_mesh_processing::approximate_max_distance_to_point_set
      (tmesh,
       CGAL::make_range (boost::make_transform_iterator
                         (points.begin(), CGAL::Property_map_to_unary_function<Point_map>()),
                         boost::make_transform_iterator
                         (points.end(), CGAL::Property_map_to_unary_function<Point_map>())),
       4000);
    std::cout << "Max distance to point_set: " << max_dist << std::endl;
    return;

}

// extract vertices which are at most k (inclusive) far from vertex v in the graph of edges
void polygon_refine::extract_k_ring(Vertex_handle v,int k,std::vector<Vertex_handle>& qv)
{
  std::map<Vertex_handle, int>  D;
  qv.push_back(v);
  D[v] = 0;
  std::size_t current_index = 0;
  int dist_v;
  while (current_index < qv.size() && (dist_v = D[qv[current_index]]) < k)
  {
    v = qv[current_index++];
    Polyhedron::Halfedge_around_vertex_circulator e(v->vertex_begin()), e_end(e);
    do {
      Vertex_handle new_v = e->opposite()->vertex();
      if (D.insert(std::make_pair(new_v, dist_v + 1)).second)
        qv.push_back(new_v);
    } while (++e != e_end);
  }
}

void polygon_refine::mesh_refine(Polyhedron& poly)
{ std::cout << "mesh_refine" << std::endl;
  std::vector<Polyhedron::Facet_handle>  new_facets;
  std::vector<Vertex_handle> new_vertices;
  CGAL::Polygon_mesh_processing::refine(poly, faces(poly),std::back_inserter(new_facets),std::back_inserter(new_vertices),CGAL::parameters::density_control_factor(2.));
  std::cout << "Refinement added " << new_vertices.size() << " vertices." << std::endl;
  
  Polyhedron::Vertex_iterator v = poly.vertices_begin();
  std::advance(v, 82/*e.g.*/);
  std::vector<Vertex_handle> region;
  extract_k_ring(v, 12/*e.g.*/, region);
  bool success = CGAL::Polygon_mesh_processing::fair(poly, region);
  std::cout << "Fairing : " << (success ? "succeeded" : "failed") << std::endl;

  return;
}