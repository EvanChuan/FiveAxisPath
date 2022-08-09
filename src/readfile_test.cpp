#include <CGAL/Point_set_3.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include "MCF_Skeleton.h"
#include "Function.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "usage: ./main <input_file>" << std::endl;
        return 0;
    }
    std::ifstream input(argv[1]);
    Polyhedron tmesh;
    //Triangle_mesh tmesh;
    input >> tmesh;
    if (!CGAL::is_triangle_mesh(tmesh))
    {
        std::cout << "Input geometry is not triangulated." << std::endl;
        return EXIT_FAILURE;
    }
    
    // Skeletonize
    Skeleton skeleton;       // the result of skeleton is stored in this
    Skeletonization mcs(tmesh);
    MCF_Skeleton MS;         // declare the class as a object
    MS.MCF_Skeleton_Fun(skeleton, mcs);
    
    // Extract Skeleton Points
    Function FN;
    std::vector<std::vector<double>> SKP = FN.ExtractSkelPoints(skeleton);
    // Rearrange the skeleton points. But it needs general solution far.
    FN.Sorting(SKP);

    // Find cutting plan, Get new bulding plan.
    //std::vector<std::vector<double>> plane;
    std::vector<std::vector<double>> plane;
    plane = FN.cutting_plan(SKP);
    
    // run mesh cut divid. 
    vector<Polyhedron> cut_res;
    try{
        //std::cout << "Try Cut the Plan." << std::endl;
        FN.set_poly(tmesh);
        //print('--> Actual plane: ', plane)
        //print('--> Actual evaluation: ', ra.step(plane))
        cut_res = FN.plane_cut(tmesh,plane);
        std::cout << "cut_res size :"<< cut_res.size() << std::endl;
    }
    catch (std::exception &e) { // exception should be caught by reference
        cout << "exception: " << e.what() << "\n";
    }

    // write the .off file and output
    std::string P;
    int res_id = 0;
    P = FN.get_poly(tmesh);
    std::ofstream out("output_" + std::to_string(res_id) + ".off");
    out << P;
    out.clear();
    out.close();
    res_id++;

    for(vector<Polyhedron>::iterator id=cut_res.end()-1; id!=cut_res.begin()-1; id--){
        P = FN.get_poly(*id);
        //std::cout << "ID :"<< std::to_string(res_id) << std::endl;
        std::ofstream out("output_" + std::to_string(res_id) + ".off");
        out << P;
        out.clear();
        out.close();
        res_id++;
    }

    return 0;
}
