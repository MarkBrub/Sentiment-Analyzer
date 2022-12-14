#include "analyzer.hpp"

//A vector of stop words
std::vector<strand> analyzer::stopWords = {"ourselves", "hers", "between", "yourself", "but", "again", "there", "about", "once", "during", "out", "very", "having", "with", "they", "own", "an", "be", "some", "for", "do", "its", "yours", "such", "into", "of", "most", "itself", "other", "off", "is", "s", "am", "or", "who", "as", "from", "him", "each", "the", "themselves", "until", "below", "are", "we", "these", "your", "his", "through", "don", "nor", "me", "were", "her", "more", "himself", "this", "down", "should", "our", "their", "while", "above", "both", "up", "to", "ours", "had", "she", "all", "no", "when", "at", "any", "before", "them", "same", "and", "been", "have", "in", "will", "on", "does", "yourselves", "then", "that", "because", "what", "over", "why", "so", "can", "did", "not", "now", "under", "he", "you", "herself", "has", "just", "where", "too", "only", "myself", "which", "those", "i", "after", "few", "whom", "t", "being", "if", "theirs", "my", "against", "a", "by", "doing", "it", "how", "further", "was", "here", "than"};

analyzer::~analyzer() {
    for(int x = 0; x < tweets.size(); x++) {
        delete tweets[x];
    }
}

//stores frequency of words in training set
void analyzer::inputTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    strand data(1024);
    strand target(32);
    int numTweets = 0;

    dataFile >> data;
    targetFile >> target;

    while(dataFile >> data) {
        targetFile >> target;
        numTweets++;

        //remove the rowNum column
        target.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();

        //get the tweet sentiment as frequency or negative
        int sentiment = (target[0] == '0') ? -1 : 1;
        strand word(128);

        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePunctuation();
                //t->m_words.push_back(word);
                if(word.isURL()) continue;
                if(word.size() > 2) frequency[word] += sentiment;
            }
        }
    }

    std::cout << "Total tweets: " << numTweets << std::endl;
}

//uses the stored data to read and classify sentiment of tweets
void analyzer::classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
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
        Tweet* t = new Tweet(data.popFirstSegment().toLongLong());
        strand word(128);

        //remove username
        data.removeFirstSegment();

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

//returns true if the strand is a stop word
bool analyzer::isStopWord(strand& str) {
    for(int x = 0; x < stopWords.size(); x++) {
        if(str == stopWords[x]) return true;
    }
    return false;
}

//uses the values of the individual words to get a total sentiment
int analyzer::calcSentiment(Tweet* t) {
    for(int x = t->size() - 1; x >= 0; x--) {
        if(isStopWord(((*t)[x]))) continue;
        if((*t)[x].size() < 3) continue;
        if((*t)[x].isURL()) continue;

        int freq = frequency[(*t)[x]];
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

//outputs the accuracy and missed tweets to a file
void analyzer::output(std::ofstream& out) {
    out << std::fixed << std::setprecision(3) << accuracy << std::endl;

    for(int x = 0; x < tweets.size(); x++) {
        if(!tweets[x]->correct) {
            out << tweets[x]->m_id << std::endl;
        }
    }
}