#include <CGAL/Point_set_3.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include "MCF_Skeleton.h"
#include "Function.h"
#include "RWfile.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {   // it can only read .off file now
        std::cout << "usage: ./main <input_file>" << std::endl;
        return 0;
    }
    std::ifstream input(argv[1]);
    Polyhedron tmesh;
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
    std::vector<std::vector<double>> plane;
    plane = FN.cutting_plan(SKP);
    
    // run mesh cut divid. 
    vector<Polyhedron> cut_res;
    try{
        //std::cout << "Try Cut the Plan." << std::endl;
        FN.set_poly(tmesh);
        cut_res = FN.plane_cut(tmesh,plane);  // cut model is stored in cut_res vector
        // First cut plan is the build plan of last sub-part which store in first of cut_res
        std::cout << "cut_res size :"<< cut_res.size() << std::endl;
    }
    catch (std::exception &e) { // exception should be caught by reference
        cout << "exception: " << e.what() << "\n";
    }

    // write the .off file and output
    RWfile::WritePolytoOFF(tmesh,cut_res);
    
    // change the data type result of .off data (string to double)
    vector< vector<Tri> > trianglemesh = FN.TransfertoTriMesh(tmesh,cut_res,plane);
    
    //std::vector<std::vector<double>> new_poly_double= RWfile::splitString(FN.get_poly(cut_res[1]));
    /*
    // print for debug
    for(int i=0; i<new_poly_double.size();i++){
    //for(int i=0; i<new_poly_double[0][0]+2;i++){
        if(new_poly_double[i].size()!=0){
            std::cout << new_poly_double[i][0] << " " << new_poly_double[i][1] << " " << new_poly_double[i][2];
            if(new_poly_double[i].size()==4){
                std::cout <<  " " << new_poly_double[i][3];
            }
            std::cout << "\n";
        }
    }
    */
    // polygon tranfer to triangle
    //vector<Tri> trianglemesh = FN.PolytoTri_for_Print(new_poly_double);
    /*
    // check trianglemesh
    ofstream file;
    file.open("checkoutput.txt");
    if (!file.is_open()) {
        cout << "Failed to open file.\n";
    }
    for(vector<Tri>::iterator it = trianglemesh.begin(); it!=trianglemesh.end(); it++){
        string normal_vector = std::to_string(it->n_x) + " " + std::to_string(it->n_y) + " " + std::to_string(it->n_z)+ "\n"; 
        string P1 = std::to_string(it->P1_x) + " " + std::to_string(it->P1_y) + " " + std::to_string(it->P1_z)+ "\n";  
        string P2 = std::to_string(it->P2_x) + " " + std::to_string(it->P2_y) + " " + std::to_string(it->P2_z)+ "\n";  
        string P3 = std::to_string(it->P3_x) + " " + std::to_string(it->P3_y) + " " + std::to_string(it->P3_z)+ "\n";  
        file << "normal_vector: " << normal_vector;
        file << "P1: " << P1;
        file << "P2: " << P2;
        file << "P3: " << P3;
    }
    file.close();
    */
    //RWfile::Write_bin_stl("output_1", trianglemesh);

    return 0;
}
