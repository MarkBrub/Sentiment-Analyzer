#pragma once
#include <vector>
#include "strand.hpp"

struct Tweet {
    long long m_id;
    std::vector<strand> m_words;
    strand m_user;

    Tweet();
    Tweet(long long id, strand user);
};