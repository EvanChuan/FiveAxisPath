# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/evanchuan/Desktop/3DP/FiveAxisPath

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/evanchuan/Desktop/3DP/FiveAxisPath/build

# Include any dependencies generated for this target.
include CMakeFiles/_readfile.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/_readfile.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/_readfile.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/_readfile.dir/flags.make

CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/MCF_Skeleton_example.cpp
CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o -MF CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o.d -o CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/MCF_Skeleton_example.cpp

CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/MCF_Skeleton_example.cpp > CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.i

CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/MCF_Skeleton_example.cpp -o CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.s

CMakeFiles/_readfile.dir/src/sorting.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/sorting.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/sorting.cpp
CMakeFiles/_readfile.dir/src/sorting.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/_readfile.dir/src/sorting.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/sorting.cpp.o -MF CMakeFiles/_readfile.dir/src/sorting.cpp.o.d -o CMakeFiles/_readfile.dir/src/sorting.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/sorting.cpp

CMakeFiles/_readfile.dir/src/sorting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/sorting.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/sorting.cpp > CMakeFiles/_readfile.dir/src/sorting.cpp.i

CMakeFiles/_readfile.dir/src/sorting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/sorting.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/sorting.cpp -o CMakeFiles/_readfile.dir/src/sorting.cpp.s

CMakeFiles/_readfile.dir/src/Function.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/Function.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/Function.cpp
CMakeFiles/_readfile.dir/src/Function.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/_readfile.dir/src/Function.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/Function.cpp.o -MF CMakeFiles/_readfile.dir/src/Function.cpp.o.d -o CMakeFiles/_readfile.dir/src/Function.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/Function.cpp

CMakeFiles/_readfile.dir/src/Function.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/Function.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/Function.cpp > CMakeFiles/_readfile.dir/src/Function.cpp.i

CMakeFiles/_readfile.dir/src/Function.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/Function.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/Function.cpp -o CMakeFiles/_readfile.dir/src/Function.cpp.s

CMakeFiles/_readfile.dir/src/FillHole.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/FillHole.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/FillHole.cpp
CMakeFiles/_readfile.dir/src/FillHole.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/_readfile.dir/src/FillHole.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/FillHole.cpp.o -MF CMakeFiles/_readfile.dir/src/FillHole.cpp.o.d -o CMakeFiles/_readfile.dir/src/FillHole.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/FillHole.cpp

CMakeFiles/_readfile.dir/src/FillHole.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/FillHole.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/FillHole.cpp > CMakeFiles/_readfile.dir/src/FillHole.cpp.i

CMakeFiles/_readfile.dir/src/FillHole.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/FillHole.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/FillHole.cpp -o CMakeFiles/_readfile.dir/src/FillHole.cpp.s

CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/PlaneCut.cpp
CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o -MF CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o.d -o CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/PlaneCut.cpp

CMakeFiles/_readfile.dir/src/PlaneCut.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/PlaneCut.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/PlaneCut.cpp > CMakeFiles/_readfile.dir/src/PlaneCut.cpp.i

CMakeFiles/_readfile.dir/src/PlaneCut.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/PlaneCut.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/PlaneCut.cpp -o CMakeFiles/_readfile.dir/src/PlaneCut.cpp.s

CMakeFiles/_readfile.dir/src/RWfile.cpp.o: CMakeFiles/_readfile.dir/flags.make
CMakeFiles/_readfile.dir/src/RWfile.cpp.o: /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/RWfile.cpp
CMakeFiles/_readfile.dir/src/RWfile.cpp.o: CMakeFiles/_readfile.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/_readfile.dir/src/RWfile.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/_readfile.dir/src/RWfile.cpp.o -MF CMakeFiles/_readfile.dir/src/RWfile.cpp.o.d -o CMakeFiles/_readfile.dir/src/RWfile.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/RWfile.cpp

CMakeFiles/_readfile.dir/src/RWfile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_readfile.dir/src/RWfile.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/RWfile.cpp > CMakeFiles/_readfile.dir/src/RWfile.cpp.i

CMakeFiles/_readfile.dir/src/RWfile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_readfile.dir/src/RWfile.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/RWfile.cpp -o CMakeFiles/_readfile.dir/src/RWfile.cpp.s

# Object files for target _readfile
_readfile_OBJECTS = \
"CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o" \
"CMakeFiles/_readfile.dir/src/sorting.cpp.o" \
"CMakeFiles/_readfile.dir/src/Function.cpp.o" \
"CMakeFiles/_readfile.dir/src/FillHole.cpp.o" \
"CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o" \
"CMakeFiles/_readfile.dir/src/RWfile.cpp.o"

# External object files for target _readfile
_readfile_EXTERNAL_OBJECTS =

lib_readfile.a: CMakeFiles/_readfile.dir/src/MCF_Skeleton_example.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/src/sorting.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/src/Function.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/src/FillHole.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/src/PlaneCut.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/src/RWfile.cpp.o
lib_readfile.a: CMakeFiles/_readfile.dir/build.make
lib_readfile.a: CMakeFiles/_readfile.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library lib_readfile.a"
	$(CMAKE_COMMAND) -P CMakeFiles/_readfile.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_readfile.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/_readfile.dir/build: lib_readfile.a
.PHONY : CMakeFiles/_readfile.dir/build

CMakeFiles/_readfile.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/_readfile.dir/cmake_clean.cmake
.PHONY : CMakeFiles/_readfile.dir/clean

CMakeFiles/_readfile.dir/depend:
	cd /Users/evanchuan/Desktop/3DP/FiveAxisPath/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/evanchuan/Desktop/3DP/FiveAxisPath /Users/evanchuan/Desktop/3DP/FiveAxisPath /Users/evanchuan/Desktop/3DP/FiveAxisPath/build /Users/evanchuan/Desktop/3DP/FiveAxisPath/build /Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles/_readfile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/_readfile.dir/depend

