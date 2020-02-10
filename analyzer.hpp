#pragma once
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

struct analyzer {
    static std::vector<strand> stopWords;
    std::unordered_map<strand, int> frequency;
    std::vector<Tweet*> tweets;
    double accuracy = 0;
    int unclassified = 0;

    ~analyzer();

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void output(std::ofstream& out);
    static bool isStopWord(strand& str);
    int calcSentiment(Tweet* t);
};