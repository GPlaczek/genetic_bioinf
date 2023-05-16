#include <algorithm>
#include <iostream>
#include <vector>

#include "genetic.hpp"
#include "shuffle.hpp"

Genetic::Genetic() {}

Genetic::Genetic(Config config, Instance instance) {
    this->config = config;
    this->instance = instance;
    std::random_device rd;
    this->rng = std::mt19937(rd());
    this->stop = false;
}

std::vector<Shuffle> Genetic::run(bool parallel) {
    int nWinners = (int)(this->config.tournament.percentWinners * this->config.population.size);

    std::vector<Shuffle> population(
        this->config.population.size,
        Shuffle(this->instance.getNWords()));
    std::vector<Shuffle> work(
        this->config.population.size,
        Shuffle(this->instance.getNWords()));

    // Generate initial population
    for (auto &it : population) {
        std::iota(it.indices.begin(), it.indices.end(), 0);
        std::shuffle(it.indices.begin(), it.indices.end(), rng);
    }

    for (int i = 0; !this->stop && i < this->config.nGenerations; i++) {
        std::cerr << "Generation: " << i << std::endl;
        // Evaluate the population
        #pragma omp parallel for if (parallel)
        for (auto &s : population) {
            this->instance.evaluate(s);
        }

        // Pick strongest elements
        std::partial_sort(population.begin(), population.begin() + nWinners, population.end());

        // Rebuild the population
        std::uniform_int_distribution<int> rand(0, nWinners-1);
        #pragma omp parallel for if (parallel)
        for (auto it = work.begin(); it < work.end(); it+=2) {
            int x, y;
            #pragma omp critical
            {
                x = rand(this->rng);
                y = rand(this->rng);
            }
            Shuffle &s1 = *(population.begin() + x);
            Shuffle &s2 = *(population.begin() + y);
            combine(s1, s2, *it, *(it+1));
        }
        work.swap(population);
    }

    #pragma omp parallel for if (parallel)
    for (auto &s : population) {
        this->instance.evaluate(s);
    }

    return population;
}

void Genetic::combine(
    const Shuffle &in1,
    const Shuffle &in2,
    Shuffle &out1,
    Shuffle &out2
) {
    int len = in1.cut - in1.indices.begin();
    int cutRange = std::floor(len * this->config.mixing.cutRange);

    std::uniform_int_distribution<int> rand((len - cutRange) / 2, (len + cutRange) / 2);

    int cutInd1, cutInd2;
    #pragma omp critical
    {
        cutInd1 = rand(this->rng);
        cutInd2 = rand(this->rng);
    }

    std::vector<bool> aux1(this->instance.getNWords(), false);
    std::vector<bool> aux2(this->instance.getNWords(), false);

    int ind1, ind2;

    for (ind1 = 0; ind1 < cutInd1; ind1++) {
        out1.indices[ind1] = in1.indices[ind1];
        aux1[in1.indices[ind1]] = true;
    }
    for (int i = cutInd2; i < len && ind1 < this->instance.getNWords(); i++) {
        if (!aux1[in2.indices[i]]) {
            out1.indices[ind1++] = in2.indices[i];
            aux1[in2.indices[i]] = true;
        }
    }
    for (int i = cutInd1; ind1 < this->instance.getNWords(); i++) {
        if (!aux1[in1.indices[i]]) {
            out1.indices[ind1++] = in1.indices[i];
            aux1[in1.indices[i]] = true;
        }
    }

    for (ind2 = 0; ind2 < cutInd2; ind2++) {
        out2.indices[ind2] = in2.indices[ind2];
        aux2[in2.indices[ind2]] = true;
    }
    for (int i = cutInd1; i < len && ind2 < this->instance.getNWords(); i++) {
        if (!aux2[in1.indices[i]]) {
            out2.indices[ind2++] = in1.indices[i];
            aux2[in1.indices[i]] = true;
        }
    }
    for (int i = cutInd2; ind2 < this->instance.getNWords(); i++) {
        if (!aux2[in2.indices[i]]) {
            out2.indices[ind2++] = in2.indices[i];
            aux2[in2.indices[i]] = true;
        }
    }
}

void Genetic::setStop() { this->stop = true; }
