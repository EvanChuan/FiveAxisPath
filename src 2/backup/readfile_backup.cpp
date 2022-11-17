#include <fstream>
#include <iostream>
#include <vector>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/STL.h>
#include <CGAL/Polygon_mesh_processing.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include "CustomisedPolyhedron.h"
#include "MeshCutEval.h"
#include "GeometryTools.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>


namespace PMP = CGAL::Polygon_mesh_processing;
static std::vector<double> initialize_supports(Polyhedron &poly_);
static bool is_supported(Vector3 &f_normal, const Vector3 &dir);

typedef CGAL::Simple_cartesian<double> KS;
typedef KS::Point_2                 Point_2;
typedef KS::Point_3                 Point_3;

int readfile(const std::string& meshfile)
{
  std::string stl_file_name;
  //std::cout << "opening file\n";
  stl_file_name = meshfile;

  Polyhedron poly,poly_Partition;   // type of Polyhedron is defined in CustomisedPolyherdron.h

  std::ifstream stl_file(stl_file_name, std::ifstream::in);   // input the file
  std::vector<CGAL::cpp11::array<double, 3>> points;
  std::vector<CGAL::cpp11::array<int, 3>> triangles;

  //std::cout << "reading file\n";
  CGAL::IO::read_STL(stl_file, points, triangles);   // read STL file and store in points and triangles

  //std::cout << "closing file \n";
  stl_file.close();

  std::cout << "processing mesh\n";
  int spoints_nb = points.size();
  std::cout << "number of points in soup : " << spoints_nb << "\n";
  int poly_nb = triangles.size();
  std::cout << "number of polys in soup : " << poly_nb << "\n";
  CGAL::Polygon_mesh_processing::orient_polygon_soup(points, triangles);
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, triangles, poly_Partition);
  int vertices_nb = poly_Partition.size_of_facets();
  std::cout << "number of facets : " << vertices_nb << "\n";
  int points_nb = points.size();
  std::cout << "number of points : " << points_nb << "\n";
  int triangles_nb = triangles.size();
  std::cout << "number of triangles : " << triangles_nb << "\n";

  std::vector<Point_3> points_3;
  std::vector<CGAL::cpp11::array<double, 3>>::iterator it;
  for (it = points.begin() ; it != points.end(); ++it) {
      CGAL::cpp11::array<double, 3> a = *it;
      Point_3 point = Point_3(a[0], a[1], a[2]);
      points_3.push_back(point);
  }

  KS::Iso_cuboid_3 c3 = CGAL::bounding_box(points_3.begin(), points_3.end());
  for (int i = 0; i < 8; i++) {
      Point_3 vert = c3.vertex(i);
      std::cout << vert.hx() << ", " << vert.hy() << ", " << vert.hz() << "\n";
  }

  return 0;
}
/*

typedef CGAL::Surface_mesh<Point3>   Surface_mesh;   
typedef boost::graph_traits<Surface_mesh>::vertex_descriptor      vertex_descriptor;
typedef boost::graph_traits<Surface_mesh>::face_descriptor        face_descriptor;

int compute_normal(const std::string& meshfile)
{
  std::string stl_file_name = meshfile;
  Surface_mesh mesh;
  if(!PMP::IO::read_polygon_mesh(stl_file_name, mesh))
  {
    std::cerr << "Invalid input." << std::endl;
    return 1;
  }

  auto vnormals = mesh.add_property_map<vertex_descriptor, Vector3>("v:normals", CGAL::NULL_VECTOR).first;
  auto fnormals = mesh.add_property_map<face_descriptor, Vector3>("f:normals", CGAL::NULL_VECTOR).first;
  PMP::compute_normals(mesh, vnormals, fnormals);

  
  std::cout << "Vertex normals :" << std::endl;
  for(vertex_descriptor vd: vertices(mesh))
    std::cout << vnormals[vd] << std::endl;
  

  std::cout << "Face normals :" << std::endl;
  for(face_descriptor fd: faces(mesh))
    std::cout << fnormals[fd] << std::endl;
  
  return 0;
}

int compute_support(const std::string& meshfile)
{
  Polyhedron poly;
  std::vector<double> initResults;

  poly.clear();
  std::ifstream f(meshfile);
  f >> poly;
  f.close();
  initResults = initialize_supports(poly);
  std::cout << initResults[0] << " " << initResults[1] << " " << initResults[2] << std::endl;

  return 0;
}

*/

typedef CGAL::Simple_cartesian<double>                        Kernel;
typedef Kernel::Point_3                                       Point;
typedef CGAL::Polyhedron_3<Kernel>                            Polyhedron_S;
typedef boost::graph_traits<Polyhedron_S>::vertex_descriptor    vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron_S> Skeletonization;
typedef Skeletonization::Skeleton                             Skeleton;
typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
typedef Skeleton::edge_descriptor                             Skeleton_edge;
//only needed for the display of the skeleton as maximal polylines
struct Display_polylines{
  const Skeleton& skeleton;
  std::ofstream& out;
  int polyline_size;
  std::stringstream sstr;
  Display_polylines(const Skeleton& skeleton, std::ofstream& out)
    : skeleton(skeleton), out(out)
  {}
  void start_new_polyline(){
    polyline_size=0;
    sstr.str("");
    sstr.clear();
  }
  void add_node(Skeleton_vertex v){
    ++polyline_size;
    sstr << " " << skeleton[v].point;
  }
  void end_polyline()
  {
    out << polyline_size << sstr.str() << "\n";
  }
};

// This example extracts a medially centered skeleton from a given mesh.
int skeleton(const std::string& meshfile)
{
  std::ifstream input(meshfile);
  Polyhedron_S tmesh;
  input >> tmesh;
  if (!CGAL::is_triangle_mesh(tmesh))
  {
    std::cout << "Input geometry is not triangulated." << std::endl;
    return EXIT_FAILURE;
  }
  Skeleton skeleton;
  CGAL::extract_mean_curvature_flow_skeleton(tmesh, skeleton);
  std::cout << "Number of vertices of the skeleton: " << boost::num_vertices(skeleton) << "\n";
  std::cout << "Number of edges of the skeleton: " << boost::num_edges(skeleton) << "\n";
  // Output all the edges of the skeleton.
  std::ofstream output("skel-poly.polylines.txt");
  Display_polylines display(skeleton,output);
  CGAL::split_graph_into_polylines(skeleton, display);
  output.close();
  // Output skeleton points and the corresponding surface points
  output.open("correspondance-poly.polylines.txt");
  for(Skeleton_vertex v : CGAL::make_range(vertices(skeleton)))
    for(vertex_descriptor vd : skeleton[v].vertices)
      output << "2 " << skeleton[v].point << " "
                     << get(CGAL::vertex_point, tmesh, vd)  << "\n";
  return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cout << "usage: ./main <input_file>" << std::endl;
    return 0;
  }

  // read STL file
  //readfile(argv[1]);
  //compute_normal(argv[1]);
  //compute_support(argv[1]);
  skeleton(argv[1]);

  return 0;
}
