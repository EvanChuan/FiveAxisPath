#include <iostream>
#include <algorithm>
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

// write the binary stl file output
//typedef Function::tri Tri;   // declear define structure
void RWfile::Write_bin_stl(std::string filename, std::vector<Tri> triangles){

    //binary file
    std::string header_info = "solid " + filename + "-output";
    char head[80];
    std::strncpy(head,header_info.c_str(),sizeof(head)-1);
    char attribute[2] = "0";
    unsigned long nTriLong = triangles.size() ;

    std::ofstream myfile;

    myfile.open((filename + ".stl").c_str(),  std::ios::out | std::ios::binary);
    myfile.write(head,sizeof(head));
    myfile.write((char*)&nTriLong,4);

    //write down every triangle
    for (std::vector<Tri>::iterator it = triangles.begin(); it!=triangles.end(); it++){
        //normal vector coordinates
        myfile.write((char*)&it->n_x, 4);
        myfile.write((char*)&it->n_y, 4);
        myfile.write((char*)&it->n_z, 4);

        //p1 coordinates
        myfile.write((char*)&it->P1_x, 4);
        myfile.write((char*)&it->P1_y, 4);
        myfile.write((char*)&it->P1_z, 4);

        //p2 coordinates
        myfile.write((char*)&it->P2_x, 4);
        myfile.write((char*)&it->P2_y, 4);
        myfile.write((char*)&it->P2_z, 4);

        //p3 coordinates
        myfile.write((char*)&it->P3_x, 4);
        myfile.write((char*)&it->P3_y, 4);
        myfile.write((char*)&it->P3_z, 4);

        myfile.write(attribute,2);
    }

    myfile.close();
}

// change the tmesh data to vector<string>
// 針對生成的off資料做處理，先將整個文字流進行 行分割，在進行每行的空白分割
std::vector<std::vector<double>> RWfile::splitString(const std::string& str)
{
    std::vector<std::vector<double>> new_poly_double;
    std::stringstream ss(str);
    std::string token;
    int flag_bug = 0;  // if need to debug for the order of data set the value to 1
    while (std::getline(ss, token, '\n')) { // split each line and then handle each line
        if(token == "OFF"){continue;}  // skip the first line "OFF" in .off data
        if(token.size()==0){continue;} // skip the change line (which is nothing) in .off data
        if(flag_bug == 1){
            std::cout << token << std::endl;
            flag_bug = 0;
        }
        std::string s="";
        std::vector<double> tmp;
        for(auto x:token){
            if(x==' '&& s!=""){
                tmp.push_back(std::stod(s));
                s="";
            }
            else if (x==' ')
            {
                s="";
            }
            else{
                s=s+x;
            }
        }
        // add the last data value to needed data vector
        tmp.push_back(std::stod(s));
        new_poly_double.push_back(tmp);
        tmp.clear();
    }
    return new_poly_double;
}
