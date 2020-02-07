#pragma once
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

class analyzer {
    std::unordered_map<strand, int> freqency;
    std::vector<Tweet*> tweets;
public:
    ~analyzer();

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void output(int threshold = 50);
};