#pragma once
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

struct analyzer {
    static std::vector<strand> stopWords;
    std::unordered_map<strand, int> positive;
    std::unordered_map<strand, int> negative;
    std::vector<Tweet*> tweets;
    double accuracy = 0;
    int unclassified = 0;

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void output(std::ofstream& out);
    bool isStopWord(strand& str);
    int calcSentiment(Tweet* t);
};