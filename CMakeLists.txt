cmake_minimum_required(VERSION 3.10)
project(Distributed_Systems_and_Algorithms_Project_MPI_PageRank)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -O0")


#[[find_package( Boost REQUIRED COMPONENTS graph)
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "Boost not found")
endif()]]
find_package(MPI REQUIRED)

link_libraries(mpi)
link_libraries(m)
add_executable(Distributed_Systems_and_Algorithms_Project_MPI_PageRank PR-implementation/pr_mpi_implementation.cpp
        GMLParser/gml_parser.cpp GMLParser/network_graph.cpp)
add_executable(gmltest GMLParser/gml_parser.cpp GMLParser/network_graph.cpp GMLParser/parse_test.cpp)


# Link against MPI
target_link_libraries(Distributed_Systems_and_Algorithms_Project_MPI_PageRank ${MPI_LIBRARIES})

# Include MPI includes
include_directories(${MPI_INCLUDE_PATH})
if(MPI_COMPILE_FLAGS)
    set_target_properties(Distributed_Systems_and_Algorithms_Project_MPI_PageRank PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(Distributed_Systems_and_Algorithms_Project_MPI_PageRank PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

#target_link_libraries(Distributed_Systems_and_Algorithms_Project_MPI_PageRank ${Boost_LIBRARIES})