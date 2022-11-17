// Copyright (c) 2022 Ultimaker B.V.
// CuraEngine is released under the terms of the AGPLv3 or higher

#include <limits>
#include <stdio.h>
#include <string.h>
#include "spdlog/spdlog.h"
#include "Cura/MeshGroup.h"
#include "Cura/mesh.h"
#include "Cura/settings/types/Ratio.h" //For the shrinkage percentage and scale factor.
#include "Cura/utils/FMatrix4x3.h" //To transform the input meshes for shrinkage compensation and to align in command line mode.
#include "Cura/utils/floatpoint.h" //To accept incoming meshes with floating point vertices.
#include "Cura/utils/gettime.h"
#include "Cura/utils/string.h"

namespace cura
{

FILE* binaryMeshBlob = nullptr;

/* Custom fgets function to support Mac line-ends in Ascii STL files. OpenSCAD produces this when used on Mac */

void* fgets_(char* ptr, size_t len, FILE* f)
{
    while (len && fread(ptr, 1, 1, f) > 0)
    {
        if (*ptr == '\n' || *ptr == '\r')
        {
            *ptr = '\0';
            return ptr;
        }
        ptr++;
        len--;
    }
    return nullptr;
}

Point3 MeshGroup::min() const
{
    if (meshes.size() < 1)
    {
        return Point3(0, 0, 0);
    }
    Point3 ret(std::numeric_limits<coord_t>::max(), std::numeric_limits<coord_t>::max(), std::numeric_limits<coord_t>::max());
    for (const Mesh& mesh : meshes)
    {
        /*
        if (mesh.settings.get<bool>("infill_mesh") || mesh.settings.get<bool>("cutting_mesh") || mesh.settings.get<bool>("anti_overhang_mesh")) // Don't count pieces that are not printed.
        {
            continue;
        }
        */
        Point3 v = mesh.min();
        ret.x = std::min(ret.x, v.x);
        ret.y = std::min(ret.y, v.y);
        ret.z = std::min(ret.z, v.z);
    }
    return ret;
}

Point3 MeshGroup::max() const
{
    if (meshes.size() < 1)
    {
        return Point3(0, 0, 0);
    }
    Point3 ret(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min());
    for (const Mesh& mesh : meshes)
    {   
        /*
        if (mesh.settings.get<bool>("infill_mesh") || mesh.settings.get<bool>("cutting_mesh") || mesh.settings.get<bool>("anti_overhang_mesh")) // Don't count pieces that are not printed.
        {
            continue;
        }
        */
        Point3 v = mesh.max();
        ret.x = std::max(ret.x, v.x);
        ret.y = std::max(ret.y, v.y);
        ret.z = std::max(ret.z, v.z);
    }
    return ret;
}

void MeshGroup::clear()
{
    for (Mesh& m : meshes)
    {
        m.clear();
    }
}

void MeshGroup::finalize()
{
    // If the machine settings have been supplied, offset the given position vertices to the center of vertices (0,0,0) is at the bed center.
    Point3 meshgroup_offset(0, 0, 0);
    
    if (! settings.get<bool>("machine_center_is_zero"))
    {
        meshgroup_offset.x = settings.get<coord_t>("machine_width") / 2;
        meshgroup_offset.y = settings.get<coord_t>("machine_depth") / 2;
    }
    
    // If a mesh position was given, put the mesh at this position in 3D space.
    for (Mesh& mesh : meshes)
    {
        Point3 mesh_offset(mesh.settings.get<coord_t>("mesh_position_x"), mesh.settings.get<coord_t>("mesh_position_y"), mesh.settings.get<coord_t>("mesh_position_z"));
        if (mesh.settings.get<bool>("center_object"))
        {
            Point3 object_min = mesh.min();
            Point3 object_max = mesh.max();
            Point3 object_size = object_max - object_min;
            mesh_offset += Point3(-object_min.x - object_size.x / 2, -object_min.y - object_size.y / 2, -object_min.z);
        }
        mesh.offset(mesh_offset + meshgroup_offset);
    }
    scaleFromBottom(settings.get<Ratio>("material_shrinkage_percentage_xy"), settings.get<Ratio>("material_shrinkage_percentage_z")); // Compensate for the shrinkage of the material.
}

void MeshGroup::scaleFromBottom(const Ratio factor_xy, const Ratio factor_z)
{
    const Point3 center = (max() + min()) / 2;
    const Point3 origin(center.x, center.y, 0);

    const FMatrix4x3 transformation = FMatrix4x3::scale(factor_xy, factor_xy, factor_z, origin);
    for (Mesh& mesh : meshes)
    {
        mesh.transform(transformation);
    }
}

bool loadMeshSTL_binary(Mesh* mesh, vector<Tri>& T, const FMatrix4x3& matrix)
{
    size_t face_count = T.size()-1; // the first one which is store cut plane so need to -1.
    // For each face read:
    // float(x,y,z) = normal, float(X,Y,Z)*3 = vertexes, uint16_t = flags
    //  Every Face is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
    mesh->faces.reserve(face_count);
    mesh->vertices.reserve(face_count);
    for (unsigned int i = 1; i < face_count; i++)   // skip the first one which is store at cut plane
    {
        Point3 v0 = matrix.apply(FPoint3((float)T[i].P1_x, (float)T[i].P1_y, (float)T[i].P1_z));
        Point3 v1 = matrix.apply(FPoint3((float)T[i].P2_x, (float)T[i].P2_y, (float)T[i].P2_z));
        Point3 v2 = matrix.apply(FPoint3((float)T[i].P3_x, (float)T[i].P3_y, (float)T[i].P3_z));
        mesh->addFace(v0, v1, v2);
    }
    mesh->finish();
    return true;
}

bool loadMeshIntoMeshGroup_(int Id,MeshGroup* meshgroup,vector<Tri>& current_tri,const FMatrix4x3& transformation,Settings& object_parent_settings,vector<Function::Skel_Points>& plane_points, vector<std::pair<float,float>>& rotated_angle)
{
    TimeKeeper load_timer;

    Mesh mesh(object_parent_settings);
    if(!loadMeshSTL_binary(&mesh, current_tri, transformation)){ return false;} // Return on failure, otherwise continue.
    meshgroup->meshes.push_back(mesh);
    
    if(Id < plane_points.size())
    {
    // calculate the rotated cutplan point and stored in the Id th MeshGroup.
    std::cout << "orig cutplan_p.z : " << plane_points[Id].Z << std::endl;
    Point3 cutplan_p = transformation.apply(Point3((float)plane_points[Id].X, (float)plane_points[Id].Y, (float)plane_points[Id].Z));
    std::cout << "cutplan_p.z : " << cutplan_p.z << std::endl;
    meshgroup->rotated_Zvaule = cutplan_p.z;
    }

    // stored the A,C angle which calculate before in the Id th MeshGroup.
    std::cout << "rotated_angle : " << rotated_angle[Id].first << " " << rotated_angle[Id].second << std::endl;
    meshgroup->angle = rotated_angle[Id];
    //spdlog::info("loading data took {:3} seconds", load_timer.restart());
    
    return true;
}


bool loadMeshIntoMeshGroup(MeshGroup* meshgroup, const char* filename, const FMatrix4x3& transformation, Settings& object_parent_settings)
{
    TimeKeeper load_timer;

    const char* ext = strrchr(filename, '.');
    if (ext && (strcmp(ext, ".stl") == 0 || strcmp(ext, ".STL") == 0))
    {
        /*
        Mesh mesh(object_parent_settings);
        if (loadMeshSTL(&mesh, filename, transformation)) // Load it! If successful...
        {
            meshgroup->meshes.push_back(mesh);
            spdlog::info("loading '{}' took {:3} seconds", filename, load_timer.restart());
            return true;
        }
        */
    }
    spdlog::warn("Unable to recognize the extension of the file. Currently only .stl and .STL are supported.");
    return false;
}


} // namespace cura
