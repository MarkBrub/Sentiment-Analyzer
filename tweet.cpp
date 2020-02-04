#include "tweet.hpp"

Tweet::Tweet() {

}
Tweet::Tweet(long id, strand user) {
    m_id = id;
    m_user = user;
}
