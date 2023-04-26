#include <memory>
#include <iostream>
#include <vector>

#include "instance.hpp"

int compareDist(std::string &first, std::string &second) {
    // this check might not be necessary
    if (first.length() != second.length()) { return -1; }

    for (int i = 0; i < first.length(); i++) {
        if (std::equal(first.begin() + i, first.end(), second.begin(), second.end() - i))
            return i;
    }

    return first.length();
}

Instance::Instance(
    std::unique_ptr<std::istream> stream,
    int popSize,
    int targetLen
) {
    this -> popSize = popSize;
    this -> targetLen = targetLen;

    this -> matrix = std::vector<std::vector<int>>(
        popSize,
        std::vector<int>(popSize));
    this -> words = std::vector<std::string>(popSize);

    for (int i = 0; i < popSize; i++) {
        *stream >> this->words[i];
    }

    for (size_t i = 0; i < popSize; i++) {
        for (size_t j = 0; j < popSize; j++)
            this->matrix[i][j] = compareDist(this->words[i], this->words[j]);
    }
}
