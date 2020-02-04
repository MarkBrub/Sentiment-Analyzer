#include <iostream>
#include "strand.hpp"
#include "analyzer.hpp"

int main(int argc, char **argv) {
    std::ifstream fin(argv[5]);
    analyzer anal;

    anal.inputTweets(fin);
    anal.output();

    return 0;
}