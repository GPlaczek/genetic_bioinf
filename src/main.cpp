#include <iostream>
#include "config.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Hello world" << std::endl;
    Config c = Config("res/config.ini");

    std::cout << c.nGenerations << std::endl;
    std::cout << c.population.size << std::endl;
    std::cout << c.tournament.nWinners << std::endl;
    std::cout << c.mixing.precise << std::endl;
    std::cout << c.mixing.cutRange << std::endl;
    std::cout << c.mutation.chance << std::endl;
    std::cout << c.mutation.nShuffles << std::endl;
    return 0;
}
