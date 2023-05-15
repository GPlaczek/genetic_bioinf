#pragma once

#include <vector>

class Shuffle {
public:
    std::vector<int> indices;
    int value;
    std::vector<int>::iterator cut;

    Shuffle(int nWords);
    // Shuffle(const Shuffle &s);

    bool operator< (Shuffle const& other) const;
    bool operator<= (Shuffle const& other) const;
    bool operator> (Shuffle const& other) const;
    bool operator>= (Shuffle const& other) const;
};
