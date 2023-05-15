#pragma once

#include "config.hpp"
#include "instance.hpp"
#include <random>

class Genetic {
    Config config;
    Instance instance;
    std::mt19937 rng;
    void combine(Shuffle &in1, Shuffle &in2, Shuffle &out1, Shuffle &out2);
public:
    Genetic();
    Genetic(Config config, Instance instance);
    void run(bool parallel);
};
