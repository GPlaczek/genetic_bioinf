#include <string>
#include <inipp.h>
#include <memory>

#include "config.hpp"

Config::Config() {}

Config::Config(std::unique_ptr<std::istream> source) {
    int nGenerations = DEFAULT_N_GENERATIONS;
    int popSize = DEFAULT_POP_SIZE;
    int nShuffles = DEFAULT_N_SHUFFLES;

    float percentWinners = DEFAULT_PERCENT_WINNERS;
    float cutRange = DEFAULT_CUT_RANGE;
    float chance = DEFAULT_MUTATION_CHANCE;

    bool precise = DEFAULT_PRECISE;

    if (source != nullptr) {
        inipp::Ini<char> ini;
        ini.parse(*source);
        inipp::get_value(ini.sections["globals"], "nGenerations", nGenerations);
        inipp::get_value(ini.sections["population"], "size", popSize);
        inipp::get_value(ini.sections["tournament"], "percentWinners", percentWinners);
        inipp::get_value(ini.sections["mixing"], "precise", precise);
        inipp::get_value(ini.sections["mixing"], "cutRange", cutRange);
        inipp::get_value(ini.sections["mutation"], "chance", chance);
        inipp::get_value(ini.sections["mutation"], "nShuffles", nShuffles);
    }

    this -> nGenerations = nGenerations;
    this -> population = { popSize };
    this -> tournament = { percentWinners };
    this -> mixing = {
        precise,
        cutRange
    };
    this -> mutation = {
        chance,
        nShuffles
    };
}
