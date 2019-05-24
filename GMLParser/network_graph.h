#pragma once
#include <vector>
#include <inttypes.h>
#include <unordered_map>

namespace GraphStructure
{

struct Vertex
{
    int id_;
    int out_degree_;
};

struct GraphAdjList
{
    /* Look up table for node id to index mapping */
    std::unordered_map<int, int> node_id_lut;
    /* Vertex information storage. It holds original vertex ids and out_degrees */
    std::vector<Vertex> vertex_ids;
    /* adjacency list that stores incoming edges */
    std::vector<std::vector<int>> adj_list;

    int add_vertex(const int vertex_id);
    void add_incoming_edge(const int src_id, const int dst_id);
    bool is_in_lut(int vertex_id);
    int get_vertex_index(const int vertex_id);
    void print_graph_original_rep();
    void print_graph_internal_rep();
};
}
