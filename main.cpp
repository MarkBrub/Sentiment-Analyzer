#include <iostream>
#include "strand.hpp"
#include "analyzer.hpp"

int main(int argc, char **argv) {
    std::ios_base::sync_with_stdio(false);
    std::ifstream data(argv[1]);
    std::ifstream target(argv[2]);
    analyzer anal;

    anal.inputTweets(data, target);
    anal.output();

    data.close();
    target.close();

    return 0;
}