//Copyright (C) 2020 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#ifndef MESH_GROUP_H
#define MESH_GROUP_H

#include "Cura/mesh.h"
#include "Cura/utils/NoCopy.h"
#include "Cura/settings/Settings.h"
#include "Cura/sliceDataStorage.h"
#include "Function.h"

namespace cura
{

class FMatrix4x3;

/*!
 * A MeshGroup is a collection with 1 or more 3D meshes.
 * 
 * One MeshGroup is a whole which is printed at once.
 * Generally there is one single MeshGroup, though when using one-at-a-time printing, multiple MeshGroups are processed consecutively.
 */
class MeshGroup : public NoCopy
{
public:
    std::vector<Mesh> meshes;
    coord_t rotated_Zvaule;
    std::pair<float,float> angle;
    Settings settings;

    Point3 min() const; //! minimal corner of bounding box
    Point3 max() const; //! maximal corner of bounding box

    void clear();

    void finalize();

    /*!
     * Scale the entire mesh group, with the bottom center as origin point.
     *
     * The mesh group is scaled around the bottom center of its bounding box. So
     * that's the center in the X and Y directions, but Z=0. This simulates the
     * shrinkage while sticking to the build plate.
     */
    void scaleFromBottom(const Ratio factor_xy, const Ratio factor_z);
};

/*!
 * Load a Mesh from file and store it in the \p meshgroup.
 * 
 * \param meshgroup The meshgroup where to store the mesh
 * \param current_tri The data of the mesh file
 * \param transformation The transformation applied to all vertices
 * \return whether the file could be loaded
 */
bool loadMeshIntoMeshGroup_(int Id,MeshGroup* meshgroup, vector<Tri>& current_tri, const FMatrix4x3& transformation, Settings& object_parent_settings,vector<Function::Skel_Points>& plane_points, vector<std::pair<float,float>>& rotated_angle);

bool loadMeshIntoMeshGroup(MeshGroup* meshgroup, const char* filename, const FMatrix4x3& transformation, Settings& object_parent_settings);


} //namespace cura

#endif //MESH_GROUP_H
