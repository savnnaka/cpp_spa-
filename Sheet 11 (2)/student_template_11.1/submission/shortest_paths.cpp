#include "shortest_paths.h"
#include <algorithm>
#include <cstddef>
#include <math.h>
#include <optional>
#include <utility>
#include <iostream>
#include <cmath>
#include <queue>
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
    struct myComp {
    constexpr bool operator()(
        std::pair<size_t, float> const& a,
        std::pair<size_t, float> const& b)
        const noexcept
    {
        return a.second > b.second;
    }
    };
    std::priority_queue<std::pair<size_t, float>, std::vector<std::pair<size_t, float>>, myComp> toExplore;

    // Points that have already been visited
    std::vector<bool> visited;
    visited.resize(SIZE, false);

    // distances and predecessors
    std::vector<float> dists(SIZE, INFINITY);
    dists.at(from) = 0;
    std::vector<std::optional<size_t>> predecessors;
    predecessors.resize(SIZE);

    // auto compare = [&dists](size_t a, size_t b) {return dists.at(a) > dists.at(b);};
    toExplore.push(std::make_pair(from, 0.0f));

    std::vector<bool> inToExplore;
    inToExplore.resize(SIZE, false);

    std::vector<float> heuristics;
    heuristics.resize(SIZE, 0.0f);
    Location destination = this->at(to);
    float destinationX = destination.pos_x;
    float destinationY = destination.pos_y;
    for(size_t i=0; i<SIZE; i++){
        Location elemI = this->at(i);
        float h = powf((powf((destinationX - elemI.pos_x),2.0f)+powf((destinationY-elemI.pos_y),2.0f)), 0.5f);
        heuristics.at(i) = h;
    }

    while( !toExplore.empty() ){
        // fetch the point with the shortest distance from the startpoint & remove from queue
        size_t elem = toExplore.top().first;
        toExplore.pop();
        // element is now visited
        ++num_visited;
        // if elem is the goal, terminate
        if(elem == to){
            break;
        }

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

                    if ( !inToExplore.at(i) ) {
                        toExplore.push(std::make_pair(i, dists.at(i)+heuristics.at(i)));
                        inToExplore.at(i) = true;
                    }
                }
            }
        }

    }
    
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
