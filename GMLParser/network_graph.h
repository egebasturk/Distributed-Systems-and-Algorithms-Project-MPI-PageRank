#pragma once
#include <vector>
#include <inttypes.h>
#include <unordered_map>

namespace GraphStructure
{

struct GraphAdjList
{
    std::unordered_map<int, int> node_id_lut;
    std::vector<int> vertex_ids;
    std::vector<std::vector<int>> adj_list;
    int add_vertex(const int vertex_id);
    void add_edge_to_vertex_index(const int index, const int dst_id);
    void add_edge(const int src_id, const int dst_id);
    bool is_in_lut(int vertex_id);
    int get_vertex_index(const int vertex_id);
    void print_graph_original_rep();
    void print_graph_internal_rep();
};
}
