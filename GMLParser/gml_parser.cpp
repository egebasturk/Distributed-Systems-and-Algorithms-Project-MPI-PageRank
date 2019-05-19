#include "gml_parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

namespace GMLParser
{
GraphAdjList
CreateAdjListFromFile(const char* filename)
{
    GraphAdjList graph;
    int last_inserted_vertex_index = -1;
    int last_inserted_vertex_id = -1;
    std::ifstream infile;
    infile.open(filename);

    if (infile.fail())
    {
        std::cout << "FILE NOT FOUND!" << std::endl;
        assert(0);
    }

    std::string line;
    while (std::getline(infile, line))
    {
        if (line.c_str()[0] == '#')
            continue;
        std::istringstream iss(line);
        int src, dst;
        if (!(iss >> src >> dst))
        {
            std::cout << "FILE FORMAT IS BROKEN!" << std::endl;
            assert(0);
        }

        if (last_inserted_vertex_id != src)
        {
            last_inserted_vertex_id = src;
            last_inserted_vertex_index = graph.add_vertex(src);
        }

        graph.add_edge_to_vertex_index(last_inserted_vertex_index, dst);
    }
    return graph;
}
}
