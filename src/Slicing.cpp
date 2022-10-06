#include <numeric>
#include <math.h>
#include "Slicing.h"
#include "RWfile.h"
#define MY_PI 3.14159265358979323846
Function FN;

vector<vector<Slicer::slice>> Slicer::subpart_slicing(vector<vector<Tri>>& trianglemesh, vector<Function::Skel_Points>& plane_points, double layerheight)
{
    vector<vector<Slicer::slice>> pregcode_data;        // Storing the result data of slicing and will be generated gcode
    vector<std::pair<double,double>> rotated_angle;     // Storing the rotational anglele
    double rotationM[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // Storing the pre rotation
    double afterRotate_plane_point[3][1] = {0,0,0};

    // each subpart is processd separately and then stored in the pregcode_data vector
    for(int i=0;i<trianglemesh.size();i++){
    //for(int i=0;i<2;i++){
        vector<Tri> current_tri = trianglemesh[i];
        Slicer::plane pl;
        double arc_A = 0, arc_C = 0;
        double rotationM_i[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // The rotation matrix is translate from i to i-1
        vector<double> Previous_normal = {current_tri[0].n_x,current_tri[0].n_y,current_tri[0].n_z};
        calculate_rotateangle(current_tri, arc_A, arc_C, rotationM_i,Previous_normal);
        deal_3by3Martix(rotationM,rotationM_i,rotationM);  // The rotation matrix is translate from i to base
        rotated_angle.push_back(make_pair(arc_A,arc_C));

        // tranlate the trianglemesh to preslicing position
        vector<Tri> new_tri;
        double plane_point[3][1] = {plane_points[i].X,plane_points[i].Y,plane_points[i].Z};
        if(arc_A!=0 || arc_C!=0){ // condition for not basic part
            deal_3by1Martix(rotationM, plane_point, afterRotate_plane_point);
            deal_translate_meshpoints(rotationM,current_tri,new_tri);
            std::string filename = "output_tran"+std::to_string(i);
            RWfile::Write_GCode(filename,new_tri);  // write the points after rotation
            std::string filename1 = "output_org"+std::to_string(i);
            RWfile::Write_GCode(filename1,current_tri);
        }
        //std::string filename = "output_org"+std::to_string(i);
        //RWfile::Write_GCode(filename,current_tri);
        
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

void Slicer::calculate_rotateangle(vector<Tri>& triangles,double& arc_A,double& arc_C,double rotation[3][3],vector<double>& PN)
{
    vector<double> normal = PN;
    vector<double> cutPlane_vector ={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};
    double inner_dot = inner_product(cutPlane_vector.begin(), cutPlane_vector.end(), normal.begin(), 0.0000);

    if(inner_dot==1){
        // basic subpart
        arc_A = 0;
        arc_C = 0;
    }
    else{  // Not a non-basic subsection that needs to be rotated
        double original_cutPlane_vector[3][1]={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};

        arc_C = get_angle(triangles[0].n_y,triangles[0].n_x);   // the result unit is in radius
        double rot_C[3][3]={ {cos(arc_C),(-1)*sin(arc_C),0},
                             {sin(arc_C),cos(arc_C),0},
                             {0,0,1} };
        double AfterC_cutPlane_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        deal_3by1Martix(rot_C,original_cutPlane_vector,AfterC_cutPlane_vector);

        arc_A = get_angle(AfterC_cutPlane_vector[2][0],AfterC_cutPlane_vector[1][0]);   // the result unit is in radius    
        double rot_A[3][3]={ {1,0,0},
                             {0,cos(arc_A),(-1)*sin(arc_A)},
                             {0,sin(arc_A),cos(arc_A)}};
        double print_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        deal_3by1Martix(rot_A,AfterC_cutPlane_vector,print_vector);
        
        deal_3by3Martix(rot_A,rot_C,rotation);  // The ratation matrix is translate from i to i-1
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

void Slicer::deal_3by1Martix(double A[3][3], double B[3][1], double Re[3][1]){
    for(int m=0; m<3; m++){
        for(int n=0; n<1; n++){
            for(int k=0; k<3; k++){
                Re[m][n]+=A[m][k]*B[k][n];
            }
        }
    }
}

void Slicer::deal_3by3Martix(double A[3][3], double B[3][3], double Re[3][3]){
    for(int m=0; m<3; m++){
        for(int n=0; n<3; n++){
            for(int k=0; k<3; k++){
                Re[m][n]+=A[m][k]*B[k][n];
            }
        }
    }
}

void Slicer::deal_translate_meshpoints(double A[3][3],vector<Tri>& current_tri,vector<Tri>& new_tri){
    for(int i = 1; i < current_tri.size(); i++){
        // The following coding part is because of the data type of Tri.
        cout<< i << endl;
        static Tri triP;
        for(int j=0;j<3;j++){
            double re_point[3][1] = {0,0,0};
            if(j==0){
                double or_point1[3][1] = {current_tri[i].P1_x, current_tri[i].P1_y, current_tri[i].P1_z};
                deal_3by1Martix(A, or_point1, re_point);
                triP.P1_x = re_point[0][0];
                triP.P1_y = re_point[1][0];
                triP.P1_z = re_point[2][0];
            }
            if(j==1){
                double or_point2[3][1] = {current_tri[i].P2_x, current_tri[i].P2_y, current_tri[i].P2_z};
                deal_3by1Martix(A, or_point2, re_point);
                triP.P2_x = re_point[0][0];
                triP.P2_y = re_point[1][0];
                triP.P2_z = re_point[2][0];
            }
            if(j==2){
                double or_point3[3][1] = {current_tri[i].P3_x, current_tri[i].P3_y, current_tri[i].P3_z};
                deal_3by1Martix(A, or_point3, re_point);
                triP.P3_x = re_point[0][0];
                triP.P3_y = re_point[1][0];
                triP.P3_z = re_point[2][0];
            } 
        }
        new_tri.push_back(triP);
    }
}