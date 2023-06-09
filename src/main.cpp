#include <iostream>
#include <fstream>
#include <functional>
#include <csignal>
#include <algorithm>

#include <omp.h>
#include <getopt.h>

#include "config.hpp"
#include "instance.hpp"
#include "genetic.hpp"

const static struct option long_opts[] = {
    {"config", required_argument, NULL, 'c'},
    {"parallel", optional_argument, NULL, 'p'},
    {"n-words", required_argument, NULL, 'n'},
    {"target-length", required_argument, NULL, 'l'},
    {0, 0, 0, 0},
};

std::function<void(int)> handler;
void sigint_handler(int signum) {
    handler(signum);
    std::signal(SIGINT, SIG_DFL);
}

int main(int argc, char *argv[]) {
    bool parallel = false;
    int nWords = -1;
    int targetLength = -1;

    std::string in_cfg;
    std::string in_inst;

    int next_option = 0;
    while (optind < argc) {
        int index = -1;
        if ((next_option = getopt_long(argc, argv, ":p::c:n:l:", long_opts, &index)) != -1) {
            switch (next_option) {
            case 'c':
                in_cfg = optarg;
                break;
            case 'p':
                if (optarg != nullptr) {
                    int threads = std::atoi(optarg);
                    if (threads < 1) {
                        std::cerr << "Invalid number of threads specified" << std::endl;
                        return 1;
                    }
                    omp_set_num_threads(threads);
                }
                parallel = true;
                break;
            case 'n':
                nWords = std::atoi(optarg);
                break;
            case 'l':
                targetLength = std::atoi(optarg); break;
            case ':':
                if (index != -1) {
                    std::cerr << "Option '" << long_opts[index].name << "' requires a parameter" << std::endl;
                } else {
                    std::cerr << "Option '" << (char)optopt << "' requires a parameter" << std::endl;
                }
                return 1;
            case '?':
                std::cerr << "Unrecognized option: '" << (char)optopt << "'" << std::endl;
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

    if (targetLength == -1) {
        std::cerr << "Target length not specified" << std::endl;
        return 1;
    }

    if (nWords == -1) {
        std::cerr << "Number of words not specified" << std::endl;
        return 1;
    }

    Config c;
    if (in_cfg != "") {
        std::ifstream __conf = std::ifstream(in_cfg);
        c = Config();
        try {
            c.load(__conf);
        } catch(const std::exception &e) {
            std::cerr << "Could not load config: ";
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    Instance i;
    if (in_inst != "") {
        std::ifstream __inst = std::ifstream(in_inst);
        i = Instance(__inst, nWords, targetLength);
    } else {
        i = Instance(std::cin, nWords, targetLength);
    }

    Genetic g(std::move(c), i);
    handler = [&g](int _signum) {
        std::cerr << "Stopping early" << std::endl;
        g.setStop();
    };
    std::signal(SIGINT, sigint_handler);

    std::vector<Shuffle> result = g.run(parallel);
    auto max = std::min_element(result.begin(), result.end());
    i.prettyPrint(std::cout, *max);

    return 0;
}
