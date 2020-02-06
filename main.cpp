#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include "strand.hpp"
#include "analyzer.hpp"

int runTests() {
    return Catch::Session().run();
}

int main(int argc, char **argv) {
    if(argc < 2) return runTests();

    std::ios_base::sync_with_stdio(false);
    std::ifstream data(argv[1]);
    std::ifstream target(argv[2]);
    analyzer anal;

    anal.inputTweets(data, target);
    //anal.output();

    data.close();
    target.close();

    return 0;
}