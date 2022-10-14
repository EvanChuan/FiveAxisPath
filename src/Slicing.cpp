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
    double rotationM[3][3] = {{1,0,0},{0,1,0},{0,0,1}}; // Storing the pre rotation
    double afterRotate_plane_point[3][1] = {0,0,0};

    // each subpart is processd separately and then stored in the pregcode_data vector
// NEED!!!!    //for(int i=0;i<trianglemesh.size();i++){
    for(int i=0;i<1;i++){
        vector<Tri> current_tri = trianglemesh[i];
        // ******** Translate Part (Input:the divided subpart mesh,current_tri. Output:the translated subpart,tri_final_rotate) ********* //
        vector<Tri> tri_inPreviousFrame;
        vector<Tri> tri_final_rotate;
        Slicer::plane pl;
        double arc_A = 0, arc_C = 0;
        if(i>0){      // for non-basic part process
            double rotationM_i[3][3] = {{0,0,0},{0,0,0},{0,0,0}};        // The rotation matrix is translate from i to i-1            
            if(i>1){  // for second-part and later
                double cutplan_normal[3][1] = {current_tri[0].n_x,current_tri[0].n_y,current_tri[0].n_z};
                double afterRotate_cutplan_normal[3][1]={0,0,0};
                deal_3by1Matrix(rotationM, cutplan_normal, afterRotate_cutplan_normal);  // transfer the cutplan_normal first
                deal_translate_meshpoints(rotationM,current_tri,tri_inPreviousFrame);    // transfer the mesh
                current_tri[0].n_x = afterRotate_cutplan_normal[0][0];
                current_tri[0].n_y = afterRotate_cutplan_normal[1][0];
                current_tri[0].n_z = afterRotate_cutplan_normal[2][0];
                calculate_rotateangle(current_tri, arc_A, arc_C, rotationM_i);      // calculate the rotate angle
            }
            else{
                calculate_rotateangle(current_tri, arc_A, arc_C, rotationM_i);      // calculate the rotate angle
                tri_inPreviousFrame = current_tri;
            }
            double tmp_rotationM[3][3] = {{0,0,0},{0,0,0},{0,0,0}};     // Storing the pre rotation matrix
            deal_3by3Matrix(rotationM,rotationM_i,tmp_rotationM);       // The rotation matrix is translate from i to base 
            update_Matrix(rotationM, tmp_rotationM);
            
            // Tranlate the trianglemesh to align the Z-axis positive direction
            // The rotation matrix used here is "rotationM_i" because the mesh was rotated once before in deal_translate_meshpoints
            double plane_point[3][1] = {plane_points[i].X,plane_points[i].Y,plane_points[i].Z};
            deal_3by1Matrix(rotationM_i, plane_point, afterRotate_plane_point);
            deal_translate_meshpoints(rotationM_i,tri_inPreviousFrame,tri_final_rotate);
            
            //std::string filename = "output_tran"+std::to_string(i);
            //RWfile::Write_GCode(filename,tri_final_rotate);  // write the points after rotation
            //std::string filename1 = "output_org"+std::to_string(i);
            //RWfile::Write_GCode(filename1,current_tri);
        }
        else if (i==0){
            tri_final_rotate = current_tri;
            // The basic part do with nothing(no rotation).
            //std::string filename = "output_org"+std::to_string(i);
            //RWfile::Write_GCode(filename,tri_final_rotate);
        }
        rotated_angle.push_back(make_pair(arc_A,arc_C)); // The unit is radius need to translate and the angle is added to previous one

        // ******** Slicing Part ********* //
        pair<double,double> bounds = findBoundaries(tri_final_rotate);  // bounds = (BottomZ,topZ)
        int layernum = round((bounds.second - bounds.first)/layerheight);
        vector<double> slice_layer_height; // store each slicing layer height
        for(int i=0;i<layernum;i++){
            if(i==0){
                //std::cout << (layerheight/2) << std::endl;
                slice_layer_height.push_back((layerheight/2));
            }
            else{
                //std::cout << (layerheight/2)+layerheight*i << std::endl;
                slice_layer_height.push_back((layerheight/2)+layerheight*i);
            }
        }

            // ********  Find Segment ********* //
            // handles each layer according layer_height pre-step //
        vector<Slicer::slice> subpart_res;
        vector<vector<point>> tmp_res; // for checking temporary result
        for(int i=0; i<slice_layer_height.size(); i++){
            bool IsSurface = false;
            Slicer::slice sl;

            sl.z_value = slice_layer_height[i];
            vector<point> intersectPoint_set;   // a set of intersection points without ordered
            for(int j=0;j<tri_final_rotate.size();j++){
                // calculate intersect points of line and plan, the result of intersect points store in intersectPoint_set
                line L1,L2,L3;
                L1.P1 = {tri_final_rotate[j].P1_x,tri_final_rotate[j].P1_y,tri_final_rotate[j].P1_z} ;
                L1.P2 = {tri_final_rotate[j].P2_x,tri_final_rotate[j].P2_y,tri_final_rotate[j].P2_z} ;
                intersectPoint(L1,slice_layer_height[i],intersectPoint_set);
                L2.P1 = {tri_final_rotate[j].P2_x,tri_final_rotate[j].P2_y,tri_final_rotate[j].P2_z} ;
                L2.P2 = {tri_final_rotate[j].P3_x,tri_final_rotate[j].P3_y,tri_final_rotate[j].P3_z} ;
                intersectPoint(L2,slice_layer_height[i],intersectPoint_set);
                L3.P1 = {tri_final_rotate[j].P3_x,tri_final_rotate[j].P3_y,tri_final_rotate[j].P3_z} ;
                L3.P2 = {tri_final_rotate[j].P1_x,tri_final_rotate[j].P1_y,tri_final_rotate[j].P1_z} ;
                intersectPoint(L3,slice_layer_height[i],intersectPoint_set);

            }
            /* //for checking
            std::string filename = "output_SP"+std::to_string(i);
            RWfile::Write_GCode_pointtype(filename,intersectPoint_set);
            */
            // decide counterclockwise or clockwise to order the countor
            vector<point> order_intersectPoint_set;
            //vector<point> interplotePoint_set;
            convex_hull(intersectPoint_set,order_intersectPoint_set);
            //interplote_points(order_intersectPoint_set,interplotePoint_set,0.1);
            intersectPoint_set.clear();
            /* //for checking
            std::string filename_con = "output_con"+std::to_string(i);
            RWfile::Write_GCode_pointtype(filename_con,order_intersectPoint_set);
            std::string filename_ins = "output_ins"+std::to_string(i);
            RWfile::Write_GCode_pointtype(filename_ins,interplotePoint_set);
            */
            // infill parten and wall


            tmp_res.push_back(interplotePoint_set);
            //order_intersectPoint_set.clear();
            interplotePoint_set.clear();
        }

        //std::string filename = "output_check";
        //RWfile::Write_Gcode_indoubleVec(filename,tmp_res);
        pregcode_data.push_back(subpart_res);
    }
    
    return pregcode_data;
}

pair<double,double> Slicer::findBoundaries(vector<Tri>& triangles){
    pair<double,double> bounds;
    double bottomZ = 500;
    double topZ = -500;
    for(int i = 1; i < triangles.size(); i++){
        double maximum = max({triangles[i].P1_z, triangles[i].P2_z, triangles[i].P3_z});
        double minimum = min({triangles[i].P1_z, triangles[i].P2_z, triangles[i].P3_z});

        if(maximum > topZ){topZ = maximum;}
        if(minimum < bottomZ){bottomZ = minimum;}
    }
    bounds = make_pair(bottomZ,topZ);
    return bounds;
} 

void Slicer::calculate_rotateangle(vector<Tri>& triangles,double& arc_A,double& arc_C,double rotation[3][3])
{
    vector<double> base_normal = {0,0,1};
    vector<double> cutPlane_vector ={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};
    double inner_dot = inner_product(cutPlane_vector.begin(), cutPlane_vector.end(), base_normal.begin(), 0.0000);

    if(inner_dot==1){
        // basic subpart
        arc_A = 0;
        arc_C = 0;
    }
    else{  // Not a non-basic subsection that needs to be rotated
        double original_cutPlane_vector[3][1]={triangles[0].n_x,triangles[0].n_y,triangles[0].n_z};
        cout << "The C ";
        arc_C = get_angle(triangles[0].n_y,triangles[0].n_x);   // the result unit is in radius
        double rot_C[3][3]={ {cos(arc_C),(-1)*sin(arc_C),0},
                             {sin(arc_C),cos(arc_C),0},
                             {0,0,1} };
        double AfterC_cutPlane_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        deal_3by1Matrix(rot_C,original_cutPlane_vector,AfterC_cutPlane_vector);
        cout << "The A ";
        arc_A = get_angle(AfterC_cutPlane_vector[2][0],AfterC_cutPlane_vector[1][0]);   // the result unit is in radius    
        double rot_A[3][3]={ {1,0,0},
                             {0,cos(arc_A),(-1)*sin(arc_A)},
                             {0,sin(arc_A),cos(arc_A)}};
        double print_vector[3][1] = {0,0,0};
        // new_vector[3][1] = rotate_matrix[3][3] * old_vector[3][1]
        deal_3by1Matrix(rot_A,AfterC_cutPlane_vector,print_vector);
        deal_3by3Matrix(rot_A,rot_C,rotation);  // The ratation matrix is translate from i to i-1
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

void Slicer::deal_3by1Matrix(double A[3][3], double B[3][1], double Re[3][1]){
    for(int m=0; m<3; m++){
        for(int n=0; n<1; n++){
            for(int k=0; k<3; k++){
                Re[m][n]+=A[m][k]*B[k][n];
            }
        }
    }
}

void Slicer::deal_3by3Matrix(double A[3][3], double B[3][3], double Re[3][3]){
    for(int m=0; m<3; m++){
        for(int n=0; n<3; n++){
            for(int k=0; k<3; k++){
                Re[m][n]+=A[m][k]*B[k][n];
            }
        }
    }
}

void Slicer::update_Matrix(double A[3][3], double B[3][3]){  // set B to A
    for(int m=0; m<3; m++){
        for(int n=0; n<3; n++){
            A[m][n]= B[m][n];
        }
    }
}

void Slicer::deal_translate_meshpoints(double A[3][3],vector<Tri>& current_tri,vector<Tri>& new_tri){
    for(int i = 1; i < current_tri.size(); i++){
        // The following coding part is because of the data type of Tri.
        static Tri triP;
        for(int j=0;j<3;j++){
            double re_point[3][1] = {0,0,0};
            if(j==0){
                double or_point1[3][1] = {current_tri[i].P1_x, current_tri[i].P1_y, current_tri[i].P1_z};
                deal_3by1Matrix(A, or_point1, re_point);
                triP.P1_x = re_point[0][0];
                triP.P1_y = re_point[1][0];
                triP.P1_z = re_point[2][0];
            }
            if(j==1){
                double or_point2[3][1] = {current_tri[i].P2_x, current_tri[i].P2_y, current_tri[i].P2_z};
                deal_3by1Matrix(A, or_point2, re_point);
                triP.P2_x = re_point[0][0];
                triP.P2_y = re_point[1][0];
                triP.P2_z = re_point[2][0];
            }
            if(j==2){
                double or_point3[3][1] = {current_tri[i].P3_x, current_tri[i].P3_y, current_tri[i].P3_z};
                deal_3by1Matrix(A, or_point3, re_point);
                triP.P3_x = re_point[0][0];
                triP.P3_y = re_point[1][0];
                triP.P3_z = re_point[2][0];
            } 
        }
        new_tri.push_back(triP);
    }
}

void Slicer::intersectPoint(line& L, double& S,vector<point>& intersectPoint_set){
    point preP;
    if(L.P1[2]==L.P2[2] && L.P2[2]== S){
        preP.x = L.P1[0];
        preP.y = L.P1[1];
        preP.z = L.P1[2];
        intersectPoint_set.push_back(preP);
    }
    else if(L.P1[2]==L.P2[2]){
        return;
    }
    else{
        vector<double> slope={L.P2[0]-L.P1[0],L.P2[1]-L.P1[1],L.P2[2]-L.P1[2]};
        double t = (S-L.P1[2])/(L.P2[2]-L.P1[2]) ;
        if(t>=0 && t<=1){
            double tmpZ = L.P1[2]+t*slope[2];
            if (tmpZ <= max(L.P2[2],L.P1[2])&& tmpZ >= min(L.P2[2],L.P1[2])){
                preP.x = L.P1[0]+t*slope[0];
                preP.y = L.P1[1]+t*slope[1];
                preP.z = L.P1[2]+t*slope[2];
                intersectPoint_set.push_back(preP);
                return;
            }
            else{ return; }
        }
        else{ return; }
    }
}

void Slicer::convex_hull(vector<point>& P,vector<point>& re_P){
	size_t n = P.size(), k = 0;
	if (n <= 3){
        sort(P.begin(), P.end());
        re_P = P;
        return;
    }
	vector<point> H(2*n);

	// Sort points lexicographically
	sort(P.begin(), P.end());

	// Build lower hull
	for (size_t i = 0; i < n; i++) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}

	// Build upper hull
	for (size_t i = n-1, t = k+1; i > 0; --i) {
		while (k >= t && cross(H[k-2], H[k-1], P[i-1]) <= 0) k--;
		H[k++] = P[i-1];
	}

	H.resize(k-1);  // Because the last point in the returned list is the same as the first one.
	re_P = H;
    return;
}

double Slicer::cross(const point &O, const point &A, const point &B){
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

void Slicer::interplote_points(vector<point>& P,vector<point>& re_P,double t){
    for(int i=0; i<P.size(); i++){
        if(i!=P.size()-1){
            point p1 = P[i], p2 = P[i+1];
            double a,b;
            int n = insert_numANDparmeters(p1,p2,a,b,t);
            point insertP;
            for(int j=0;j<n;j++){
                insertP.x = p1.x+(a*j*t);
                insertP.y = p1.y+(b*j*t);
                insertP.z = p1.z;
                re_P.push_back(insertP);
            }
        }
        else{  // deal with the last point
            point p1 = P[P.size()-1], p2 = P[0];
            double a,b;
            int n = insert_numANDparmeters(p1,p2,a,b,t);
            point insertP;
            for(int j=0;j<n;j++){
                insertP.x = p1.x+(a*j*t);
                insertP.y = p1.y+(b*j*t);
                insertP.z = p1.z;
                re_P.push_back(insertP);
            }           
        }
    }
}

int Slicer::insert_numANDparmeters(point& p1,point& p2,double& a,double& b,double& t){
    a = p2.x-p1.x;
    b = p2.y-p1.y;
    double L = sqrt(pow(a,2)+pow(b,2));
    int n = (int)(L/t);
    return n;
}