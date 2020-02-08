#pragma once
#include <vector>
#include "strand.hpp"

struct Tweet {
    int m_sentiment = 0;
    long long m_id;
    strand m_user;
    std::vector<strand> m_words;
    bool correct = false;

    Tweet();
    Tweet(long long id, strand user);

    int size();
    strand& operator[](int x);

    void clampSentiment();

};