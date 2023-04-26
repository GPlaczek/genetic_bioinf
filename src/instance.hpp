#pragma once

#include <vector>
#include <string>
#include <memory>

class Instance {
    int nWords;
    int targetLen;
    std::vector<std::vector<int>> matrix;
    std::vector<std::string> words;
public:
    int getNWords();
    int getTargetLen();
    Instance(std::unique_ptr<std::istream> stream, int nWords, int targetLen);
};
