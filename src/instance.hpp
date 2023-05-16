#pragma once

#include <vector>
#include <string>
#include <memory>
#include <ostream>

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
    void prettyPrint(std::ostream &out, const Shuffle &s1) const;

    Instance();
    Instance(std::istream &stream, int nWords, int targetLen);
};
