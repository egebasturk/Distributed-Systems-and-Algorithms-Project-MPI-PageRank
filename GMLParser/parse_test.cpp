#include "gml_parser.h"
#include <stdio.h>

int
main()
{
    auto x = GMLParser::CreateAdjListFromFile("test_gml.txt");
    x.print_graph_original_rep();
    printf("\n##################\n\n");
    x.print_graph_internal_rep();
    int a = x.get_vertex_index(7);
    printf("%d",a);
    getchar();
    return 0;
}
