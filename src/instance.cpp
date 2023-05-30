#include <memory>
#include <iostream>
#include <vector>

#include "instance.hpp"
#include "shuffle.hpp"

#define ANSI_RED(str) "\033[1;31m" + str + "\033[0m"

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

    stream >> this->words[0];
    if (this->words[0].length() == 0) {
            std::cerr << "Invalid instance file" << std::endl;
            std::exit(1);
    }
    for (int i = 1; i < nWords; i++) {
        stream >> this->words[i];
        if (this->words[i].length() != this->words[0].length()) {
            std::cerr << "Invalid instance file" << std::endl;
            std::exit(1);
        }
    }

    for (size_t i = 0; i < nWords; i++) {
        for (size_t j = 0; j < nWords; j++)
            this->matrix[i][j] = compareDist(this->words[i], this->words[j]);
    }
}

void Instance::evaluate(Shuffle &s) const {
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

    int diff = seqLen < this->targetLen ? this->targetLen - seqLen : seqLen - this->targetLen;
    s.value = nPositive + (seqLen - this->targetLen) * (seqLen - this->targetLen);
    s.cut = s.indices.begin() + i;
}

int Instance::getNWords() const { return this -> nWords; }
int Instance::getTargetLen() const { return this -> targetLen; }

void Instance::prettyPrint(std::ostream &out, const Shuffle &s1) const {
    out << this->words[s1.indices.front()];
    for (auto it = s1.indices.begin() + 1; it < s1.cut; it++) {
        int dist = this->matrix[*(it-1)][*it];
        out << ANSI_RED(this->words[*it].substr(0, dist-1));
        out << this->words[*it].back();
    }
    out << ": " << s1.value << std::endl;
}
