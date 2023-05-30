#include <string>

#include <inipp.h>

#include "config.hpp"

Config::Config() {
    this->nGenerations = DEFAULT_N_GENERATIONS;
    this->population = { DEFAULT_POP_SIZE };
    this->mutation = {
        DEFAULT_MUTATION_CHANCE,
        DEFAULT_N_SHUFFLES,
        DEFAULT_DIRTY
    };
    this->tournament = { DEFAULT_PERCENT_WINNERS };
    this->mixing = {
        DEFAULT_PRECISE,
        DEFAULT_CUT_RANGE
    };
}

void Config::load(std::istream &source) {
    inipp::Ini<char> ini;
    ini.parse(source);

    inipp::get_value(ini.sections["globals"], "nGenerations", this->nGenerations);
    if (this->nGenerations < 1)
        throw std::out_of_range("Invalid number of generations: " + std::to_string(this->nGenerations));

    inipp::get_value(ini.sections["population"], "size", this->population.size);
    if (this->population.size < 1)
        throw std::out_of_range("Invalid population size: " + std::to_string(this->population.size));
    if (this->population.size & 1)
        throw std::out_of_range("Invalid population size (should be even): " + std::to_string(this->population.size));

    inipp::get_value(ini.sections["tournament"], "percentWinners", this->tournament.percentWinners);
    if (this->tournament.percentWinners < 0 || this->tournament.percentWinners > 1)
        throw std::out_of_range("Invalid percent winners: " + std::to_string(this->tournament.percentWinners));
    if (this->tournament.percentWinners * static_cast<float>(this->population.size) < 1)
        throw std::out_of_range("Invalid percent winners : " + std::to_string(this->tournament.percentWinners) + " (no winners with given population size)");

    inipp::get_value(ini.sections["mixing"], "precise", this->mixing.precise);
    inipp::get_value(ini.sections["mixing"], "cutRange", this->mixing.cutRange);
    if (this->tournament.percentWinners < 0 || this->mixing.cutRange > 1)
        throw std::out_of_range("Invalid cut range: " + std::to_string(this->mixing.cutRange));

    inipp::get_value(ini.sections["mutation"], "chance", this->mutation.chance);
    if (this->tournament.percentWinners < 0 || this->mutation.chance> 1)
        throw std::out_of_range("Invalid mutation chance: " + std::to_string(this->mutation.chance));

    inipp::get_value(ini.sections["mutation"], "nShuffles", this->mutation.nShuffles);
    if (this->mutation.nShuffles < 1)
        throw std::out_of_range("Invalid number of shuffles: " + std::to_string(this->mutation.chance));
    inipp::get_value(ini.sections["mutation"], "dirty", this->mutation.dirty);
}
