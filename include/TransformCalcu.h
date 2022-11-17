#ifndef TransformCalcu_H
#define TransformCalcu_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Function.h"
#include "Cura/utils/Point3.h"
#include "Cura/utils/FMatrix4x3.h"
#include "Cura/utils/floatpoint.h" //To accept incoming meshes with floating point vertices.
#include "Cura/FffGcodeWriter.h"
#include "Cura/sliceDataStorage.h"

using namespace cura;

class TransformCalcu
{
public:
    // Calculate the transform matrix for each mash group
    static void GetTransformation(int Id, FMatrix4x3& transformation,vector<Tri>& current_tri,vector<std::pair<float,float>>& rotated_angle);
    static void calculate_rotateangle(cura::Point3& normal_V,float& arc_A,float& arc_C,FMatrix4x3& FM);
    static float get_angle(float a, float b);
    static float todegree(float& angle);
    static Point3 deal_3by1Matrix(float m[3][3], const FPoint3& p);
    static Point3 Multiple_3by1Matrix(FMatrix4x3& FM, const FPoint3& p);
    static void deal_FrameMatrix(float& A,float& C,FMatrix4x3& FM);
    static void Multiple_Matrix(FMatrix4x3& A,FMatrix4x3& B,FMatrix4x3& FM);
};
#endif