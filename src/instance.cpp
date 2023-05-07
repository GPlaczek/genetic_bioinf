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

Instance::Instance() {}

Instance::Instance(
    std::istream &stream,
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
        stream >> this->words[i];
    }

    for (size_t i = 0; i < nWords; i++) {
        for (size_t j = 0; j < nWords; j++)
            this->matrix[i][j] = compareDist(this->words[i], this->words[j]);
    }
}

void Instance::evaluate(Shuffle &s) {
    int nPositive = s.indices.size();
    int nNegative = 0;
    int seqLen = this->words[0].length();

    // find a place where expected length is greater or equal to targetLen
    int i;
    for (i = 0; i < s.indices.size() - 1 && seqLen < this->targetLen; i++) {
        int distance = this->matrix[s.indices[i]][s.indices[i+1]];

        nPositive -= 1;
        nNegative += distance - 1;
        seqLen += distance;
    }

    s.value = (seqLen - targetLen) * (seqLen - targetLen) + nPositive;
    s.cut = s.indices.begin() + i;
}

int Instance::getNWords() { return this -> nWords; }
int Instance::getTargetLen() { return this -> targetLen; }
