#include <vector>

#include "shuffle.hpp"

Shuffle::Shuffle(int nWords) {
    this->indices = std::vector<int>(nWords);
    this->cut = this->indices.end();
    this->value = -1;
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
