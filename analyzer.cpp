#include "analyzer.hpp"

analyzer::~analyzer() {
    for(int x = 0; x < tweets.size(); x++) {
        delete tweets[x];
    }
}

void analyzer::inputTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    char* dataLine = new char[2048];
    char* targetLine = new char[32];

    //remove first line
    dataFile.getline(dataLine, 2048);
    targetFile.getline(targetLine, 32);

    while(dataFile.getline(dataLine, 2048)) {
        targetFile.getline(targetLine, 32);
        strand data(dataLine);
        strand target(targetLine);
        strand word(64);

        //remove the rowNum column
        target.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();

        //get the tweet sentiment as positive or negative
        int sentiment = (target[0] == '0') ? -1 : 1;

        Tweet* t = new Tweet(data.popFirstSegment().strtol(), data.popFirstSegment());

        while(data.size() > 0) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 47 && word[0] < 123 && word[0] != 64) {
                word.toLower();
                t->m_words.push_back(word);

                if(word.size() > 2) freqency[word] += sentiment;
            }
        }

        tweets.push_back(t);
    }

    delete[] dataLine;
    delete[] targetLine;
}
void analyzer::output() {
    std::unordered_map<strand, int>:: iterator x;
    for (x = freqency.begin(); x != freqency.end(); x++) {
        if(x->second > 100 || x->second < -100) std::cout << "(" << x->first << ", " << x->second << ")" << std::endl;
    }
}