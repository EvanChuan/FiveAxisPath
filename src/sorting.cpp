#include <CGAL/Point_set_3.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include "Function.h"
#include "MCF_Skeleton.h"

using namespace std;

bool Function::compareZ(std::vector<double> p1, std::vector<double> p2){
   return p1[2] < p2[2];
}

void Function::Sorting(std::vector<std::vector<double>>& points){
	std::cout << "Start Sorting!" << "\n";
	std::sort(points.begin(), points.end(), compareZ);

	std::ofstream output2("order_skel_points.txt");
	for(std::vector<std::vector<double>>::iterator it = points.begin(); it != points.end(); ++it)
	{
		output2 << "X" << it[0][0] << " Y" << it[0][1] << " Z" << it[0][2] << "\n";
	}
	output2.close();
	return;
}

// Extract points ~ ~ ~
std::vector<std::vector<double>> Function::ExtractSkelPoints(Skeletonization::Skeleton& skeleton){
	std::cout << "Start Extract Points!" << "\n";
	// Output all the edges of the skeleton.
	set<std::vector<double>> Res_SKP_Set;  // the result of skeleton points
	//std::ofstream output("skel_points.txt");
	for(Skeleton_edge e : CGAL::make_range(edges(skeleton)))
	{
    	const Point& s = skeleton[source(e, skeleton)].point;    // start point of edge  data type : CGAL::Point_3<CGAL::Simple_cartesian<double>>
    	//std::cout << s[0] << "\n";
    	//const Point& t = skeleton[target(e, skeleton)].point;    // end point of edge
    	//output << s << "\n";
		Res_SKP_Set.insert({s[0],s[1],s[2]});  // use set to avoid duplicates, https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
	}
	//output.close();
	std::vector<std::vector<double>> Res_SKP_Vec;
	Res_SKP_Vec.assign( Res_SKP_Set.begin(), Res_SKP_Set.end() );

	return Res_SKP_Vec;
}
