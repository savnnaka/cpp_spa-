#include "random_walk_graph.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <ios>
#include <numeric>
#include <random>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>

void RandomWalkGraph::simulate_random_walk(uint32_t num_steps) {
    // uint32_t size = this->size();
    // std::uniform_int_distribution<uint32_t> dice {0, size};
    // uint32_t start = dice(prng);
    // float weightSum = 0;
    // for (size_t i = 0; i < size; i++){
    //     weightSum += edges[start][i].second;
    // }
    // float sum = 0;
    // for (size_t i=0; i < size; i++){

    // }
 }

void RandomWalkGraph::write_histogram_pgm(const std::string& filename, uint32_t width, uint32_t height) const {
    // TODO: 10.3 b)
}
