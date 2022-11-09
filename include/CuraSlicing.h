#ifndef CURASLICING_H
#define CURASLICING_H

#include "Function.h"
#include "Cura/MeshGroup.h"

class CuraSlicer
{
public:
    void subpart_slicing(vector<vector<Tri>>& trianglemesh, vector<Function::Skel_Points>& plane_points,double layerheight);
    bool load_parameters(std::string& json_filename, cura::Settings& last_settings);

};
#endif //CURASLICING_H