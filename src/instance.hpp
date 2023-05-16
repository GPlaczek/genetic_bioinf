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
    int getNWords() const;
    int getTargetLen() const;
    void evaluate(Shuffle &s) const;

    Instance();
    Instance(std::istream &stream, int nWords, int targetLen);
};
