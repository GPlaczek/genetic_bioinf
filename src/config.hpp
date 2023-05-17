#pragma once

#include <utility>
#include <string>
#include <memory>

const int DEFAULT_N_GENERATIONS = 1500;
const int DEFAULT_POP_SIZE = 1000;
const int DEFAULT_N_SHUFFLES = 10;

const float DEFAULT_PERCENT_WINNERS = 0.10f;
const float DEFAULT_CUT_RANGE = 1.0f;
const float DEFAULT_MUTATION_CHANCE = 0.1f;

const bool DEFAULT_PRECISE = false;
const bool DEFAULT_DIRTY = true;

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
        bool dirty;
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
