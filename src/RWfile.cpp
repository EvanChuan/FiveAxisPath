#include <iostream>
#include "Function.h"
#include "RWfile.h"

// write the .off file and output
void RWfile::WritePolytoOFF(Polyhedron& tmesh,vector<Polyhedron> cut_res){
    std::string P;
    int res_id = 0;

    P = Function::get_poly(tmesh);
    //std::cout << P << std::endl;
    std::ofstream out("output_" + std::to_string(res_id) + ".off");
    out << P;
    out.clear();
    out.close();
    res_id++;

    for(vector<Polyhedron>::iterator id=cut_res.end()-1; id!=cut_res.begin()-1; id--){
        P = Function::get_poly(*id);
        //std::cout << "ID :"<< std::to_string(res_id) << std::endl;
        std::ofstream out("output_" + std::to_string(res_id) + ".off");
        out << P;
        out.clear();
        out.close();
        res_id++;
    }
}
