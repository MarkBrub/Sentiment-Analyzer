#pragma once
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

struct analyzer {
    std::unordered_map<strand, int> frequency;
    std::vector<Tweet*> tweets;
    static const std::vector<strand> stopWords;
    int count = 0;

    ~analyzer();

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void output(int threshold = 50);
    static bool isStopWord(strand& str);
    int calcSentiment(Tweet* t);
};