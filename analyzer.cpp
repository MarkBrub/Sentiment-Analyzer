#include "analyzer.hpp"

void analyzer::inputTweets(std::ifstream& fin) {
    char* line = new char[2048];

    //remove first line
    fin.getline(line, 2048);

    while(fin.getline(line, 2048)) {
        strand str(line);
        str.removeFirstSegment();

        Tweet* t = new Tweet(str.popFirstSegment().strtol(), str.popFirstSegment());

        strand word(16);

        word = str.popFirstSegment(' ');
        while(word != "") {
            if(word != "\r") {
                t->m_words.push_back(word);
                //output();
                //std::cout << std::endl;
                freqency[word]++;
            }
            word = str.popFirstSegment(' ');
        }

        tweets.push_back(t);
    }
}
void analyzer::output() {
    unordered_map<strand, int>:: iterator x;
    for (x = freqency.begin(); x != freqency.end(); x++) {
        cout << "(" << x->first << ", " << x->second << ")";
    }
}
