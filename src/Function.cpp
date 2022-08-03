#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include "Function.h"

using namespace std;

void Function::cutting_plan(std::vector<std::vector<double>>& points){
    std::vector<double> printDIR = {0,0,1};
    std::vector<double> preP = {0,0,0};
    for(std::vector<std::vector<double>>::iterator it = points.begin(); it != points.end(); ++it)
	{
        std::vector<double> it_v = {it[0][0] - preP[0],it[0][1] - preP[1],it[0][2] - preP[2]};
        std::vector<double> unit_it_v = Normal(it_v);
        float tha = angle(unit_it_v,printDIR);
        if( tha > 30){
            std::cout << "more than 30 " << std::endl;
            printDIR = unit_it_v;
            std::cout << "cutplan_vector : X" << unit_it_v[0] << " Y"  << unit_it_v[1] << " Z"<< unit_it_v[2] << std::endl;
            preP = {it[0][0],it[0][1],it[0][2]};
            // D = -(ax+by+cz)
            double D = (unit_it_v[0]*it[0][0]+unit_it_v[1]*it[0][1]+unit_it_v[2]*it[0][2])*(-1);
            std::cout << "D : " << D << std::endl;
        }
	}
    return;
}

double Function::angle(std::vector<double> v1,std::vector<double> v2){
    double inner = inner_product(v1.begin(), v1.end(), v2.begin(), 0.0000); // the fourth argument can seen as precision, https://stackoverflow.com/questions/12065041/zero-inner-product-when-using-stdinner-product
    double scal = sqrt(pow(v1[0],2)+pow(v1[1],2)+pow(v1[2],2))*sqrt(pow(v2[0],2)+pow(v2[1],2)+pow(v2[2],2));
    double cos_th = inner / scal;
    double tha = acos(cos_th)*180/3.14159;
    std::cout << "angle : " << tha << std::endl;
    return tha;
}

std::vector<double> Function::Normal(std::vector<double> v){
    double len = sqrt(pow(v[0],2)+pow(v[1],2)+pow(v[2],2));
    std::vector<double> it_v = {v[0]/len,v[1]/len,v[2]/len};
    return it_v;
}
