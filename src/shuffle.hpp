#pragma once

#include <vector>

class Shuffle {
public:
    std::vector<int> indices;
    int value;
    Shuffle(int nWords);
    bool operator< (Shuffle const& other) const;
    bool operator<= (Shuffle const& other) const;
    bool operator> (Shuffle const& other) const;
    bool operator>= (Shuffle const& other) const;
};
