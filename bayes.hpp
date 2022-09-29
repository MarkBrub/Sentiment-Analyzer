#pragma once
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "strand.hpp"
#include "tweet.hpp"

struct Bayes {
    static std::vector<strand> stopWords;
    std::unordered_map<strand, int> positive;
    std::unordered_map<strand, int> negative;
    std::unordered_map<strand, int> unique;
    int positiveWords = 0;
    int negativeWords = 0;
    std::vector<Tweet*> tweets;
    double accuracy = 0;

    ~Bayes();

    void inputTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    void classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile);
    double naiveBayes(Tweet* t, std::unordered_map<strand, int>& frequency, double total);
    void output(std::ofstream& out);
    static bool isStopWord(strand* str);
};