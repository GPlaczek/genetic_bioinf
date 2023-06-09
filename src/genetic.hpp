#pragma once

#include "config.hpp"
#include "instance.hpp"
#include <random>

class Genetic {
    Config config;
    Instance instance;
    std::mt19937 rng;
    bool stop;
    void combine(const Shuffle &in1, const Shuffle &in2, Shuffle &out1, Shuffle &out2);
    void __mutatePop(std::vector<Shuffle> &pop, bool parallel);
    void mutate(Shuffle &in);
public:
    Genetic();
    Genetic(Config config, Instance instance);
    std::vector<Shuffle> run(bool parallel);
    void setStop();
};
