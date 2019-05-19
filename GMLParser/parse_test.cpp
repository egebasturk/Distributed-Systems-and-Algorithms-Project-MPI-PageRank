#include "gml_parser.h"
#include <stdio.h>

int
main()
{
    volatile auto x = GMLParser::CreateAdjListFromFile("web-Google.txt");
    // x.print_graph_original_rep();
    // printf("\n##################\n\n");
    // x.print_graph_internal_rep();
    getchar();
    return 0;
}
