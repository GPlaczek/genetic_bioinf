#pragma once

#include <vector>
#include <string>
#include <memory>

#include "shuffle.hpp"

class Instance {
    int nWords;
    int targetLen;
    std::vector<std::vector<int>> matrix;
    std::vector<std::string> words;
public:
    int getNWords();
    int getTargetLen();
    void evaluate(Shuffle &s);

    Instance();
    Instance(std::unique_ptr<std::istream> stream, int nWords, int targetLen);
};
