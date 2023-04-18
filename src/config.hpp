#pragma once

#include <utility>
#include <string>

class Config {
    struct Population { int size; };
    struct Tournament { int nWinners; };
    struct Mixing {
        bool precise;
        int cutRange;
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

    Config(std::string path);
};
