#include <numeric>
#include <math.h>
#include "Slicing.h"
#define MY_PI 3.14159265358979323846

vector<vector<Slicer::slice>> Slicer::subpart_slicing(vector<vector<Tri>>& trianglemesh, double layerheight){
    vector<vector<Slicer::slice>> pregcode_data;

    // each subpart is processd separately and then stored in the pregcode vector
    //for(int i=0;i<trianglemesh.size();i++){
    for(int i=0;i<2;i++){
        vector<Tri> process_tri = trianglemesh[i];
        Slicer::plane pl;
        double arc_A, arc_C;
        calculate_rotateangle(process_tri,pl,arc_A,arc_C);
        
        /*
        pair<double,double> bounds = findBoundaries(process_tri);
        int layernum = round((bounds.second - bounds.first)/layerheight);
        vector<double> slice_layer; // store each slicing layer height
        for(int i=0;i<layernum;i++){
            if(i==0){
                //std::cout << (layerheight/2) << std::endl;
                slice_layer.push_back((layerheight/2));
            }
            else{
                //std::cout << (layerheight/2)+layerheight*i << std::endl;
                slice_layer.push_back((layerheight/2)+layerheight*i);
            }
        }
        
        vector<Slicer::slice> subpart_res;
        for(int i=0; i<slice_layer.size(); i++){
            bool IsSurface = false;
            Slicer::slice sl;
            sl.z_value = slice_layer[i];
            for(int j=0;j<process_tri.size();j++){
                // calculate intersect points of line and plan
                //point1 = intersectPoint()
            }
            
        }

        pregcode_data.push_back(subpart_res);
        */
    }
    
    return pregcode_data;
}

pair<double,double> Slicer::findBoundaries(vector<Tri>& triangles){
    pair<double,double> bounds;
    double bottomZ = 500;
    double topZ = -500;
    //std::cout << "START Compare!" << std::endl;
    for(int i = 1; i < triangles.size(); i++){
        double maximum = max({triangles[i].P1_z, triangles[i].P2_z, triangles[i].P3_z});
        double minimum = min({triangles[i].P1_z, triangles[i].P2_z, triangles[i].P3_z});

        if(maximum > topZ){topZ = maximum;}
        if(minimum < bottomZ){bottomZ = minimum;}
    }
    //std::cout << "maximum = " << topZ << std::endl;
    //std::cout << "minimum = " << bottomZ << std::endl;
    bounds = make_pair(bottomZ,topZ);
    return bounds;
} 

void Slicer::calculate_rotateangle(vector<Tri>& triangles,Slicer::plane& pl,double& arc_A,double& arc_C){
    vector<double> normal = {0,0,1};
    vector<double> cutPlane_vector ={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};
    double inner_dot = inner_product(cutPlane_vector.begin(), cutPlane_vector.end(), normal.begin(), 0.0000);

    if(inner_dot==1){
        // basic subpart
        arc_A = 0;
        arc_C = 0;
        pl.n_x = triangles[0].n_x;
        pl.n_y = triangles[0].n_y;
        pl.n_z = triangles[0].n_z;
        pl.n_d = triangles[0].n_d;
    }
    else{  // Not a non-basic subsection that needs to be rotated
        double original_cutPlane_vector[3][1]={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};
        std::cout << "original_cutPlane_vector : X" << original_cutPlane_vector[0][0];
        std::cout << " Y" << original_cutPlane_vector[1][0];
        std::cout << " Z" << original_cutPlane_vector[2][0] << std::endl;
        
        double arc_C = get_angle(triangles[0].n_y,triangles[0].n_x);   // the result unit is in radius
        std::cout << "angle of C in radius:" << arc_C << std::endl;
        double rot_C[3][3]={ {cos(arc_C),(-1)*sin(arc_C),0},
                             {sin(arc_C),cos(arc_C),0},
                             {0,0,1} };
        double AfterC_cutPlane_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        for(int m=0; m<3; m++){
            for(int n=0; n<1; n++){
                for(int k=0; k<3; k++){
                    AfterC_cutPlane_vector[m][n]+=rot_C[m][k]*original_cutPlane_vector[k][n];
                }
            }
        }
        /*
        std::cout << "Print out the rotation matrix." << std::endl;
        for(int m=0; m<3; m++){
            for(int n=0; n<3; n++){
                std::cout << rot_C[m][n] << "\t";
            }
            std::cout << "\n";
        }
        */
        std::cout << "AfterC_cutPlane_vector : X" << AfterC_cutPlane_vector[0][0];
        std::cout << " Y" << AfterC_cutPlane_vector[1][0];
        std::cout << " Z" << AfterC_cutPlane_vector[2][0] << std::endl;

        double arc_A = get_angle(AfterC_cutPlane_vector[2][0],AfterC_cutPlane_vector[1][0]);   // the result unit is in radius    
        std::cout << "angle of A in radius:" << arc_A << std::endl;

        double rot_A[3][3]={ {1,0,0},
                             {0,cos(arc_A),(-1)*sin(arc_A)},
                             {0,sin(arc_A),cos(arc_A)}};
        double print_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        for(int m=0; m<3; m++){
            for(int n=0; n<1; n++){
                for(int k=0; k<3; k++){
                    print_vector[m][n]+=rot_A[m][k]*AfterC_cutPlane_vector[k][n];
                }
            }
        }
        std::cout << "print_vector : X" << print_vector[0][0];
        std::cout << " Y" << print_vector[1][0];
        std::cout << " Z" << print_vector[2][0] << std::endl;
    }

    return;
}

double Slicer::get_angle(double a, double b){
    // using actan
    double arc=atan(a/b);
    double deg=arc*180/MY_PI;
    if(a*b<0){
        deg = (-1)*(90+deg);
    }
    else{
        deg = (90-deg);
    }
    std:: cout << "angle in degree:" << deg << std::endl;
    deg=deg*(MY_PI/180);
    /*
    // using acsin
    double scal_b = abs(b);
    double scal = sqrt(pow(a,2)+pow(b,2));
    double angle = asin(scal_b/scal);  // the result unit is in radius
    double deg = angle*180/3.14159;

    // need add addtion conditon code
    //->>
    */
    return deg;
}