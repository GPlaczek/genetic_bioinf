#include <vector>
#include <bits/stdc++.h>

#include "shuffle.hpp"

Shuffle::Shuffle(const Shuffle &s) {
    this->indices = std::vector<int>(s.indices.size());
    std::copy(s.indices.begin(), s.indices.end(), this->indices.begin());
    this->cut = this->indices.end() - (s.indices.end() - s.cut);
    this->value = s.value;
}

Shuffle::Shuffle(int nWords) {
    this->indices = std::vector<int>(nWords);
    this->cut = this->indices.end();
    this->value = INT_MAX;
}

bool Shuffle::operator< (Shuffle const& other) const {
    return this->value < other.value;
}

bool Shuffle::operator<= (Shuffle const& other) const {
    return this->value <= other.value;
}

bool Shuffle::operator> (Shuffle const& other) const {
    return this->value > other.value;
}

bool Shuffle::operator>= (Shuffle const& other) const {
    return this->value >= other.value;
}
