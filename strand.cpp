#include "strand.hpp"

//constructors -----------------------------------------------------------------------------------
strand::strand() {
    m_data = nullptr;
}
strand::strand(int len) {
    if(len < 0) throw std::invalid_argument("strand must be a frequency length");
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
    m_capacity = m_size;
    m_data = new char[m_size];
    memcpy(m_data, str, m_size);
}
strand::strand(char c) {
    m_size = 1;
    m_capacity = 1;
    m_data = new char[1];
    m_data[0] = c;
}

//destructor
strand::~strand() {
    delete[] m_data;
}

//assignment operators ---------------------------------------------------------------------------
strand& strand::operator=(const strand& str) {
    if(*this == str) return *this;

    if(str.size() > m_capacity) resize(str.size());
    //if(str.size() < m_size) erase(str.size(), m_size - str.size());
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

//capacity ---------------------------------------------------------------------------------------
//returns the local size
int strand::size() const {
    return m_size;
}
//creates a new strand of the desired capacity
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
//return the local capacity
int strand::capacity() const {
    return m_capacity;
}
//sets all chars in the allocated memory to 0
void strand::clear() {
    memset(m_data, 0, m_capacity);
}
//returns true if the size is 0
bool strand::empty() const {
    return !m_size;
}

//element access ----------------------------------------------------------------------------------
//gives access to the char at the specified location
char& strand::operator[](int x) {

    if(x >= m_size) {
        throw std::out_of_range("I have no idea why this hits");
        std::cout << "operator[]: access out of range " << m_data[x] << " " << (int)*m_data <<  std::endl;
    }

    return m_data[x];
}
const char& strand::operator[](int x) const {
    if(x >= m_capacity) {
        std::cout << "const operator[]: access out of range" << std::endl;
        outputValues();
    }

    return m_data[x];
}

//modifiers ---------------------------------------------------------------------------------------
//adds a strand to the end of this one
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
//same as +=
strand& strand::append(const strand& str) {
    return *this += str;
}
strand& strand::append(char* str) {
    return *this += str;
}
//adds a single char to the end of the strand
void strand::push_back(char c) {
    if(m_capacity < m_size + 1) resize(m_size + 1);
    m_data[m_size] = c;
    m_size++;
}
//adds a sub strand to the desired location in the strand
strand& strand::insert(int pos, const strand&str) {
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
//removes desired length at specified position
strand& strand::erase(int pos, int len) {
    memmove(m_data + pos, m_data + pos + len, m_size - len);
    m_size -= len;

    return *this;
}
//removes and returns the last char
char strand::pop_back() {
    m_size--;
    return m_data[m_size + 1];
}

//operations --------------------------------------------------------------------------------------
//returns a version of the c-string with a null terminator at the end
char* strand::c_str() const {
    //return a c-string that is not stored in the class
    char* temp = new char[m_size + 1];
    memcpy(temp, m_data, m_size);
    temp[m_size] = '\0';

    return temp;
}
//returns address of the data
char* strand::data() const {
    return m_data;
}
//returns the position of specified sub strand, starting position can be specified
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
//returns sub strand of length at position
strand strand::substr(int pos, int len) {
    if(pos < 0 || pos > m_size) throw std::out_of_range("invalid position for substr");
    if(len < 0 || len > m_size - pos) throw std::out_of_range("invalid position for substr");

    strand temp(len);
    memcpy(temp.m_data, m_data + pos, len);
    temp.m_size = len;

    return temp;
}

//other standard functions and operator overloads -------------------------------------------------
//returns a new strand that is a combination of the two passed in
strand operator+(strand lhs, const strand& rhs) {
    lhs += rhs;

    return lhs;
}
strand operator+(strand lhs, const char* rhs) {
    lhs += rhs;

    return lhs;
}
//compares the data in the strands but not the capacity
bool operator==(const strand& lhs, const strand& rhs) {
    if(lhs.empty() || rhs.empty()) return lhs.empty() && rhs.empty();
    if(lhs.m_data == nullptr || rhs.m_data == nullptr) return false;
    if(lhs.m_size != rhs.m_size) return false;

    for(int x = 0; x < lhs.m_size; x++) {
        if(lhs[x] != rhs[x]) return false;
    }
    return true;
}
bool operator==(const strand& lhs, const char* rhs) {
    int len = std::strlen(rhs);
    if(len == 0) return lhs.empty();
    if(lhs.m_data == nullptr) return false;
    if(lhs.m_size != len) return false;

    for(int x = 0; x < len; x++) {
        if(lhs[x] != rhs[x]) return false;
    }
    return true;
}
//returns !==
bool operator!=(const strand& lhs, const strand& rhs) {
    if(lhs.m_data == nullptr) return false;
    return !(lhs == rhs);
}
bool operator!=(const strand& lhs, const char* rhs) {
    if(lhs.m_data == nullptr) return false;
    return !(lhs == rhs);
}
//for reading in from a file stream
std::ostream& operator<<(std::ostream& out, const strand& str) {
    //makes output faster but no longer buffers with c output functions
    std::ios_base::sync_with_stdio(false);
    for(int x = 0; x < str.m_size; x++) {
        std::cout << str[x];
    }

    return out;
}
//for outputting to a file stream
std::istream& operator>>(std::istream& in, strand& str) {
    char* buffer = new char[1024];
    in.getline(buffer, 1024);
    int len = strlen(buffer);

    memcpy(str.m_data, buffer, len);
    str.m_size = len;

    delete[] buffer;

    return in;
}
//converts stored data to numerical value, undefined behavior if data in not a number
long long strand::toLongLong() {
    char* c = c_str();
    long long temp = std::strtoll(c, nullptr, 10);
    delete[] c;

    return temp;
}

//custom functions --------------------------------------------------------------------------------
//outputs the size, capacity and the ASCII value of all chars in capacity
void strand::outputValues() const {
    //makes output faster but no longer buffers with c output functions
    std::ios_base::sync_with_stdio(false);

    std::cout << "Size: " << m_size << " Capacity: " << m_capacity << std::endl;

    for(int x = 0; x < m_capacity; x++) {
        std::cout << (int)m_data[x] << " ";
    }
}
//removes and returns up to the delimiter
strand strand::popFirstSegment(char delimiter) {
    if(m_size == 0){
        strand temp("~");
        return temp;
    }

    for(int x = 0; x < m_size; x++) {
        if(m_data[x] == delimiter) {
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
//returns and removes from the end to the next delimiter
strand strand::popLastSegment(char delimiter) {
    for(int x = m_size - 1; x >= 0; x--) {
        if(m_data[x] == delimiter) {
            int size = m_size - x - 1;

            if(size == 0) {
                m_size--;
                return strand(delimiter);
            }

            strand temp(size);

            //dont include the delimiter character
            memcpy(temp.m_data, m_data + x + 1, m_size - x - 1);
            temp.m_size = m_size - x - 1;
            m_size -= (m_size - x);

            //remove any extra white space
            while(x > 0 && (m_data[x - 1] == delimiter || m_data[x - 1] == ',' || m_data[x - 1] == '.' || m_data[x - 1] == '!')) {
                m_size--;
                x--;
            }


            return temp;
        } 
    }

    if(m_size > 0) {
        strand temp(m_size);
        memcpy(temp.m_data, m_data, m_size);
        temp.m_size = m_size;
        m_size = 0;
        return temp;
    } else throw std::logic_error("String empty");
}
//removes up to the delimiter
void strand::removeFirstSegment(char delimiter) {
    for(int x = 0; x < m_size; x++) {
        if(m_data[x] == delimiter) {
            erase(0, x + 1);
            /*m_data += (x + 1);
            m_size -= (x + 1);
            m_capacity -= (x + 1);*/

            return;
        }
    }
}
//converts all letters to their lowercase version
void strand::toLower() {
    for(int x = 0; x < m_size; x++) {
        if(m_data[x] > 64 && m_data[x] < 91) {
            m_data[x] += 32;
        }
    }
}
//removes an array of non letter characters
void strand::removePunctuation() {
    while(m_data[m_size - 1] < 64 || m_data[m_size - 1] > 122) m_size--;
}
//returns if the stored value starts with www or http
bool strand::isURL() {
    if(m_size < 3) return false;
    if(m_data[0] == 'w' && m_data[1] == 'w' && m_data[2] == 'w') return true;
    if(m_size == 3) return false;
    if(m_data[0] == 'h' && m_data[1] == 't' && m_data[2] == 't' && m_data[3] == 'p') return true;
    return false;
}
