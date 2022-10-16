#include "shortest_paths.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <optional>
#include <utility>
#include <iostream>
#include <cmath>
#include <vector>

size_t ShortestPaths::getNodeIdByName(const std::string& name) const {
    // NOTE: if you like, you can make this more efficient by caching the mapping in a mutable hash map that gets reset when calling non-const functions
    const auto it = std::find_if(adjacency_matrix.begin(), adjacency_matrix.end(), [=](const Location& row) -> bool { return row.name == name; });
    if (it == adjacency_matrix.end())
        throw std::runtime_error("Location "+name+" not found");
    return static_cast<size_t>(std::distance(adjacency_matrix.begin(), it));
}

std::vector<size_t> ShortestPaths::compute_shortest_path(size_t from, size_t to) const
{
    /// your result path
    std::vector<size_t> result;
    /// increment this for every node that you pop from the queue
    size_t num_visited = 0;

    // TODO: your code here
    size_t SIZE = this->size();
    Location fromL = this->at(from);

    // Queue toExplore
    // std::vector<size_t> *toExplore = new std::vector<size_t>{};
    std::vector<size_t> toExplore;

    // Points that have already been visited
    std::vector<bool> visited;
    visited.resize(SIZE, false);

    // distances and predecessors
    std::vector<float> dists(SIZE, INFINITY);
    dists.at(from) = 0;
    std::vector<std::optional<size_t>> predecessors;
    predecessors.resize(SIZE);

    auto compare = [&dists](size_t a, size_t b) {return dists.at(a) > dists.at(b);};
    toExplore.push_back(from);
    push_heap(toExplore.begin(), toExplore.end());
    //std::make_heap(toExplore.begin(), toExplore.end(), compare);
    // std::make_heap(toExplore->begin(), toExplore->end(), compare);

    while( !toExplore.empty() ){
        // fetch the point with the shortest distance from the startpoint & remove from queue
        // std::pop_heap(toExplore->begin(), toExplore->end(), compare);
        // size_t elem = toExplore->back();
        // toExplore->pop_back();

        // alt:
        size_t elem = toExplore.front();
        pop_heap(toExplore.begin(), toExplore.end());
        toExplore.pop_back();
        push_heap(toExplore.begin(), toExplore.end(), compare);
        // element is now visited
        ++num_visited;
        // if elem is the goal, terminate
        if(elem == to){
            break;
        }
        //std::erase(toExplore, elem);
        visited.at(elem) = true;
        // who are the neighbours of elem
        Location elemL = this->at(elem);
        // loop through the whole adj matrix
        for(size_t i=0; i<SIZE; i++){
            // if there's a value stored, it's a neighbour
            if(elemL.at(i).has_value()){
                // add to queue
                // update shortest paths to ith neighbour & set predecessor
                // ordered by min dists value
                if( !visited.at(i) ){

                    float newDist = elemL.at(i).value() + dists.at(elem);
                    if( (dists.at(i) > newDist) ){
                        dists.at(i) = newDist;
                        predecessors.at(i) = elem;
                    }
                    //toExplore.push_back(i);
                    //std::push_heap(toExplore->begin(), toExplore->end(), compare);
                    
                    // toExplore->push_back(i);
                    // std::make_heap(toExplore->begin(), toExplore->end(), compare);

                    toExplore.push_back(i);
                    push_heap(toExplore.begin(), toExplore.end(), compare);
                }
            }
        }

    }
    //delete toExplore;
    
    // trace back the path
    if( predecessors.at(to).has_value() ){
        size_t elem = to;
        while( elem != from ){
            result.push_back(elem);
            elem = predecessors.at(elem).value();
        }
        result.push_back(from);
    } 
    
    std::cout << "Distance: " << dists.at(to) << std::endl;
    std::cout << "Nodes visited: " << num_visited << std::endl;

    std::reverse(result.begin(), result.end());
    return result;
}

