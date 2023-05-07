#include <iostream>
#include <getopt.h>
#include <memory>
#include <fstream>
#include <numeric>

#include <omp.h>

#include "config.hpp"
#include "instance.hpp"
#include "genetic.hpp"

const static struct option long_opts[] = {
    {"config", required_argument, NULL, 'c'},
    {"parallel", optional_argument, NULL, 'p'},
    {0, 0, 0, 0},
};


int main(int argc, char *argv[]) {
    bool parallel = false;

    std::string in_cfg;
    std::string in_inst;

    int next_option;
    while (optind < argc) {
        if ((next_option = getopt_long(argc, argv, "p::c:", long_opts, NULL)) != -1) {
            switch (next_option) {
            case 'c':
                in_cfg = optarg;
                break;
            case 'p': {
                if (optarg != nullptr) {
                    int threads = std::atoi(optarg);
                    if (threads < 1) {
                        std::cerr << "Invalid number of threads specified" << std::endl;
                        return 1;
                    }
                    omp_set_num_threads(threads);
                }
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
