#include <CGAL/Point_set_3.h>
#include <fstream>
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
    //Polyhedron tmesh;
    Triangle_mesh tmesh;
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

    // Find cutting plan
    FN.cutting_plan(SKP);
    
    // Get new bulding plan
    // 
    try{
        //ra = RoboFDM.init()  // inital RoboFDM oject in python script
        //FN.reset(argv[1])  // ("argv[1]") ?
        //FN.set_poly(poly)
        //print('--> Actual plane: ', plane)
        //print('--> Actual evaluation: ', ra.step(plane))
        //ra.plane_cut(plane)
        //poly=ra.get_poly()
        //return_dict[0] = poly
    }
    catch (std::exception &e) { // exception should be caught by reference
        cout << "exception: " << e.what() << "\n";
    }

    return 0;
}
