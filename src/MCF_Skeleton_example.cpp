#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>

#include <fstream>
#include "MCF_Skeleton.h"

void MCF_Skeleton::MCF_Skeleton_Fun(Skeletonization::Skeleton& skeleton,
                 CGAL::Mean_curvature_flow_skeletonization<Polyhedron>& mcs)
{
  std::cout << "Start Skeletonize!" << "\n";
  // 1. Contract the mesh by mean curvature flow.
  mcs.contract_geometry();
  // 2. Collapse short edges and split bad triangles.
  mcs.collapse_edges();
  mcs.split_faces();
  // 3. Fix degenerate vertices.
  mcs.detect_degeneracies();
  // Perform the above three steps in one iteration.
  mcs.contract();
  // Iteratively apply step 1 to 3 until convergence.
  mcs.contract_until_convergence();
  // Convert the contracted mesh into a curve skeleton and
  // get the correspondent surface points
  mcs.convert_to_skeleton(skeleton);

  //std::cout << "Number of vertices of the skeleton: " << boost::num_vertices(skeleton) << "\n";
  //std::cout << "Number of edges of the skeleton: " << boost::num_edges(skeleton) << "\n";
  /*
  // Output all the edges of the skeleton.
  std::ofstream output("skel-poly.polylines.txt");
  for(Skeleton_edge e : CGAL::make_range(edges(skeleton)))
  {
    const Point& s = skeleton[source(e, skeleton)].point;    // start point of edge
    const Point& t = skeleton[target(e, skeleton)].point;    // end point of edge
    output << "2 "<< s << " " << t << "\n";
  }
  output.close();
  */
  /*
  // Output skeleton points and the corresponding surface points
  output.open("correspondance-poly.polylines.txt");
  for(Skeleton_vertex v : CGAL::make_range(vertices(skeleton)))
    for(vertex_descriptor vd : skeleton[v].vertices)
      output << "2 " << skeleton[v].point << "  " << get(CGAL::vertex_point, tmesh, vd)  << "\n";
  */

  return;
}

