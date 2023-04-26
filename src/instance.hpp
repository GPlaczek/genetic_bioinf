#pragma once

#include <vector>
#include <string>
#include <memory>

class Instance {
    int popSize;
    int targetLen;
    std::vector<std::vector<int>> matrix;
    std::vector<std::string> words;
public:
    Instance(std::unique_ptr<std::istream> stream, int popSize, int targetLen);
};
