#include "TransformCalcu.h"
#include "spdlog/spdlog.h"

float MY_PI = 3.1415926;

using namespace cura;

void TransformCalcu::GetTransformation(int Id, FMatrix4x3& transformation,vector<Tri>& current_tri,vector<std::pair<float,float>> rotated_angle)
{
    float arc_A = 0, arc_C = 0;  // the angles here are ith mesh to (i-1)th mesh
    
    if(Id == 0){  // base mesh
        rotated_angle.push_back(make_pair(arc_A,arc_C));
        return;
    }
    // Handling not the base mesh, i>0
    // 1. rotate cutplan_normal
    double cn[3][1] = {current_tri[0].n_x,current_tri[0].n_y,current_tri[0].n_z};
    Point3 afterRotate_cn = transformation.apply(FPoint3((float)cn[0][0], (float)cn[1][0], (float)cn[2][0]));
    // 2. After rotate the current cutplan_normal to previousframe, and then calulate needed angle
    FMatrix4x3 Frame_i_matrix;
    calculate_rotateangle(afterRotate_cn, arc_A, arc_C, Frame_i_matrix);
    // 3. update the rotation matrix from i to base
    FMatrix4x3 tmp_matrix;
    Multiple_Matrix(transformation,Frame_i_matrix,tmp_matrix);       // The rotation matrix is translate from i to base 
    transformation = tmp_matrix;
    return;
}

void TransformCalcu::calculate_rotateangle(cura::Point3& normal_V,float& arc_A,float& arc_C,FMatrix4x3& FM)
{
    Point3 base_normal(0,0,1);
    float inner_dot = base_normal.dot(normal_V);
    if(inner_dot==1){// basic subpart
        spdlog::warn("inner_dot for normal_V and base_normal is 1 !");
        arc_A = 0;
        arc_C = 0;
    }
    else{  // Not a non-basic subsection that needs to be rotated
        Point3 original_cutPlane_vector = normal_V;
        //cout << "The C ";
        arc_C = get_angle(normal_V.y,normal_V.x);   // the result unit is in radius
        float rot_C[3][3]={ {cos(arc_C),(-1)*sin(arc_C),0},
                             {sin(arc_C),cos(arc_C),0},
                             {0,0,1} };

        Point3 AfterC_cutPlane_vector = deal_3by1Matrix(rot_C,original_cutPlane_vector);
        
        //cout << "The A ";
        arc_A = get_angle(AfterC_cutPlane_vector.z,AfterC_cutPlane_vector.y);   // the result unit is in radius    
        float rot_A[3][3]={ {1,0,0},
                             {0,cos(arc_A),(-1)*sin(arc_A)},
                             {0,sin(arc_A),cos(arc_A)}};
        Point3 print_vector = deal_3by1Matrix(rot_A,AfterC_cutPlane_vector);
        //print_vector.show();  // print out the print_vector
        deal_FrameMatrix(arc_A,arc_C,FM);  // The ratation matrix is translate from i to i-1
    }
    return;
}

float TransformCalcu::get_angle(float a, float b){
    // using actan
    float arc=atan(a/b);
    float deg=arc*180/MY_PI;
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
    float scal_b = abs(b);
    float scal = sqrt(pow(a,2)+pow(b,2));
    float angle = asin(scal_b/scal);  // the result unit is in radius
    float deg = angle*180/3.14159;

    // need add addtion conditon code
    //->>
    */
    return deg;
}

Point3 TransformCalcu::deal_3by1Matrix(float m[3][3], const FPoint3& p)
{
    return Point3(
        MM2INT(p.x * m[0][0] + p.y * m[0][1] + p.z * m[0][2]),
        MM2INT(p.x * m[1][0] + p.y * m[1][1] + p.z * m[1][2]),
        MM2INT(p.x * m[2][0] + p.y * m[2][1] + p.z * m[2][2])
    );
}

void TransformCalcu::deal_FrameMatrix(float& A,float& C,FMatrix4x3& FM){
    FM.m[0][0] = cos(C);
    FM.m[1][0] = (-1)*sin(C);
    FM.m[2][0] = 0.0;
    FM.m[3][0] = 0.0;
    FM.m[0][1] = cos(A)*sin(C);
    FM.m[1][1] = cos(A)*cos(C);
    FM.m[2][1] = (-1)*sin(A);
    FM.m[3][1] = 0.0;
    FM.m[0][2] = sin(A)*sin(C);
    FM.m[1][2] = sin(A)*cos(C);
    FM.m[2][2] = cos(A);
    FM.m[3][2] = 0.0;
}

void TransformCalcu::Multiple_Matrix(FMatrix4x3& A,FMatrix4x3& B,FMatrix4x3& FM){
    FM.m[0][0] = A.m[0][0]*B.m[0][0]+A.m[1][0]*B.m[0][1]+A.m[2][0]*B.m[0][2];
    FM.m[1][0] = A.m[0][0]*B.m[1][0]+A.m[1][0]*B.m[1][1]+A.m[2][0]*B.m[1][2];
    FM.m[2][0] = A.m[0][0]*B.m[2][0]+A.m[1][0]*B.m[2][1]+A.m[2][0]*B.m[2][2];
    FM.m[3][0] = 0.0;
    FM.m[0][1] = A.m[0][1]*B.m[0][0]+A.m[1][1]*B.m[0][1]+A.m[2][1]*B.m[0][2];
    FM.m[1][1] = A.m[0][1]*B.m[1][0]+A.m[1][1]*B.m[1][1]+A.m[2][1]*B.m[1][2];
    FM.m[2][1] = A.m[0][1]*B.m[2][0]+A.m[1][1]*B.m[2][1]+A.m[2][1]*B.m[2][2];
    FM.m[3][1] = 0.0;
    FM.m[0][2] = A.m[0][2]*B.m[0][0]+A.m[1][2]*B.m[0][1]+A.m[2][2]*B.m[0][2];
    FM.m[1][2] = A.m[0][2]*B.m[1][0]+A.m[1][2]*B.m[1][1]+A.m[2][2]*B.m[1][2];
    FM.m[2][2] = A.m[0][2]*B.m[2][0]+A.m[1][2]*B.m[2][1]+A.m[2][2]*B.m[2][2];
    FM.m[3][2] = 0.0;
}