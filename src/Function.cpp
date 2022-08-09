#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include "Function.h"
#include "FillHole.h"

using namespace std;

std::vector<std::vector<double>> Function::cutting_plan(std::vector<std::vector<double>>& points){
  std::vector<double> printDIR = {0,0,1};
  std::vector<double> preP = {0,0,0};
  std::vector<std::vector<double>> P;
  for(std::vector<std::vector<double>>::iterator it = points.begin(); it != points.end(); ++it){
      std::vector<double> it_v = {it[0][0] - preP[0],it[0][1] - preP[1],it[0][2] - preP[2]};
      std::vector<double> unit_it_v = Normal(it_v);
      float tha = angle(unit_it_v,printDIR);
      if( tha > 30){
          //std::cout << "more than 30 " << std::endl;
          printDIR = unit_it_v;
          std::cout << "cutplan_vector : X" << unit_it_v[0] << " Y"  << unit_it_v[1] << " Z"<< unit_it_v[2] << std::endl;
          preP = {it[0][0],it[0][1],it[0][2]};
          // D = -(ax+by+cz)
          double D = (unit_it_v[0]*it[0][0]+unit_it_v[1]*it[0][1]+unit_it_v[2]*it[0][2])*(-1);
          std::cout << "D : " << D << std::endl;
          P.push_back({unit_it_v[0],unit_it_v[1],unit_it_v[2],D});
      }
	}
  return P;
}

double Function::angle(std::vector<double> v1,std::vector<double> v2){
    double inner = inner_product(v1.begin(), v1.end(), v2.begin(), 0.0000); // the fourth argument can seen as precision, https://stackoverflow.com/questions/12065041/zero-inner-product-when-using-stdinner-product
    double scal = sqrt(pow(v1[0],2)+pow(v1[1],2)+pow(v1[2],2))*sqrt(pow(v2[0],2)+pow(v2[1],2)+pow(v2[2],2));
    double cos_th = inner / scal;
    double tha = acos(cos_th)*180/3.14159;
    //std::cout << "angle : " << tha << std::endl;
    return tha;
}

std::vector<double> Function::Normal(std::vector<double> v){
    double len = sqrt(pow(v[0],2)+pow(v[1],2)+pow(v[2],2));
    std::vector<double> it_v = {v[0]/len,v[1]/len,v[2]/len};
    return it_v;
}

bool Function::set_poly(Polyhedron& p) {
  if (p.is_valid()) {
    if (!p.is_closed()) {  // 整修模型網目，確認網模資料完整, same as the part of reset
      unsigned int nb_holes = 0;
      for (Halfedge_handle h : halfedges(p)) {
        if (h->is_border()) {
          std::vector<Facet_handle> patch_facets;
          CGAL::Polygon_mesh_processing::triangulate_hole(
              p, h, std::back_inserter(patch_facets),
              CGAL::Polygon_mesh_processing::parameters::use_delaunay_triangulation(true));

          ++nb_holes;
        }
      }
    }
    //auto initResults =  MeshCutEval::initialize_supports(poly);
    //std::cout << initResults[0] << " " << initResults[1] << " " << initResults[2] << std::endl;

    return true;
  } else {
    return false;
  }
}

vector<Polyhedron> Function::plane_cut(Polyhedron& p,std::vector<std::vector<double>>& input) {
  std::cout << "Run plane_cut~" << std::endl;
  Plane3 pl;
  
  for(std::vector<std::vector<double>>::iterator it = input.begin(); it != input.end(); ++it)
  {
    pl = Plane3(it[0][0], it[0][1], it[0][2], it[0][3]);
    PlaneCutter pc;
    Polyhedron _;
    bool res = pc.cut_and_fill(p, _, pl);
    std::cout << "cut_and_fill result:" << res << std::endl;
    cut_res.push_back(_);
    //cut_res.push_back(p);
  }

  return cut_res;
}

std::string Function::get_poly(Polyhedron& poly) {
  std::stringstream ss;
  ss << std::setprecision(8) << poly;
  return ss.str();
}