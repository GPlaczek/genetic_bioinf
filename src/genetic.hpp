#pragma once

#include "config.hpp"
#include "instance.hpp"
#include <random>

class Genetic {
    Config config;
    Instance instance;
    std::mt19937 rng;
    void combine(
        std::unique_ptr<Shuffle> &in1,
        std::unique_ptr<Shuffle> &in2,
        std::unique_ptr<Shuffle> &out1,
        std::unique_ptr<Shuffle> &out2);
public:
    Genetic();
    Genetic(Config config, Instance instance);
    void run(bool parallel);
};
