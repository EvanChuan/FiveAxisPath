#include <iostream>
#include <fstream>
#include "CuraSlicing.h"
#include "TransformCalcu.h"
#include "Cura/Application.h" //To get the extruders for material estimates.
#include "Cura/Slice.h"
#include "Cura/utils/FMatrix4x3.h" //For the mesh_rotation_matrix setting.
#include "Cura/FffProcessor.h" //To start a slice and get time estimates.
#include "Cura/communication/CommandLine.h"
#include "spdlog/spdlog.h"

using namespace cura;

void CuraSlicer::subpart_slicing(vector<vector<Tri>>& trianglemesh, vector<Function::Skel_Points>& plane_points,double layerheight)
{   // each subpart is processd separately and then stored in the pregcode_data vector
    
    // ************* Reference from CommandLine::sliceNext() ************* //
    size_t num_mesh_groups = trianglemesh.size();   // Count the number of sub mesh groups for slicing.
    Slice slice(num_mesh_groups);  // a container for slicing mesh, create Scene object.
    
    Application::getInstance().current_slice = &slice;  // the current_slice is a pointer in Application class. the address of slice
    size_t mesh_group_index = 0;
    Settings* last_settings = &slice.scene.settings;  // the pointer last_settings point to parameter slice.scene.settings

    //*** 宣告extruder的vector大小，並做存放
    //slice.scene.extruders.reserve(arguments.size() >> 1); // Allocate enough memory to prevent moves.
    slice.scene.extruders.reserve(1); // Allocate enough memory to prevent moves.
    slice.scene.extruders.emplace_back(0, &slice.scene.settings); // Always have one extruder. 根據class ExtruderTrain定義下去的
    ExtruderTrain* last_extruder = &slice.scene.extruders[0];

    // load settings parameters, the values are load successfully in &slice.scene.settings
    std::string json_filename = "../resources/definitions/fdmprinter.def.json";
    if (CommandLine::loadJSON(json_filename, *last_settings)){
        spdlog::error("Failed to load JSON file: {}", json_filename);
    }
    // If this was the global stack, create extruders for the machine_extruder_count setting.
    if (last_settings == &slice.scene.settings){
        const size_t extruder_count = slice.scene.settings.get<size_t>("machine_extruder_count");
        while (slice.scene.extruders.size() < extruder_count){
            slice.scene.extruders.emplace_back(slice.scene.extruders.size(), &slice.scene.settings);
        }
    }      
    //load_parameters(json_filename, *last_settings);
    std::cout << "Load filename successfully" << std::endl;

    FMatrix4x3 transformation; // A matrix to rotate the mesh to the desired orientation
    vector<std::pair<float,float>> rotated_angle;     // Storing the rotational angle
    //for(int i=0;i<num_mesh_groups;i++){
    for(int i=0;i<1;i++){
        vector<Tri> current_tri = trianglemesh[i];  // current triangle data
        
        // Since the rotation is a sequence, the matrices are superimposed.
        TransformCalcu::GetTransformation(i,transformation, current_tri, rotated_angle);

        // load trianglemesh data to Cura mesh_group.meshes data
        if(!loadMeshIntoMeshGroup_(&slice.scene.mesh_groups[mesh_group_index], current_tri, transformation,last_extruder->settings))
        {
            std::cout << "Load Failed." << std::endl;
        }
        //std::cout << "meshgroup.size: " << slice.scene.mesh_groups[mesh_group_index].meshes.size() << std::endl;
    
        if(i == 0){
        slice.scene.mesh_groups[mesh_group_index].finalize();
        }
        //spdlog::info("Loaded from disk in {:3}s\n", FffProcessor::getInstance()->time_keeper.restart());
    }
    // Start slicing.
    slice.compute();
    
    std::cout << "subpart_slicing done!" << std::endl;
    return;
}

bool CuraSlicer::load_parameters(std::string& json_filename, cura::Settings& last_settings)
{
    std::ifstream input_file(json_filename);
    if (!input_file.is_open())
    {
        spdlog::error("Couldn't open file: {}", json_filename);
        return false;
    }
    std::string line;
    while (std::getline(input_file, line)){
        std::string name="", value ="";
        bool flag = 0;
        for(auto x:line){
            if(flag == 0 && x != '='){
                name += x;
            }
            else if (x == '='&& flag != 1)
            {
                flag = 1;
            }
            else if (flag==1){
                value += x;
            }
            
        }

        last_settings.add(name,value);
    }
    input_file.close();
    return true;
}