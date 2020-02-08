#pragma once
#include <vector>
#include "strand.hpp"

struct Tweet {
    int m_sentiment = 0;
    long long m_id;
    std::vector<strand> m_words;
    strand m_user;

    Tweet();
    Tweet(long long id, strand user);

    int size();
    strand& operator[](int x);

    void clampSentiment();

};