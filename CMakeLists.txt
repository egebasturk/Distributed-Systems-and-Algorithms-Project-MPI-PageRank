cmake_minimum_required(VERSION 3.10)
project(Distributed_Systems_and_Algorithms_Project_MPI_PageRank)

set(CMAKE_CXX_STANDARD 17)


#[[find_package( Boost REQUIRED COMPONENTS graph)
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "Boost not found")
endif()]]


add_executable(Distributed_Systems_and_Algorithms_Project_MPI_PageRank main.cpp Util.cpp Util.h)

#target_link_libraries(Distributed_Systems_and_Algorithms_Project_MPI_PageRank ${Boost_LIBRARIES})