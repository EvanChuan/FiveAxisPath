#include <CGAL/Point_set_3.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include "Function.h"
#include "MCF_Skeleton.h"

using namespace std;

bool Function::compareZ(Skel_Points p1, Skel_Points p2){
   return p1.Z < p2.Z;
}

void Function::Sorting(std::vector<Function::Skel_Points> points){
	std::cout << "Start Sorting!" << "\n";
	std::sort(points.begin(), points.end(), compareZ);
    
	std::ofstream output2("order_skel_points.txt");
	for(std::vector<struct Skel_Points>::iterator it = points.begin(); it != points.end(); ++it)
	{
		output2 << "X" << it->X << " Y" << it->Y << " Z" << it->Z << "\n";
	}
	output2.close();
	return;
}

std::vector<Function::Skel_Points> Function::ExtractSkelPoints(Skeletonization::Skeleton& skeleton){
	std::cout << "Start Extract Points!" << "\n";
	// Output all the edges of the skeleton.
	std::vector<Function::Skel_Points> Res_SKP;  // the result of skeleton points
	Function::Skel_Points SP;
	//std::ofstream output("skel_points.txt");
	for(Skeleton_edge e : CGAL::make_range(edges(skeleton)))
	{
    	const Point& s = skeleton[source(e, skeleton)].point;    // start point of edge  data type : CGAL::Point_3<CGAL::Simple_cartesian<double>>
    	//std::cout << s[0] << "\n";
    	//const Point& t = skeleton[target(e, skeleton)].point;    // end point of edge
    	//output << s << "\n";
    	SP.X = s[0];
    	SP.Y = s[1];
    	SP.Z = s[2];
    	Res_SKP.push_back(SP);
	}
	//output.close();

	return Res_SKP;
}
