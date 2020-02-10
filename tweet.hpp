#pragma once
#include <vector>
#include "strand.hpp"

struct Tweet {
    double m_sentiment = 1;
    long long m_id{};
    std::vector<strand> m_words;
    bool correct = false;

    Tweet();
    explicit Tweet(long long id);

    int size();
    strand& operator[](int x);

    void clampSentiment();

};