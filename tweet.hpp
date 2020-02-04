#pragma once
#include <vector>
#include "strand.hpp"

using namespace std;

struct Tweet {
    long m_id;
    vector<strand> m_words;
    strand m_user;

    Tweet();
    Tweet(long id, strand user);
};