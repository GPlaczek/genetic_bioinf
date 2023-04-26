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
    int nWords,
    int targetLen
) {
    this -> nWords = nWords;
    this -> targetLen = targetLen;

    this -> matrix = std::vector<std::vector<int>>(
        nWords,
        std::vector<int>(nWords));
    this -> words = std::vector<std::string>(nWords);

    for (int i = 0; i < nWords; i++) {
        *stream >> this->words[i];
    }

    for (size_t i = 0; i < nWords; i++) {
        for (size_t j = 0; j < nWords; j++)
            this->matrix[i][j] = compareDist(this->words[i], this->words[j]);
    }
}

int Instance::getNWords() { return this -> nWords; }
int Instance::getTargetLen() { return this -> targetLen; }
