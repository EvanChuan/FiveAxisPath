#include <CGAL/Point_set_3.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include "MCF_Skeleton.h"
#include "Function.h"
#include "RWfile.h"
#include "Slicing.h"

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
    vector<vector<double>> SKP = FN.ExtractSkelPoints(skeleton);
    // Rearrange the skeleton points. But it needs general solution far.
    FN.Sorting(SKP);

    // Find the cutting plan based on the skeleton points, Get new bulding plan.
    vector<vector<double>> plane;
    vector<Function::Skel_Points> plane_points;
    plane = FN.cutting_plan(SKP,plane_points);   // The data of plane is [a,b,c,d] which are normal vector and parameter d.
    
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
    //RWfile::WritePolytoOFF(tmesh,cut_res);
    
    // change the data type result of .off data (string to double)
    vector< vector<Tri> > trianglemesh = FN.TransfertoTriMesh(tmesh,cut_res,plane);
    /*
    for(int i=0;i<=trianglemesh.size()-1;i++){
        std::cout << i << std::endl;
        vector<Tri> current_tri = trianglemesh[i];
        std::string filename = "output"+std::to_string(i);
        RWfile::Write_GCode(filename,current_tri);
        //RWfile::Write_bin_stl(filename, current_tri);
    }
    */
    // *******************************************
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
    
    // *******************************************

    // The slicing process (input:trianglemesh  output:pregcode_data)
    double layerheight = 0.2;
    Slicer SL;
    // The size of plane_points and trianglemesh differ by 1.
    vector<vector<Slicer::slice>> pregcode_data = SL.subpart_slicing(trianglemesh,plane_points,layerheight);

    return 0;
}
