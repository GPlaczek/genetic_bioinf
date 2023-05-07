#include <algorithm>
#include <iostream>
#include <vector>

#include "genetic.hpp"
#include "shuffle.hpp"

bool compare(std::unique_ptr<Shuffle> &s1, std::unique_ptr<Shuffle> &s2) {
    return s1->value < s2->value;
}

Genetic::Genetic() {}

Genetic::Genetic(Config config, Instance instance) {
    this->config = config;
    this->instance = instance;
    std::random_device rd;
    this->rng = std::mt19937(rd());
}

void Genetic::run(bool parallel) {
    int nWinners = (int)(this->config.tournament.percentWinners * this->config.population.size);

    std::vector<std::unique_ptr<Shuffle>> population(this->config.population.size);
    std::vector<std::unique_ptr<Shuffle>> bestHeap(nWinners);

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
        std::partial_sort(population.begin(), population.begin() + nWinners, population.end(), compare);

        // Merge strongest elements with new strongest elements
        for (auto it = population.begin() + nWinners - 1; it >= population.begin(); it--) {
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
            int y, x = rand(this->rng);
            while ((y = rand(this->rng) == x));
            std::unique_ptr<Shuffle> &s1 = *(bestHeap.begin() + x);
            std::unique_ptr<Shuffle> &s2 = *(bestHeap.begin() + y);
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

void Genetic::combine(
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
