# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.23.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.23.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/evanchuan/Desktop/3DP/FiveAxisPath

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/evanchuan/Desktop/3DP/FiveAxisPath/build

# Include any dependencies generated for this target.
include CMakeFiles/refine_fair_example.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/refine_fair_example.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/refine_fair_example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/refine_fair_example.dir/flags.make

CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o: CMakeFiles/refine_fair_example.dir/flags.make
CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o: ../src/refine_fair_example.cpp
CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o: CMakeFiles/refine_fair_example.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o -MF CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o.d -o CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o -c /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/refine_fair_example.cpp

CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/refine_fair_example.cpp > CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.i

CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/evanchuan/Desktop/3DP/FiveAxisPath/src/refine_fair_example.cpp -o CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.s

# Object files for target refine_fair_example
refine_fair_example_OBJECTS = \
"CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o"

# External object files for target refine_fair_example
refine_fair_example_EXTERNAL_OBJECTS =

refine_fair_example: CMakeFiles/refine_fair_example.dir/src/refine_fair_example.cpp.o
refine_fair_example: CMakeFiles/refine_fair_example.dir/build.make
refine_fair_example: /usr/local/lib/libgmpxx.dylib
refine_fair_example: /usr/local/lib/libmpfr.dylib
refine_fair_example: /usr/local/lib/libgmp.dylib
refine_fair_example: CMakeFiles/refine_fair_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable refine_fair_example"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/refine_fair_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/refine_fair_example.dir/build: refine_fair_example
.PHONY : CMakeFiles/refine_fair_example.dir/build

CMakeFiles/refine_fair_example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/refine_fair_example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/refine_fair_example.dir/clean

CMakeFiles/refine_fair_example.dir/depend:
	cd /Users/evanchuan/Desktop/3DP/FiveAxisPath/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/evanchuan/Desktop/3DP/FiveAxisPath /Users/evanchuan/Desktop/3DP/FiveAxisPath /Users/evanchuan/Desktop/3DP/FiveAxisPath/build /Users/evanchuan/Desktop/3DP/FiveAxisPath/build /Users/evanchuan/Desktop/3DP/FiveAxisPath/build/CMakeFiles/refine_fair_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/refine_fair_example.dir/depend

