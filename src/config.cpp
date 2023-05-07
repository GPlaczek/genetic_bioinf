#include <string>
#include <inipp.h>
#include <memory>

#include "config.hpp"

Config::Config() {
    this->nGenerations = DEFAULT_N_GENERATIONS;
    this->population = { DEFAULT_POP_SIZE };
    this->mutation = {
        DEFAULT_MUTATION_CHANCE,
        DEFAULT_N_SHUFFLES
    };
    this->tournament = { DEFAULT_PERCENT_WINNERS };
    this->mixing = {
        DEFAULT_PRECISE,
        DEFAULT_CUT_RANGE
    };
}

Config::Config(std::istream &source) : Config::Config() {
    inipp::Ini<char> ini;
    ini.parse(source);
    inipp::get_value(ini.sections["globals"], "nGenerations", this->nGenerations);
    inipp::get_value(ini.sections["population"], "size", this->population.size);
    inipp::get_value(ini.sections["tournament"], "percentWinners", this->tournament.percentWinners);
    inipp::get_value(ini.sections["mixing"], "precise", this->mixing.precise);
    inipp::get_value(ini.sections["mixing"], "cutRange", this->mixing.cutRange);
    inipp::get_value(ini.sections["mutation"], "chance", this->mutation.chance);
    inipp::get_value(ini.sections["mutation"], "nShuffles", this->mutation.nShuffles);
}
