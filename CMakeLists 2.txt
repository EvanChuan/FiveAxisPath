cmake_minimum_required(VERSION 3.1...3.22)
project(fiveaxis)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)
set(CMAKE_CXX_STANDARD 17)

# CGAL and its components
find_package( CGAL REQUIRED)
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  
endif()
# Don't let CGAL override flags
set(CGAL_DONT_OVERRIDE_CMAKE_FLAGS
    TRUE
    CACHE BOOL "Force CGAL to maintain CMAKE flags")
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

# Boost and its components
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()

find_package(Eigen3 3.2.0)
include(CGAL_Eigen3_support)

#find_package(clipper REQUIRED)

set(SRCS
  src/clipper.cpp
  src/CuraSlicing.cpp
  src/FillHole.cpp 
  src/Function.cpp
  src/MCF_Skeleton_example.cpp
  src/PlaneCut.cpp
  src/RWfile.cpp
  #src/Slicing.cpp
  src/sorting.cpp
  src/TransformCalcu.cpp
  
  src/Cura/Application.cpp
  src/Cura/bridge.cpp
  src/Cura/ConicalOverhang.cpp
  src/Cura/ExtruderTrain.cpp
  src/Cura/FffGcodeWriter.cpp
  src/Cura/FffPolygonGenerator.cpp
  src/Cura/FffProcessor.cpp
  src/Cura/gcodeExport.cpp
  src/Cura/GCodePathConfig.cpp
  src/Cura/infill.cpp
  src/Cura/InsetOrderOptimizer.cpp
  src/Cura/layerPart.cpp
  src/Cura/LayerPlan.cpp
  src/Cura/LayerPlanBuffer.cpp
  src/Cura/mesh.cpp
  src/Cura/MeshGroup.cpp
  src/Cura/Mold.cpp
  src/Cura/multiVolumes.cpp
  src/Cura/PathOrderPath.cpp
  src/Cura/Preheat.cpp
  src/Cura/PrimeTower.cpp
  src/Cura/raft.cpp
  src/Cura/Scene.cpp
  src/Cura/SkeletalTrapezoidation.cpp
  src/Cura/SkeletalTrapezoidationGraph.cpp
  src/Cura/skin.cpp
  src/Cura/SkirtBrim.cpp
  src/Cura/SupportInfillPart.cpp
  src/Cura/Slice.cpp
  src/Cura/sliceDataStorage.cpp
  src/Cura/slicer.cpp
  src/Cura/support.cpp
  src/Cura/timeEstimate.cpp
  src/Cura/TopSurface.cpp
  src/Cura/TreeModelVolumes.cpp
  src/Cura/TreeSupport.cpp
  src/Cura/WallsComputation.cpp
  src/Cura/Weaver.cpp
  src/Cura/Wireframe2gcode.cpp
  src/Cura/WallToolPaths.cpp

  src/Cura/BeadingStrategy/BeadingStrategy.cpp
  src/Cura/BeadingStrategy/BeadingStrategyFactory.cpp
  src/Cura/BeadingStrategy/DistributedBeadingStrategy.cpp
  src/Cura/BeadingStrategy/LimitedBeadingStrategy.cpp
  src/Cura/BeadingStrategy/RedistributeBeadingStrategy.cpp
  src/Cura/BeadingStrategy/WideningBeadingStrategy.cpp
  src/Cura/BeadingStrategy/OuterWallInsetBeadingStrategy.cpp

  src/Cura/communication/ArcusCommunication.cpp
  src/Cura/communication/ArcusCommunicationPrivate.cpp
  src/Cura/communication/CommandLine.cpp
  src/Cura/communication/Listener.cpp
  
  src/Cura/infill/ImageBasedDensityProvider.cpp
  src/Cura/infill/NoZigZagConnectorProcessor.cpp
  src/Cura/infill/ZigzagConnectorProcessor.cpp
  src/Cura/infill/LightningDistanceField.cpp
  src/Cura/infill/LightningGenerator.cpp
  src/Cura/infill/LightningLayer.cpp
  src/Cura/infill/LightningTreeNode.cpp
  src/Cura/infill/SierpinskiFill.cpp
  src/Cura/infill/SierpinskiFillProvider.cpp
  src/Cura/infill/SubDivCube.cpp
  src/Cura/infill/GyroidInfill.cpp

  src/Cura/pathPlanning/Comb.cpp
  src/Cura/pathPlanning/GCodePath.cpp
  src/Cura/pathPlanning/LinePolygonsCrossings.cpp
  src/Cura/pathPlanning/NozzleTempInsert.cpp
  src/Cura/pathPlanning/TimeMaterialEstimates.cpp

  src/Cura/progress/Progress.cpp
  src/Cura/progress/ProgressStageEstimator.cpp

  src/Cura/settings/AdaptiveLayerHeights.cpp
  src/Cura/settings/FlowTempGraph.cpp
  src/Cura/settings/PathConfigStorage.cpp
  src/Cura/settings/Settings.cpp
  src/Cura/settings/ZSeamConfig.cpp

  src/Cura/utils/AABB.cpp
  src/Cura/utils/AABB3D.cpp
  src/Cura/utils/Date.cpp
  src/Cura/utils/ExtrusionJunction.cpp
  src/Cura/utils/ExtrusionLine.cpp
  src/Cura/utils/ExtrusionSegment.cpp
  src/Cura/utils/FMatrix4x3.cpp
  src/Cura/utils/gettime.cpp
  src/Cura/utils/LinearAlg2D.cpp
  src/Cura/utils/ListPolyIt.cpp
  src/Cura/utils/MinimumSpanningTree.cpp
  src/Cura/utils/Point3.cpp
  src/Cura/utils/PolygonConnector.cpp
  src/Cura/utils/PolygonsPointIndex.cpp
  src/Cura/utils/PolygonsSegmentIndex.cpp
  src/Cura/utils/polygonUtils.cpp
  src/Cura/utils/polygon.cpp
  src/Cura/utils/PolylineStitcher.cpp
  src/Cura/utils/ProximityPointLink.cpp
  src/Cura/utils/Simplify.cpp
  src/Cura/utils/SVG.cpp
  src/Cura/utils/socket.cpp
  src/Cura/utils/SquareGrid.cpp
  src/Cura/utils/ThreadPool.cpp
  src/Cura/utils/ToolpathVisualizer.cpp
  src/Cura/utils/VoronoiUtils.cpp
)

add_library(_readfile STATIC ${SRCS})

target_include_directories(_readfile
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(_readfile 
    CGAL::CGAL 
    CGAL::Eigen3_support
    )

add_executable(readfile
  src/main.cpp
  ${_readfile}
)
target_link_libraries(readfile PRIVATE _readfile)  # add CGAL Library to executable file
#target_link_libraries(readfile CGAL::CGAL CGAL::Eigen3_support)  # add this line to link CGAL lib and Eigen lib to target project