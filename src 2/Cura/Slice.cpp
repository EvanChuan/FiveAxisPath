// Copyright (c) 2022 Ultimaker B.V.
// CuraEngine is released under the terms of the AGPLv3 or higher

#include "spdlog/spdlog.h"

#include "Cura/ExtruderTrain.h"
#include "Cura/Slice.h"

namespace cura
{

Slice::Slice(const size_t num_mesh_groups) : scene(num_mesh_groups)
{
}

void Slice::compute()
{   int count =0;
    for (std::vector<MeshGroup>::iterator mesh_group = scene.mesh_groups.begin(); mesh_group != scene.mesh_groups.end(); mesh_group++)
    {   std::cout << "Run compute round: " << count << std::endl;
        scene.mesh_group_num = count;   // record the dealing mesh_group
        scene.current_mesh_group = mesh_group;  // this is a pointer to point the MeshGroup in the vector.

        for (ExtruderTrain& extruder : scene.extruders){
            extruder.settings.setParent(&scene.current_mesh_group->settings);
        }
        
        scene.processMeshGroup(*mesh_group);
        count++;
    }
}

void Slice::reset()
{
    scene.extruders.clear();
    scene.mesh_groups.clear();
    scene.settings = Settings();
}

} // namespace cura