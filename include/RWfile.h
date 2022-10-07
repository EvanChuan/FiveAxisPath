#ifndef RWFILE_H
#define RWFILE_H

#include<iostream>
#include "CustomisedPolyhedron.h"
#include "Function.h"
#include "Slicing.h"

using namespace std;

class RWfile
{
public:

    static void WritePolytoOFF(Polyhedron& poly,vector<Polyhedron> cut_res);
    static void Write_bin_stl(std::string filename, std::vector<Tri> triangles);
    static std::vector<std::vector<double>> splitString(const std::string& str);
    static void Write_GCode(std::string filename, vector<Tri> triangles);
    static void Write_GCode_pointtype(std::string filename, vector<Slicer::point> triangles);

};
#endif