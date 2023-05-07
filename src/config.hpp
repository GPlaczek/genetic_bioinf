#pragma once

#include <utility>
#include <string>
#include <memory>

const int DEFAULT_N_GENERATIONS = 2000;
const int DEFAULT_POP_SIZE = 200;
const int DEFAULT_N_SHUFFLES = 8;

const float DEFAULT_PERCENT_WINNERS = 0.15f;
const float DEFAULT_CUT_RANGE = 0.20f;
const float DEFAULT_MUTATION_CHANCE = 0.05f;

const bool DEFAULT_PRECISE = false;

class Config {
    struct Population { int size; };
    struct Tournament { float percentWinners; };
    struct Mixing {
        bool precise;
        float cutRange;
    };
    struct Mutation {
        float chance;
        int nShuffles;
    };

public:
    int nGenerations;

    Population population;
    Tournament tournament;
    Mixing mixing;
    Mutation mutation;

    Config();
    Config(std::istream &source);
};
