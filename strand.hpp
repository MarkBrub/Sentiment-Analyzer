#pragma once
#include <exception>
#include <functional>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>

#include "stemmer.hpp"

class strand {
    //The goal of this class is to implement many of the <string> functions so that the
    //two classes can be interchanged
    //There are some additional functions to aid in this project
    char* m_data = nullptr;
    int m_size = 0;
    int m_capacity = 0;
public:
    //some of the functions have range checks and some don't
    //constructors
    strand();
    explicit strand(int len);
    strand(const strand& str);
    strand(const char* str);
    strand(char c);

    //destructor
    ~strand();

    //assignment operators
    strand& operator=(const strand& str);
    strand& operator=(const char* str);
    strand& operator=(char c);

    //capacity
    int size() const;
    void resize(int len);
    int capacity() const;
    void clear();
    bool empty() const;

    //element access
    char& operator[](int x);
    const char& operator[](int x) const;

    //modifiers
    strand& operator+=(const strand& str);
    strand& operator+=(const char* str);
    strand& append(const strand& str);
    strand& append(char* str);
    void push_back(char c);
    strand& insert(int pos, const strand& str);
    strand& insert(int pos, const char* str);
    strand& erase(int pos, int len);
    char pop_back();

    //operations
    char* c_str() const;
    char* data() const;
    int find(const strand& str, int pos = 0);
    int find(const char* str, int pos = 0);
    strand substr(int pos, int len);

    //other standard functions and operator overloads
    friend strand operator+(strand lhs, const strand& rhs);
    friend strand operator+(strand lhs, const char* rhs);
    friend bool operator==(const strand& lhs, const strand& rhs);
    friend bool operator==(const strand& lhs, const char* rhs);
    friend bool operator!=(const strand& lhs, const strand& rhs);
    friend bool operator!=(const strand& lhs, const char* rhs);
    friend std::ostream& operator<<(std::ostream& out, const strand& str);
    friend std::istream& operator>>(std::istream& in, strand& str);
    long long toLongLong();

    //custom functions
    void outputValues() const;
    strand popFirstSegment(char delimiter = ',');
    strand popLastSegment(char delimiter = ',');
    void removeFirstSegment(char delimiter = ',');
    void toLower();
    void removePunctuation();
    bool isURL();
    void stemStrand();
};

//For use in std::unordered_map
template <> struct std::hash<strand> {
    size_t operator()(const strand& str) const {
        //adapted from:
        // http://www.cse.yorku.ca/~oz/hash.html
        size_t h = 5381;

        for(int x = 0; x < str.size(); x++) {
            h = ((h << 5) + h) + str[x];
        }

        return h;
    }
};