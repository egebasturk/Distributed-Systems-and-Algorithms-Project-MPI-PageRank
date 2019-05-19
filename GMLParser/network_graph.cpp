#include "network_graph.h"
namespace GraphStructure
{
int
GraphAdjList::add_vertex(const int vertex_id)
{
    if (!is_in_lut(vertex_id))
    {
        node_id_lut[vertex_id] = adj_list.size();
        vertex_ids.push_back(vertex_id);
        adj_list.push_back(std::vector<int>{});
        return adj_list.size() - 1;
    }
    return node_id_lut[vertex_id];
}

void
GraphAdjList::add_edge_to_vertex_index(const int index, const int dst_id)
{
    int vertex_index = add_vertex(dst_id);
    adj_list[index].push_back(vertex_index);
}

void
GraphAdjList::add_edge(const int src_id, const int dst_id)
{
    int dst_index = add_vertex(dst_id);
    int src_index = add_vertex(src_id);
    adj_list[src_index].push_back(dst_index);
}

bool
GraphAdjList::is_in_lut(int vertex_id)
{
    auto iterator = node_id_lut.find(vertex_id);
    return iterator != node_id_lut.end();
}

int
GraphAdjList::get_vertex_index(const int vertex_id)
{
    if (!is_in_lut(vertex_id))
    {
        return -1;
    }
    return node_id_lut[vertex_id];
}

void
GraphAdjList::print_graph_original_rep()
{
    printf("Graph original representation: \n");
    for (unsigned int i = 0; i < adj_list.size(); i++)
    {
        int x = vertex_ids[i];
        printf("%d -> ", x);
        for (auto& vertex : adj_list[i])
        {
            printf("%d, ", vertex_ids[vertex]);
        }
        printf("\n");
    }
}

void
GraphAdjList::print_graph_internal_rep()
{
    printf("Graph internal representation: \n");
    for (unsigned int i = 0; i < adj_list.size(); i++)
    {
        printf("%d -> ", i);
        for (auto& vertex : adj_list[i])
        {
            printf("%d, ", vertex);
        }
        printf("\n");
    }
}
}
