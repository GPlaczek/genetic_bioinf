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

class Genetic {
    Config *config;
    Instance *instance;
    int nWinners;
    std::mt19937 rng;
public:
    Genetic(Config &config, Instance &instance) {
        this->config = &config;
        this->instance = &instance;
        this->nWinners = (int)(this->config->tournament.percentWinners * this->config->population.size);
        std::random_device rd;
        this->rng = std::mt19937(rd());
    };

    void run() {
        std::vector<Shuffle> population(
            this->config->population.size,
            Shuffle(this->instance->getNWords()));

        std::vector<Shuffle> work(
            this->config->population.size,
            Shuffle(this->instance->getNWords()));

        // Generate initial population
        for (
            std::vector<Shuffle>::iterator it = population.begin();
            it < population.end();
            it++
        ) {
            std::iota(it->indices.begin(), it->indices.end(), 0);
            std::shuffle(it->indices.begin(), it->indices.end(), rng);
        }

        for (int i = 0; i < this->config->nGenerations; i++) {
            // Evaluate the population
            for (Shuffle &s : population) {
                this->instance->evaluate(s);
            }

            // Pick strongest elements
            std::partial_sort(population.begin(), population.begin() + nWinners, population.end());

            // Rebuild the population
            std::uniform_int_distribution<int> rand(0, nWinners-1);
            for (auto it = work.begin(); it < work.end(); it+=2) {
                Shuffle &s1 = *(population.begin() + rand(this->rng));
                Shuffle &s2 = *(population.begin() + rand(this->rng));
                combine(s1, s2, *it, *(it+1));
            }
            population.swap(work);
        }

        for (Shuffle &s : population) {
            this->instance->evaluate(s);
            for (auto it = s.indices.begin(); it < s.indices.end(); it++) {
                if (it== s.cut) { std::cout << "| "; }
                std::cout << *it << " ";
            }
            std::cout << ": " << s.value << std::endl << std::endl;
        }
    }

    void combine(Shuffle const &in1, Shuffle const &in2, Shuffle &out1, Shuffle &out2) {
        int len = in1.indices.end() - in1.indices.begin();
        int cutRange = std::floor(len * this->config->mixing.cutRange);

        std::uniform_int_distribution<int> rand((len - cutRange) / 2, (len + cutRange) / 2);
        int cutInd = rand(this->rng);

        std::vector<bool> aux1(this->instance->getNWords());
        std::vector<bool> aux2(this->instance->getNWords());

        for (int i = 0; i < cutInd; i++) {
            out1.indices[i] = in1.indices[i];
            aux1[in1.indices[i]] = true;
        }

        for (int i = 0; i < len - cutInd; i++) {
            out2.indices[i] = in1.indices[i+cutInd];
            aux2[in1.indices[i+cutInd]] = true;
        }

        int ind1 = cutInd;
        int ind2 = len - cutInd;

        for (int i : in2.indices) {
            if (!aux1[i]) {
                out1.indices[ind1++] = i;
                aux1[i] = true;
            }
            if (!aux2[i]) {
                out2.indices[ind2++] = i;
                aux2[i] = true;
            }
        }
    }
};

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

    Instance i(std::move(in_inst), 160, 209);

    Genetic g(c, i);
    g.run();

    return 0;
}
