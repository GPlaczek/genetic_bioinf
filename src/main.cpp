#include <iostream>
#include <getopt.h>
#include <memory>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <random>

#include "config.hpp"
#include "instance.hpp"

const static struct option long_opts[] = {
    {"config", required_argument, NULL, 'c'},
    {0, 0, 0, 0},
};

void genetic(
    Config &config,
    Instance &instance
) {
    std::default_random_engine rng{};
    std::vector<std::vector<int>> population(
        config.population.size,
        std::vector<int>(instance.getNWords()));

    // Generate initial population
    for (
        std::vector<std::vector<int>>::iterator it = population.begin();
        it < population.end();
        it++
    ) {
        std::iota(it->begin(), it->end(), 0);
        std::shuffle(it->begin(), it->end(), rng);
    }
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
