#pragma once
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

struct analyzer {
    std::unordered_map<strand, int> positive;
    std::unordered_map<strand, int> negative;
    std::vector<Tweet*> tweets;
    double accuracy;
    int unclassified = 0;

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void output(std::ofstream& out);
    int calcSentiment(Tweet* t);
};