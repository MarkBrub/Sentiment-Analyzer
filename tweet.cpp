#include "tweet.hpp"

Tweet::Tweet() {

}
Tweet::Tweet(long long id, strand user) {
    m_id = id;
    m_user = user;
}
