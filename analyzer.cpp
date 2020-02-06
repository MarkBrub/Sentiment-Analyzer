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

        data.removeFirstSegment();
        target.removeFirstSegment();

        Tweet* t = new Tweet(data.popFirstSegment().strtol(), data.popFirstSegment());
        //Tweet* t = new Tweet();

        strand word(128);

        word = data.popFirstSegment(' ');
        while(word[0] != '~') {
            //make sure the first char is a letter
            if(word[0] > 47 && word[0] < 123 && word[0] != 64) {
                word.toLower();
                t->m_words.push_back(word);
                if(word.size() > 2) {
                    if(target[0] == '0') freqency[word]--;
                    if(target[0] == '4') freqency[word]++;
                }
            }
            word = data.popFirstSegment(' ');
        }

        tweets.push_back(t);
    }
}
void analyzer::output() {
    unordered_map<strand, int>:: iterator x;
    for (x = freqency.begin(); x != freqency.end(); x++) {
        if(x->second > 30 || x->second < -30) cout << "(" << x->first << ", " << x->second << ")" << std::endl;
    }
}