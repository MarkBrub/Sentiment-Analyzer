#include "bayes.hpp"

//A vector of stop words
std::vector<strand> Bayes::stopWords = {"ourselves", "hers", "between", "yourself", "but", "again", "there", "about", "once", "during", "out", "very", "having", "with", "they", "own", "an", "be", "some", "for", "do", "its", "yours", "such", "into", "of", "most", "itself", "other", "off", "is", "s", "am", "or", "who", "as", "from", "him", "each", "the", "themselves", "until", "below", "are", "we", "these", "your", "his", "through", "don", "nor", "me", "were", "her", "more", "himself", "this", "down", "should", "our", "their", "while", "above", "both", "up", "to", "ours", "had", "she", "all", "no", "when", "at", "any", "before", "them", "same", "and", "been", "have", "in", "will", "on", "does", "yourselves", "then", "that", "because", "what", "over", "why", "so", "can", "did", "not", "now", "under", "he", "you", "herself", "has", "just", "where", "too", "only", "myself", "which", "those", "i", "after", "few", "whom", "t", "being", "if", "theirs", "my", "against", "a", "by", "doing", "it", "how", "further", "was", "here", "than"};

Bayes::~Bayes() {
    for (Tweet* tweet : tweets) {
        delete tweet;
    }
}

//stores frequency of words in training set
void Bayes::inputTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
    strand data(1024);
    strand target(32);
    int numTweets = 0;

    dataFile >> data;
    targetFile >> target;

    while(dataFile >> data) {
        targetFile >> target;
        numTweets++;

        //remove the rowNum id and username column
        target.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();
        data.removeFirstSegment();

        //get the tweet sentiment as positive or negative
        int sentiment = (target[0] == '0') ? -1 : 1;
        strand word(512);

        //loop through the words in the tweet
        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePunctuation();
                if(word.isURL()) continue;

                //word.stemStrand();

                //push the words into the appropriate unordered map
                if(sentiment == 1) {
                    positive[word]++;
                    positiveWords++;
                } else if(sentiment == -1) {
                    negative[word]++;
                    negativeWords++;
                }

                //if not already in the map it add the word
                unique[word];
            }
        }
    }

    std::cout << "Total tweets: " << numTweets << std::endl;
    std::cout << "Positive words: " << positiveWords << std::endl;
    std::cout << "Negative words: " << negativeWords << std::endl;
}

//uses the stored data to read and classify sentiment of tweets
void Bayes::classifyTweets(std::ifstream& dataFile, std::ifstream& targetFile) {
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

        //remove the user column
        data.removeFirstSegment();

        //loop through the words in the tweet
        while(!data.empty()) {
            word = data.popLastSegment(' ');
            //make sure the first char is a letter
            if(word[0] > 64 && word[0] < 123) {
                word.toLower();
                word.removePunctuation();
                t->m_words.push_back(word);
            }
        }

        //gets the change for a tweet to be positive and negative
        double pos = naiveBayes(t, positive, positiveWords);
        //reset the tweet sentiment
        t->m_sentiment = 1;
        double neg = naiveBayes(t, negative, negativeWords);

        //get the higher chance of tweet sentiment
        if(pos > neg) {
            t->m_sentiment = 1;
        } else {
            t->m_sentiment = -1;
        }

        if(t->m_sentiment == actualSentiment) {
            numCorrect++;
            t->correct = true;
        }

        tweets.push_back(t);
    }

    accuracy = numCorrect / numTweets;

    std::cout << "Total tweets: " << numTweets << std::endl;
    std::cout << "Percent correct: " << accuracy << std::endl;
}

double Bayes::naiveBayes(Tweet* t, std::unordered_map<strand, int>& frequency, double total) {
    for(int x = t->size() - 1; x >= 0; x--) {
        strand* word = &(*t)[x]; //This line is cancer but if I put the tweets on the stack the program slows way down
        if(isStopWord(word)) continue;
        if(word->isURL()) continue;

        //word->stemStrand();

        //Naive Bayes algorithm
        double freq = frequency[*word] + 1;
        t->m_sentiment *= (freq / ((unique.size() + total)));
    }

    //if no solution is reached
    //should not happen
    if(t->m_sentiment == 0) return 1;

    return t->m_sentiment;
}

//returns true if the strand is a stop word
bool Bayes::isStopWord(strand* str) {
    for(int x = 0; x < stopWords.size(); x++) {
        if(*str == stopWords[x]) return true;
    }
    return false;
}

//outputs the accuracy and missed tweets to a file
void Bayes::output(std::ofstream& out) {
    out << std::fixed << std::setprecision(3) << accuracy << std::endl;

    for(int x = 0; x < tweets.size(); x++) {
        if(!tweets[x]->correct) {
            out << tweets[x]->m_id << std::endl;
        }
    }
}