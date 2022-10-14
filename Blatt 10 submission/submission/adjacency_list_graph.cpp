#include "adjacency_list_graph.h"

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>

void detail::AdjacencyListGraphBase::add_edge(uint32_t from, uint32_t to, float weight)
{
    // TODO: task 10.1 a)
    size_t size_nodes = edges.size();
    if (from >= size_nodes) edges.resize(from+1);
    size_t size_edges = edges[from].size();
    for (size_t i = 0; i < size_edges; i++){
        if(edges[from][i].first == to){
            throw std::runtime_error("edge already exists");
            return;
        }
    }
    edges[from].resize(size_edges + 1);
    edges[from][size_edges] = std::make_pair(to,weight);
}

void detail::AdjacencyListGraphBase::remove_edge(uint32_t from, uint32_t to)
{
    // TODO: task 10.1 b)
    if(edges.size() > from){
        size_t size = edges[from].size();
        for (size_t i=0; i < size; i++){
            if(edges[from][i].first == to){
                for (size_t j=i; j < size -1; j++){
                    edges[from][j] = edges[from][j+1];
                }
                edges[from].pop_back();
                return;
            }
        }
    }
    throw std::runtime_error("edge does not exist");
}

std::optional<float> detail::AdjacencyListGraphBase::get_edge(uint32_t from, uint32_t to) const
{
    // TODO: task 10.1 c)
    if (edges.size() > from){
        std::vector<std::pair<uint32_t, float>> node = edges[from];
        for (size_t i = 0; i < edges[from].size(); i++){
            if(edges[from][i].first == to) return edges[from][i].second;
        }
    }
    return std::optional<float>{};
}

const std::vector<std::pair<uint32_t, float>>& detail::AdjacencyListGraphBase::get_edges_starting_at(uint32_t node) const
{
    // TODO: task 10.1 d)
    if (node < edges.size() && ! edges[node].empty())
        return edges[node];
    else {
        static std::vector<std::pair<uint32_t,float>> empty = std::vector<std::pair<uint32_t,float>>();
        return empty;
    }
}
