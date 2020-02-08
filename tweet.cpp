#include "tweet.hpp"

Tweet::Tweet() {

}
Tweet::Tweet(long long id, strand user) {
    m_id = id;
    m_user = user;
}

int Tweet::size() {
    return m_words.size();
}
strand& Tweet::operator[](int x) {
    return m_words[x];
}

void Tweet::clampSentiment()  {
    if(m_sentiment > 0) m_sentiment = 1;
    if(m_sentiment < 0) m_sentiment = -1;
}