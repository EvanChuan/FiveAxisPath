
cmake_minimum_required(VERSION 3.1...3.22)
project(fiveaxis)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

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

# include for local directory

# include for local package


add_executable(readfile
  src/MCF_Skeleton.h
  src/MCF_Skeleton_example.cpp
  src/readfile_test.cpp
)

target_link_libraries(readfile CGAL::CGAL CGAL::Eigen3_support)  # add this line to link CGAL lib and Eigen lib to target project