cmake_minimum_required(VERSION 3.14)
project(S20-PA01-SentimentAnalysis)
set (EXE_NAME SentimentAnalyzer)

set (input01 "dev-train-data.csv")
set (input02 "dev-train-target.csv")
set (input03 "dev-test-data.csv")
set (input04 "dev-test-target.csv")
set (input05 "dev-train-data-sm.csv")
set (input06 "dev-train-target-sm.csv")

# Copy data files to build dir
foreach(file IN LISTS input05 input06 input03 input04)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 17)
#add_executable(${EXE_NAME} main.cpp strand.cpp analyzer.cpp tweet.cpp bayes.hpp bayes.cpp)
add_executable(${EXE_NAME} main.cpp strand.cpp analyzer.cpp tweet.cpp catch.hpp catch.cpp)