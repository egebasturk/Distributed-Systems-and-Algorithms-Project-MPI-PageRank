# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/oz/Downloads/clion-2019.1.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/oz/Downloads/clion-2019.1.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/gmltest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gmltest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gmltest.dir/flags.make

CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o: CMakeFiles/gmltest.dir/flags.make
CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o: ../GMLParser/gml_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o -c /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/gml_parser.cpp

CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/gml_parser.cpp > CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.i

CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/gml_parser.cpp -o CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.s

CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o: CMakeFiles/gmltest.dir/flags.make
CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o: ../GMLParser/network_graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o -c /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/network_graph.cpp

CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/network_graph.cpp > CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.i

CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/network_graph.cpp -o CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.s

CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o: CMakeFiles/gmltest.dir/flags.make
CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o: ../GMLParser/parse_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o -c /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/parse_test.cpp

CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/parse_test.cpp > CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.i

CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/GMLParser/parse_test.cpp -o CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.s

# Object files for target gmltest
gmltest_OBJECTS = \
"CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o" \
"CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o" \
"CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o"

# External object files for target gmltest
gmltest_EXTERNAL_OBJECTS =

gmltest: CMakeFiles/gmltest.dir/GMLParser/gml_parser.cpp.o
gmltest: CMakeFiles/gmltest.dir/GMLParser/network_graph.cpp.o
gmltest: CMakeFiles/gmltest.dir/GMLParser/parse_test.cpp.o
gmltest: CMakeFiles/gmltest.dir/build.make
gmltest: CMakeFiles/gmltest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable gmltest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmltest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gmltest.dir/build: gmltest

.PHONY : CMakeFiles/gmltest.dir/build

CMakeFiles/gmltest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gmltest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gmltest.dir/clean

CMakeFiles/gmltest.dir/depend:
	cd /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug /home/oz/repos/Distributed-Systems-and-Algorithms-Project-MPI-PageRank/cmake-build-debug/CMakeFiles/gmltest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gmltest.dir/depend
