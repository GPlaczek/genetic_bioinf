#include <vector>
#include <bits/stdc++.h>

#include "shuffle.hpp"

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
