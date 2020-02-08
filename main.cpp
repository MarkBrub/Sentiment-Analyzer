#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <chrono>
#include "strand.hpp"
#include "analyzer.hpp"

int runTests() {
    return Catch::Session().run();
    return 0;
}

int main(int argc, char **argv) {
    auto start = std::chrono::steady_clock::now();

    if(argc < 2) return runTests();

    std::ios_base::sync_with_stdio(false);
    std::ifstream trainData(argv[5]);
    std::ifstream trainTarget(argv[6]);
    std::ifstream testData(argv[3]);
    std::ifstream testTarget(argv[4]);
    std::ofstream output("output01.txt");
    analyzer anal;

    anal.inputTweets(trainData, trainTarget);
    anal.classifyTweets(testData, testTarget);
    //anal.output(10);

    trainData.close();
    trainTarget.close();
    testData.close();
    testTarget.close();
    output.close();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed time in milliseconds : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    return 0;
}