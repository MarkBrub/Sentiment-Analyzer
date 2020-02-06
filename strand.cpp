#include "strand.hpp"

//constructor
strand::strand() {
    m_data = nullptr;
}
strand::strand(int len) {
    if(len < 0) throw std::invalid_argument("strand must be a positive length");
    m_data = new char[len];
    m_capacity = len;
}
strand::strand(const strand& str) {
    m_data = new char[str.m_size];
    
    m_size = str.size();
    m_capacity = m_size;
    memcpy(m_data, str.data(), m_size);
}
strand::strand(const char* str) {
    m_size = strlen(str);
    //if(m_size == 0) return;

    m_capacity = m_size;
    m_data = new char[m_size];
    memcpy(m_data, str, m_size);
}
strand::strand(char c) {
    m_size = 1;
    m_capacity = 1;
    m_data = new char[m_size];
    m_data[0] = c;
}

//destructor
strand::~strand() {
    delete[] m_data;
}

//assignment operators
strand& strand::operator=(const strand& str) {
    if(*this == str) return *this;

    if(str.size() > m_capacity) resize(str.size());
    if(str.size() < m_size) erase(str.size(), m_size - str.size());
    memcpy(m_data, str.data(), str.size());
    m_size = str.size();

    return *this;
}
strand& strand::operator=(const char* str) {
    if(str == nullptr) {
        throw std::invalid_argument("cannot set strand to nullptr");
    }
    if(*this == str) return *this;

    int len = strlen(str);

    if(len > m_capacity) resize(len);
    if(len < m_size) erase(len, m_size - len);
    memcpy(m_data, str, len);
    m_size = len;

    return *this;
}
strand& strand::operator=(const char c) {
    if(1 > m_capacity) resize(1);

    *m_data = c;
    m_size = 1;

    return *this;
}

//capacity
int strand::size() const {
    return m_size;
}
void strand::resize(int len) {
    if(len < 0) {
        throw std::invalid_argument("invalid capacity");
    }

    if(m_data == nullptr) {
        m_data = new char[len];
        m_size = len;
        m_capacity = m_size;
        return;
    }

    char* temp = new char[len];

    memcpy(temp, m_data, len);
    delete[] m_data;
    m_data = temp;
    m_capacity = len;
}
int strand::capacity() const {
    return m_capacity;
}
void strand::clear() {
    memset(m_data, 0, m_size);
}
bool strand::empty() const {
    return !m_size;
}

//element access
char& strand::operator[](int x) {
    if(x >= m_capacity) {
        std::cout << "operator[]: access out of range" << std::endl;
        //return ;
    }

    return *(m_data + x);
}
const char& strand::operator[](int x) const {
    if(x >= m_capacity) {
        std::cout << "operator[]: access out of range" << std::endl;
        //return ;
    }

    return *(m_data + x);
}
char& strand::front() {
    if(m_size < 1) {
        std::cout << "strand empty: cannot call front" << std::endl;
        //why doesnt this work and what should it do
        //return '\0';
        return *m_data;
    }

    return *m_data;
}
const char& strand::front() const {
    if(m_size < 1) {
        std::cout << "strand empty: cannot call front" << std::endl;
        //return '\0';
        return *m_data;
    }

    return *m_data;
}
char& strand::back() {
        if(m_size < 1) {
        std::cout << "strand empty: cannot call back" << std::endl;
        //why doesnt this work and what should it do
        //return '\0';
        return *m_data;
    }

    return m_data[m_size - 1];
}
const char& strand::back() const {
    if(m_size < 1) {
        std::cout << "strand empty: cannot call back" << std::endl;
        //return '\0';
        return *m_data;
    }

    return m_data[m_size - 1];
}

//modifiers
strand& strand::operator+=(const strand& str) {
    //I feel like this is a cop out, either way it is slower
    //*this = *this + str;
    //return *this

    if(m_capacity < m_size + str.size()) {
        resize(m_size + str.size());
    }
    memcpy(m_data + m_size, str.data(), str.size());
    m_size += str.size();

    return *this;
}
strand& strand::operator+=(const char* str) {
    int len = strlen(str);

    if(m_capacity < m_size + len) {
        resize(m_size + len);
    }
    memcpy(m_data + m_size, str, len);
    m_size += len;

    return *this;
}
strand& strand::append(const strand& str) {
    return *this += str;
}
strand& strand::append(char* str) {
    return *this += str;
}
void strand::push_back(char c) {
    if(m_capacity < m_size + 1) resize(m_size + 1);
    m_data[m_size] = c;
    m_size++;
}
strand& strand::insert(int pos, const strand& str) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for insert");
    if(m_capacity < m_size + str.size()) {
        resize(m_size + str.size());
    }

    memcpy(m_data + pos + str.size(), m_data + pos, str.size());
    memcpy(m_data + pos, str.data(), str.size());
    m_size += str.size();
    
    return *this;
}
strand& strand::insert(int pos, const char* str) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for insert");

    int len = strlen(str);

    if(m_capacity < m_size + len) {
        resize(m_size + len);
    }
    for(int x = pos; x < m_size; x++) {
        m_data[x + len] = m_data[x];
    }
    memcpy(m_data + pos, str, len);
    m_size += len;
    
    return *this;
}
strand& strand::erase(int pos, int len) {
    memmove(m_data + pos, m_data + pos + len, m_size - len);
    m_size -= len;
    memset(m_data + m_size, 0, len);
    
    return *this;
}
char strand::pop_back() {
    char temp = back();

    erase(m_size, 1);
    m_size -= 1;

    return temp;
}

//operations
char* strand::c_str() const {
    //return a c-string that is not stored in the class
    char* temp = new char[m_size + 1];
    memcpy(temp, m_data, m_size);
    temp[m_size] = '\0';

    return temp;
}
char* strand::data() const {
    return m_data;
}
int strand::find(const strand& str, int pos) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for find");
    
    for(int x = pos; x < m_size; x++) {
        if(m_data[x] == str[0]) {
            for(int y = 0; y < str.size(); y++) {
                if(x + y > m_size) return -1;
                if(m_data[x + y] != str[y]) break;
                if(y == str.size() - 1) return x;
            }
        }
    }

    return -1;
}
int strand::find(const char* str, int pos) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for find");

    int len = strlen(str);
    
    for(int x = pos; x < m_size; x++) {
        if(m_data[x] == str[0]) {
            for(int y = 0; y < len; y++) {
                if(x + y > m_size) return -1;
                if(m_data[x + y] != str[y]) break;
                if(y == len - 1) return x;
            }
        }
    }

    return -1;
}
strand strand::substr(int pos, int len) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for substr");
    if(len < 0 || len > m_size - pos) throw std::out_of_range("invalid position for substr");

    strand temp(len);
    memcpy(temp.m_data, m_data + pos, len);

    return temp;
}

//other function and operator overloads
strand operator+(strand lhs, const strand& rhs) {
    lhs += rhs;

    return lhs;
}
strand operator+(strand lhs, const char* rhs) {
    lhs += rhs;

    return lhs;
}
bool operator==(const strand& lhs, const strand& rhs) {
    if(lhs.empty() || rhs.empty()) return lhs.empty() && rhs.empty();
    if(lhs.m_data == nullptr || rhs.m_data == nullptr) return false;
    return *lhs.data() == *rhs.data(); 
}
bool operator==(const strand& lhs, const char* rhs) {
    if(std::strlen(rhs) == 0) return lhs.empty();
    if(lhs.m_data == nullptr) return false;
    return *lhs.m_data == *rhs;
}
bool operator!=(const strand& lhs, const strand& rhs) {
    if(lhs.m_data == nullptr) return false;
    return !(lhs == rhs);
}
bool operator!=(const strand& lhs, const char* rhs) {
    if(lhs.m_data == nullptr) return false;
    return !(lhs == rhs);
}
std::ostream& operator<<(std::ostream& out, const strand& str) {
    //makes output faster but no longer buffers with c output functions
    std::ios_base::sync_with_stdio(false);
    for(int x = 0; x < str.m_size; x++) {
        std::cout << str[x];
        //std::cout << (int)str[x] << " ";
    }

    return out;
}
std::istream& operator>>(std::istream& in, const strand& str) {
    in.getline(str.m_data, str.m_capacity);

    return in;
}
void strand::outputValues() const {
    //makes output faster but no longer buffers with c output functions
    std::ios_base::sync_with_stdio(false);

    std::cout << "Size: " << m_size << " Capacity: " << m_capacity << std::endl;

    for(int x = 0; x < m_capacity; x++) {
        std::cout << (int)m_data[x] << " ";
    }
}
strand strand::popFirstSegment(char delimiter) {
    if(m_size == 0){
        strand temp("~");
        return temp;
    }

    for(int x = 0; x < m_size; x++) {
        if(x == m_size || m_data[x] == delimiter) {
            if(x == 0) {
                erase(0, 1);
                strand temp(delimiter);
                //in case the delimiter is ' '
                return temp;
            }
            strand temp(x);

            //dont include the delimiter character in the copy but erase it
            memcpy(temp.m_data, m_data, x);
            temp.m_size = x;
            erase(0, x + 1);

            return temp;
        }
    }

    //case if delimiter not found
    strand temp(m_size);
    memcpy(temp.m_data, m_data, m_size);
    temp.m_size = m_size;
    erase(0, m_size);

    return temp;
}
strand strand::popLastSegment(char delimiter) {
    for(int x = m_size - 1; x >= 0; x--) {
        if(m_data[x] == delimiter) {
            strand temp(m_size - x - 1);

            //dont include the delimiter character
            memcpy(temp.m_data, m_data + x + 1, m_size - x - 1);
            temp.m_size = m_size - x - 1;
            erase(x, m_size - x);

            return temp;
        } 
    }
    return strand("");
}
void strand::removeFirstSegment(char delimiter) {
    for(int x = 0; x < m_size; x++) {
        if(x == m_size || m_data[x] == delimiter) {
            erase(0, x + 1);

            return;
        }
    }
}
void strand::toLower() {
    for(int x = 0; x < m_size; x++) {
        if(m_data[x] > 64 && m_data[x] < 91) {
            m_data[x] += 32;
        }
    }
}
long long strand::strtol() {
    char* c = c_str();
    long long temp = std::strtoll(c, nullptr, 10);
    delete[] c;

    return temp;
}
char* strand::strtok(const char* delimiters) {
    return std::strtok(m_data, delimiters);
}
