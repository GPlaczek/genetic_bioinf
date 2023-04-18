#include <string>
#include <inipp.h>
#include <fstream>

#include "config.hpp"

Config::Config(std::string path) {
    inipp::Ini<char> ini;
    std::ifstream is(path);
    ini.parse(is);

    int nGenerations;
    inipp::get_value(ini.sections["globals"], "nGenerations", nGenerations);
    this -> nGenerations = nGenerations;

    int popSize;
    inipp::get_value(ini.sections["population"], "size", popSize);
    this -> population = { popSize };

    int nWinners;
    inipp::get_value(ini.sections["tournament"], "nWinners", nWinners);
    this -> tournament = { nWinners };

    bool precise;
    int cutRange;
    inipp::get_value(ini.sections["mixing"], "precise", precise);
    inipp::get_value(ini.sections["mixing"], "cutRange", cutRange);
    this -> mixing = {
        precise,
        cutRange
    };

    float chance;
    int nShuffles;
    inipp::get_value(ini.sections["mutation"], "chance", chance);
    inipp::get_value(ini.sections["mutation"], "nShuffles", nShuffles);
    this -> mutation = {
        chance,
        nShuffles
    };
}
