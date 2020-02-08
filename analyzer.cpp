#include "analyzer.hpp"

const std::vector<strand> analyzer::stopWords = {"ourselves", "hers", "between", "yourself",
                                              "but", "again", "there", "about", "once", "during",
                                              "out", "very", "having", "with", "they", "own", "an",
                                              "be", "some", "for", "do", "its", "yours", "such",
                                              "into", "of", "most", "itself", "other", "off", "is",
                                              "s", "am", "or", "who", "as", "from", "him", "each",
                                              "the", "themselves", "until", "below", "are", "we",
                                              "these", "your", "his", "through", "don", "nor", "me",
                                              "were", "her", "more", "himself", "this", "down",
                                              "should", "our", "their", "while", "above", "both",
                                              "up", "to", "ours", "had", "she", "all", "no", "when",
                                              "at", "any", "before", "them", "same", "and", "been",
                                              "have", "in", "will", "on", "does", "yourselves",
                                              "then", "that", "because", "what", "over", "why",
                                              "so", "can", "did", "not", "now", "under", "he", "you",
                                              "herself", "has", "just", "where", "too", "only",
                                              "myself", "which", "those", "i", "after", "few",
                                              "whom", "t", "being", "if", "theirs", "my", "against",
                                              "a", "by", "doing", "it", "how", "further", "was",
                                              "here", "than"};

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

        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePuncuation();
                //t->m_words.push_back(word);
                //if(word.isURL()) continue;
                if(word.size() > 2) frequency[word] += sentiment;
            }
        }
    }

    delete[] dataLine;
    delete[] targetLine;
}

void analyzer::classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    char* dataLine = new char[2048];
    char* targetLine = new char[32];
    double numTweets = 0;
    double numCorrect = 0;

    //remove first line
    dataFile.getline(dataLine, 2048);
    targetFile.getline(targetLine, 32);

    while(dataFile.getline(dataLine, 2048)) {
        targetFile.getline(targetLine, 32);
        numTweets++;

        strand data(dataLine);
        strand target(targetLine);
        strand word(64);

        //remove the rowNum column
        target.removeFirstSegment();
        data.removeFirstSegment();

        int actualSentiment = (target[0] == '0') ? -1 : 1;
        Tweet* t = new Tweet(data.popFirstSegment().strtol(), data.popFirstSegment());

        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePuncuation();
                t->m_words.push_back(word);
            }
        }

        if(calcSentiment(t) == actualSentiment) numCorrect++;

        tweets.push_back(t);
    }

    std::cout << "Total tweets: " << numTweets << std::endl;
    std::cout << "Percent correct: " << numCorrect / numTweets << std::endl;
    std::cout << "Undetermined tweets: " << count << std::endl;

    delete[] dataLine;
    delete[] targetLine;
}

void analyzer::output(int threshold) {
    std::unordered_map<strand, int>:: iterator x;
    for (x = frequency.begin(); x != frequency.end(); x++) {
        if(x->second > threshold || x->second < -threshold) std::cout << "(" << x->first << ", " << x->second << ")" << std::endl;
    }
}
bool analyzer::isStopWord(strand& str) {
    for(int x = 0; x < stopWords.size(); x++) {
        if(str == stopWords[x]) return true;
    }
    return false;
}
int analyzer::calcSentiment(Tweet* t) {
    for(int x = t->size() - 1; x >= 0; x--) {
        //if(isStopWord(((*t)[x]))) continue;
        if((*t)[x].size() < 3) continue;

        int freq = frequency[(*t)[x]];
        if(abs(freq) < 30) continue;
        if(freq > 0) freq = 1;
        if(freq < 0) freq = -1;
        t->m_sentiment += freq;
    }

    t->clampSentiment();

    if(t->m_sentiment == 0) {
        count++;
        return 1;
    }

    return t->m_sentiment;
    //uhhhhhhhg
}