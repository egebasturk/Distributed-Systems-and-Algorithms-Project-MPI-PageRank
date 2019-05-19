#pragma once
#include "network_graph.h"

namespace GMLParser
{
using namespace GraphStructure;
GraphAdjList
CreateAdjListFromFile(const char* filename);

}
