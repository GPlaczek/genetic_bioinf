#include <iostream>
#include <getopt.h>
#include <memory>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <random>

#include <omp.h>

#include "config.hpp"
#include "shuffle.hpp"
#include "instance.hpp"

const static struct option long_opts[] = {
    {"config", required_argument, NULL, 'c'},
    {"parallel", optional_argument, NULL, 'p'},
    {0, 0, 0, 0},
};

bool compare(std::unique_ptr<Shuffle> &s1, std::unique_ptr<Shuffle> &s2) {
    return s1->value < s2->value;
}

class Genetic {
    Config config;
    Instance instance;
    int nWinners;
    std::mt19937 rng;
public:
    Genetic(Config config, Instance instance) {
        this->config = config;
        this->instance = instance;
        this->nWinners = (int)(this->config.tournament.percentWinners * this->config.population.size);
        std::random_device rd;
        this->rng = std::mt19937(rd());
    };

    void run(bool parallel) {
        std::vector<std::unique_ptr<Shuffle>> population(this->config.population.size);

        std::vector<std::unique_ptr<Shuffle>> bestHeap(this->nWinners);

        // Generate initial population
        for (auto &it : population) {
            it.reset(new Shuffle(this->instance.getNWords()));
            std::iota(it->indices.begin(), it->indices.end(), 0);
            std::shuffle(it->indices.begin(), it->indices.end(), rng);
        }

        for (auto &it : bestHeap) {
            it.reset(new Shuffle(this->instance.getNWords()));
        }

        for (int i = 0; i < this->config.nGenerations; i++) {
            // Evaluate the population
            #pragma omp parallel for if (parallel)
            for (auto &s : population) {
                this->instance.evaluate(*s);
            }

            // Pick strongest elements
            std::partial_sort(population.begin(), population.begin() + this->nWinners, population.end(), compare);

            // Merge strongest elements with new strongest elements
            for (auto it = population.begin() + this->nWinners - 1; it >= population.begin(); it--) {
                if ((*it)->value < bestHeap.front()->value) {
                    std::pop_heap(bestHeap.begin(), bestHeap.end(), compare);
                    std::unique_ptr<Shuffle> &back = bestHeap.back();
                    back.reset(new Shuffle(**it));
                    std::push_heap(bestHeap.begin(), bestHeap.end(), compare);
                }
            }

            // Rebuild the population
            std::uniform_int_distribution<int> rand(0, nWinners-1);
            #pragma omp parallel for if (parallel)
            for (auto it = population.begin(); it < population.end(); it+=2) {
                std::unique_ptr<Shuffle> &s1 = *(bestHeap.begin() + rand(this->rng));
                std::unique_ptr<Shuffle> &s2 = *(bestHeap.begin() + rand(this->rng));
                combine(s1, s2, *it, *(it+1));
            }
        }
        for (auto &s : bestHeap) {
            // this->instance->evaluate(*s);
            for (auto it = s->indices.begin(); it < s->cut; it++) {
                std::cout << *it << " ";
            }
            std::cout << ": " << s->value << std::endl << std::endl;
        }
    }

    void combine(
        std::unique_ptr<Shuffle> &in1,
        std::unique_ptr<Shuffle> &in2,
        std::unique_ptr<Shuffle> &out1,
        std::unique_ptr<Shuffle> &out2
    ) {
        int len = in1->cut - in1->indices.begin();
        int cutRange = std::floor(len * this->config.mixing.cutRange);

        std::uniform_int_distribution<int> rand((len - cutRange) / 2, (len + cutRange) / 2);
        int cutInd = rand(this->rng);

        std::vector<bool> aux1(this->instance.getNWords());
        std::vector<bool> aux2(this->instance.getNWords());

        for (int i = 0; i < cutInd; i++) {
            out1->indices[i] = in1->indices[i];
            aux1[in1->indices[i]] = true;
        }

        for (int i = 0; i < len - cutInd; i++) {
            out2->indices[i] = in1->indices[i+cutInd];
            aux2[in1->indices[i+cutInd]] = true;
        }

        int ind1 = cutInd;
        int ind2 = len - cutInd;

        for (int i : in2->indices) {
            if (!aux1[i]) {
                // std::cout << out2->indices.size() << " ind1: " << ind2 << std::endl;
                out1->indices.at(ind1++) = i;
                aux1[i] = true;
            }
            if (!aux2[i]) {
                // std::cout << out2->indices.size() << " ind2: " << ind2 << std::endl;
                out2->indices.at(ind2++) = i;
                aux2[i] = true;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    bool parallel = false;

    std::string in_cfg;
    std::string in_inst;

    int next_option;
    while (optind < argc) {
        if ((next_option = getopt_long(argc, argv, "p:c:", long_opts, NULL)) != -1) {
            switch (next_option) {
            case 'c':
                in_cfg = optarg;
                break;
            case 'p': {
                int threads = std::atoi(optarg);
                if (threads < 1) {
                    std::cerr << "Invalid number of threads specified" << std::endl;
                    return 1;
                }
                omp_set_num_threads(threads);
                parallel = true;
                break; }
            default:
                return 1;
            }
        } else {
            if (in_inst == "") {
                in_inst = argv[optind];
            } else {
                std::cerr << "Too many instance files specified" << std::endl;
                return 1;
            }
            optind++;
        }
    }

    Config c;
    if (in_cfg != "") {
        std::ifstream __conf = std::ifstream(in_cfg);
        c = Config(__conf);
    }

    Instance i;
    if (in_inst != "") {
        std::ifstream __inst = std::ifstream(in_inst);
        i = Instance(__inst, 160, 209);
    } else {
        i = Instance(std::cin, 160, 209);
    }

    Genetic g(std::move(c), std::move(i));
    g.run(parallel);

    return 0;
}
