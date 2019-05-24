#include "gml_parser.h"
#include <stdio.h>

int
main()
{
    GraphStructure::GraphAdjList graph = GMLParser::CreateAdjListFromFile("test_gml.txt");
    graph.print_graph_original_rep();
    printf("\n##################\n\n");
    graph.print_graph_internal_rep();
    getchar();
    return 0;
}
