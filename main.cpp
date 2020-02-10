#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <chrono>
#include "analyzer.hpp"

int runTests() {
    return Catch::Session().run();
}

int main(int argc, char **argv) {
    auto start = std::chrono::steady_clock::now();

    if(argc < 2) return runTests();

    std::ifstream trainData(argv[1]);
    std::ifstream trainTarget(argv[2]);
    std::ifstream testData(argv[3]);
    std::ifstream testTarget(argv[4]);
    std::ofstream out(argv[5]);
    analyzer anal;

    anal.inputTweets(trainData, trainTarget);
    anal.classifyTweets(testData, testTarget);
    anal.output(out);

    trainData.close();
    trainTarget.close();
    testData.close();
    testTarget.close();
    out.close();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Run in milliseconds : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    return 0;
}