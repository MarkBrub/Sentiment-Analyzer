#include "bayes.hpp"

void bayes::inputTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    strand data(1024);
    strand target(32);

    dataFile >> data;
    targetFile >> target;

    while(dataFile >> data) {
        targetFile >> target;

        //remove the rowNum column
        target.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();

        //get the tweet sentiment as positive or negative
        int sentiment = (target[0] == '0') ? -1 : 1;
        strand word(64);

        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePunctuation();
                //t->m_words.push_back(word);
                if(word.isURL()) continue;
                if(word.size() > 2) positive[word] += sentiment;
            }
        }
    }
}

void bayes::classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    strand data(1024);
    strand target(32);
    double numTweets = 0;
    double numCorrect = 0;

    //remove first line
    dataFile >> data;
    targetFile >> target;

    while(dataFile >> data) {
        targetFile >> target;
        numTweets++;

        //remove the rowNum column
        target.removeFirstSegment();
        data.removeFirstSegment();

        int actualSentiment = (target[0] == '0') ? -1 : 1;
        Tweet* t = new Tweet(data.popFirstSegment().toLongLong(), data.popFirstSegment());
        strand word(64);

        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePunctuation();
                t->m_words.push_back(word);
            }
        }

        if(calcSentiment(t) == actualSentiment) {
            numCorrect++;
            t->correct = true;
        }

        tweets.push_back(t);
    }

    accuracy = numCorrect / numTweets;

    std::cout << "Total tweets: " << numTweets << std::endl;
    std::cout << "Percent correct: " << accuracy << std::endl;
    std::cout << "Undetermined tweets: " << unclassified << std::endl;
}

int bayes::calcSentiment(Tweet* t) {
    for(int x = t->size() - 1; x >= 0; x--) {
        //if(isStopWord(((*t)[x]))) continue;
        if((*t)[x].size() < 3) continue;
        if((*t)[x].isURL()) continue;

        int freq = positive[(*t)[x]];
        if(abs(freq) < 10) continue;
        if(freq > 0) freq = 1;
        if(freq < 0) freq = -1;
        t->m_sentiment += freq;
    }

    t->clampSentiment();

    if(t->m_sentiment == 0) {
        unclassified++;
        return 1;
    }

    return t->m_sentiment;
}

/*
void bayes::output(std::ofstream& out) {
    out << std::fixed << std::setprecision(3) << accuracy << std::endl;

    for(int x = 0; x < tweets.size(); x++) {
        if(!tweets[x]->correct) {
            out << tweets[x]->m_id << std::endl;
        }
    }
}*/