#include <iostream>
#include <getopt.h>
#include <memory>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <random>

#include "config.hpp"
#include "shuffle.hpp"
#include "instance.hpp"

const static struct option long_opts[] = {
    {"config", required_argument, NULL, 'c'},
    {0, 0, 0, 0},
};

void genetic(
    Config &config,
    Instance &instance
) {
    int nWinners = (int)(config.tournament.percentWinners * config.population.size);
    std::default_random_engine rng{};

    std::vector<Shuffle> population(
        config.population.size,
        Shuffle(instance.getNWords()));

    std::vector<Shuffle> work(
        config.population.size,
        Shuffle(instance.getNWords()));

    // Generate initial population
    for (
        std::vector<Shuffle>::iterator it = population.begin();
        it < population.end();
        it++
    ) {
        std::iota(it->indices.begin(), it->indices.end(), 0);
        std::shuffle(it->indices.begin(), it->indices.end(), rng);
    }

    // Evaluate the population
    for (Shuffle &s : population)
        instance.evaluate(s);

    // Pick strongest elements
    // all the strongest elements are contained between itStrongest and population.end() iterators
    auto itStrongest = population.end() - nWinners;
    std::partial_sort(population.begin(), itStrongest, population.end());
}

int main(int argc, char *argv[]) {
    std::unique_ptr<std::istream> in_cfg;
    std::unique_ptr<std::istream> in_inst;

    int next_option;
    while (optind < argc) {
        if ((next_option = getopt_long(argc, argv, "c:", long_opts, NULL)) != -1) {
            switch (next_option) {
            case 'c':
                in_cfg.reset(new std::ifstream(optarg));
                break;
            default:
                return 1;
            }
        } else {
            if (in_inst == nullptr) {
                in_inst.reset(new std::ifstream(argv[optind]));
            } else {
                std::cerr << "Too many instance files specified" << std::endl;
                return 1;
            }
            optind++;
        }
    }
    Config c(std::move(in_cfg));

    Instance i(std::move(in_inst), 10, 5);

    genetic(c, i);

    return 0;
}
