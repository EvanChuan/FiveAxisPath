#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include "Function.h"
#include "FillHole.h"
#include "RWfile.h"

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
          //std::cout << "cutplan_vector : X" << unit_it_v[0] << " Y"  << unit_it_v[1] << " Z"<< unit_it_v[2] << std::endl;
          preP = {it[0][0],it[0][1],it[0][2]};
          // D = -(ax+by+cz)
          double D = (unit_it_v[0]*it[0][0]+unit_it_v[1]*it[0][1]+unit_it_v[2]*it[0][2])*(-1);
          //std::cout << "D : " << D << std::endl;
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

std::vector<double> Function::Normal_Vector(std::vector<double>& v1, std::vector<double>& v2){
  std::vector<double> NV;
  double N_x = v1[1] * v2[2] - v1[2] * v2[1];
  NV.push_back(N_x);
  double N_y = v1[2] * v2[0] - v1[0] * v2[2];
  NV.push_back(N_y);
  double N_z = v1[0] * v2[1] - v1[1] * v2[0];
  NV.push_back(N_z);
  return NV;
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
  //std::cout << "Run plane_cut~" << std::endl;
  Plane3 pl;
  
  for(std::vector<std::vector<double>>::iterator it = input.begin(); it != input.end(); ++it)
  {
    pl = Plane3(it[0][0], it[0][1], it[0][2], it[0][3]);
    std::cout << "cutplan_vector : X" << it[0][0] << " Y"  << it[0][1] << " Z"<< it[0][2] << " D"<< it[0][3] << std::endl;
    PlaneCutter pc;
    Polyhedron _;
    bool res = pc.cut_and_fill(p, _, pl);  // cut reslut will store in poly_right and poly_left is remaining
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

void Function::PolytoTri(std::vector<std::vector<double>>& newpoly, vector<Tri>& tri){
  int v_num = newpoly[0][0];
  int f_num = newpoly[0][1];
  //vector<Tri> tri;  // 由外面做宣告設定
  for(int i=v_num+1;i<=v_num+f_num;i++){  // handle each poly face to tri
    static Tri triP;
    int p1ID = newpoly[i][1]+1; // skip the first line of v_num and f_num
    int p2ID = newpoly[i][2]+1;
    int p3ID = newpoly[i][3]+1;
    triP.P1_x = newpoly[p1ID][0];
    triP.P1_y = newpoly[p1ID][1];
    triP.P1_z = newpoly[p1ID][2];
    triP.P2_x = newpoly[p2ID][0];
    triP.P2_y = newpoly[p2ID][1];
    triP.P2_z = newpoly[p2ID][2];
    triP.P3_x = newpoly[p3ID][0];
    triP.P3_y = newpoly[p3ID][1];
    triP.P3_z = newpoly[p3ID][2];
    std::vector<double> v1, v2;
    v1.push_back(triP.P2_x-triP.P1_x);
    v1.push_back(triP.P2_y-triP.P1_y);
    v1.push_back(triP.P2_z-triP.P1_z);
    v2.push_back(triP.P3_x-triP.P1_x);
    v2.push_back(triP.P3_y-triP.P1_y);
    v2.push_back(triP.P3_z-triP.P1_z);
    std::vector<double> NV = Normal_Vector(v1,v2);
    std::vector<double> NNV = Normal(NV);
    triP.n_x = NNV[0];
    triP.n_y = NNV[1];
    triP.n_z = NNV[2];

    tri.push_back(triP);
  }
  return;
}

vector< vector<Tri> > Function::TransfertoTriMesh(Polyhedron& tmesh,vector<Polyhedron>& cut_res, std::vector<std::vector<double>>& input){
  vector< vector<Tri> > STLdata;
  int count = 0;
  // the first part is store in tmesh
  vector<Tri> PB;
  static Tri triP;
  triP.n_x = 0;
  triP.n_y = 0;
  triP.n_z = 1;
  triP.n_d = 0;
  PB.push_back(triP); // store cut plan data first
  //std::cout << count << std::endl;
  std::vector<std::vector<double>> new_poly_double= RWfile::splitString(get_poly(tmesh));
  PolytoTri(new_poly_double,PB);
  STLdata.push_back(PB);
  PB.clear();
  count++;

  // the second and after part
  int len = cut_res.size();
  for(std::vector<std::vector<double>>::iterator it = input.end()-1; it != input.begin()-1; it--){
    static Tri triP;
    triP.n_x = it[0][0];
    triP.n_y = it[0][1];
    triP.n_z = it[0][2];
    triP.n_d = it[0][3];
    PB.push_back(triP);  // store cut plan data first
    //std::cout << count << std::endl;
    int ID = len - count; 
    std::vector<std::vector<double>> new_poly_double= RWfile::splitString(get_poly(cut_res[ID]));
    PolytoTri(new_poly_double,PB);
    STLdata.push_back(PB);
    count++;
    PB.clear();
  }
  return STLdata;
}

vector<Tri> Function::PolytoTri_for_Print(std::vector<std::vector<double>>& newpoly){
  int v_num = newpoly[0][0];
  int f_num = newpoly[0][1];
  vector<Tri> tri;
  for(int i=v_num+1;i<=v_num+f_num;i++){  // handle each poly face to tri
    static Tri triP;
    int p1ID = newpoly[i][1]+1; // skip the first line of v_num and f_num
    int p2ID = newpoly[i][2]+1;
    int p3ID = newpoly[i][3]+1;
    triP.P1_x = newpoly[p1ID][0];
    triP.P1_y = newpoly[p1ID][1];
    triP.P1_z = newpoly[p1ID][2];
    triP.P2_x = newpoly[p2ID][0];
    triP.P2_y = newpoly[p2ID][1];
    triP.P2_z = newpoly[p2ID][2];
    triP.P3_x = newpoly[p3ID][0];
    triP.P3_y = newpoly[p3ID][1];
    triP.P3_z = newpoly[p3ID][2];
    std::vector<double> v1, v2;
    v1.push_back(triP.P2_x-triP.P1_x);
    v1.push_back(triP.P2_y-triP.P1_y);
    v1.push_back(triP.P2_z-triP.P1_z);
    v2.push_back(triP.P3_x-triP.P1_x);
    v2.push_back(triP.P3_y-triP.P1_y);
    v2.push_back(triP.P3_z-triP.P1_z);
    std::vector<double> NV = Normal_Vector(v1,v2);
    std::vector<double> NNV = Normal(NV);
    triP.n_x = NNV[0];
    triP.n_y = NNV[1];
    triP.n_z = NNV[2];

    tri.push_back(triP);
  }
  return tri;
}