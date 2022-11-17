#include <fstream>
#include <iostream>
#include <vector>
#include <CGAL/Bbox_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Min_sphere_d.h>
#include <CGAL/Min_sphere_annulus_d_traits_3.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Polyhedron_3<K>                              Polyhedron;
typedef K::Vector_3                                           Vector3;
typedef Polyhedron::Facet_handle                         Facet_handle;
typedef Polyhedron::Halfedge_handle                   Halfedge_handle;
typedef CGAL::Bbox_3                                             Bbox;
typedef CGAL::Min_sphere_annulus_d_traits_3<K>                 Traits;
typedef CGAL::Min_sphere_d<Traits>                          MinSphere;
typedef K::Point_3                                             Point3;
Polyhedron poly;
Vector3 facetNormal;
Vector3 facetCentroid;

Vector3 get_facet_centroid(const Facet_handle&);
std::vector<float> initialize_supports(Polyhedron&);

CGAL::HalfedgeDS_in_place_list_face
<CGAL::I_Polyhedron_facet
<CGAL::HalfedgeDS_face_base
<CGAL::HalfedgeDS_list_types
<CGAL::Epick,CGAL::I_Polyhedron_derived_items_3<CGAL::Polyhedron_items_3>, std::__1::allocator<int> >, CGAL::Boolean_tag<true>,CGAL::Plane_3<CGAL::Epick> > > >

int reset(const std::string& meshfile) {
  poly.clear();
  std::ifstream f(meshfile);
  f >> poly;
  f.close();
  if (poly.is_valid()) {
    if (!poly.is_closed()) {
      unsigned int nb_holes = 0;
      for (Halfedge_handle h : halfedges(poly)) {    // 用指標形式在std::vector<Halfedge_iterator> halfedges中進行
        if (h->is_border()) {     // 當宣告為指標型態時Halfedge_handle h，就使用箭頭(->)來存取Class中的成員
          std::vector<Facet_handle> patch_facets;
          CGAL::Polygon_mesh_processing::triangulate_hole(
              poly, h, std::back_inserter(patch_facets),
              CGAL::Polygon_mesh_processing::parameters::use_delaunay_triangulation(true));

          ++nb_holes;
        }
      }
    }
  }

  //rPlatform = MeshCutEval::find_suitable_platform(poly);

  std::vector<float> initResults = initialize_supports(poly);
  Bbox bbox_ = bbox_3(poly.points_begin(), poly.points_end());
  MinSphere bsphere = MinSphere(poly.points_begin(), poly.points_end());

  float initVolume = initResults[0];
  float initArea = initResults[1];
  float initRiskyArea = initResults[2];

  std::cout << initResults[0] << " " << initResults[1] << " " << initResults[2] << std::endl;
  return 0;
  //return array_t<float>({1});
  /*
  if (!first_init_) {
    init_features_ = render();
    first_init_ = true;
  }
  return init_features_;
  */
}

Vector3 get_facet_centroid(const Facet_handle& fh) {
    Halfedge_handle he = fh->halfedge();
    Point3 &p1 = he->prev()->vertex()->point();
    Point3 &p2 = he->vertex()->point();
    Point3 &p3 = he->next()->vertex()->point();
    return {(p1.x() + p2.x() + p3.x()) / 3.0, (p1.y() + p2.y() + p3.y()) / 3.0,
            (p1.z() + p2.z() + p3.z()) / 3.0};
}

/**
 * \brief This function will initialize the normals, areas and whether needs support of a face
 * \param
 */
std::vector<float> initialize_supports(Polyhedron& poly_) {
    std::vector<float> init_results;
    float sumRiskyArea = 0.0, sumVolume = 0.0, sumArea = 0.0;
    sumVolume = CGAL::Polygon_mesh_processing::volume(poly_);

    // TODO: make this function static
    Vector3 cut(0, 1, 0);

    Debug("base plane eps " << base_plane_eps << std::endl);

    // Initialize normals, supported and area
    for (auto fit = poly_.facets_begin(); fit != poly_.facets_end(); ++fit) 
    {
        fit->facetCentroid = get_facet_centroid(fit);
        fit->facetNormal = CGAL::Polygon_mesh_processing::compute_face_normal(fit, poly_);
        fit->supported() = is_supported(fit->facetNormal, cut);

        // Compute the area of face
        Halfedge_handle he = fit->halfedge();
        const auto a = CGAL::sqrt(CGAL::squared_area(he->prev()->vertex()->point(),
                                                    he->vertex()->point(),
                                                    he->next()->vertex()->point()));
        fit->set_face_area(a);
    }

    for (auto vit = poly_.vertices_begin(); vit != poly_.vertices_end(); ++vit) {
        vit->vertexNormal = CGAL::Polygon_mesh_processing::compute_vertex_normal(vit, poly_);
    }

    // For sup volume and area
    for (auto &fit : faces(poly_)) {
        // Step 0: Compute sum area
        sumArea += fit->area();

        // Step1: Virtual supported Case I: Lower than the base plane
        Point3 cp(fit->facetCentroid.x(), fit->facetCentroid.y(), fit->facetCentroid.z());

        // TODO: how to handle virtual supported
        //        if (base_plane_eps.has_on_negative_side(cp)) {
        //            fit->virtual_supported() = true;
        //            continue;
        //        }

        // Step 2: Compute risky area and volume
        if (!fit->supported()) {
            sumRiskyArea += fit->area();
            Debug(fit->area() << " " << fit->proj_area() << std::endl);
        }
    }

    std::cout << "Sum area = " << sumArea  << " or " << CGAL::Polygon_mesh_processing::area(poly_) << std::endl;
    std::cout <<"Sum risky area = " << sumRiskyArea << std::endl;

    init_results.push_back(sumVolume);
    init_results.push_back(sumArea);
    init_results.push_back(sumRiskyArea);
    return init_results;
}


int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cout << "usage: ./main <input_file>" << std::endl;
    return 0;
  }

  // read STL file
  int data = reset(argv[1]);

  return 0;
}
