cmake_minimum_required(VERSION 3.15)
project(S20-PA01-SentimentAnalysis)
set (EXE_NAME SentimentAnalyzer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set (input01 "dev-train-data-mini.csv")
set (input02 "dev-train-target-mini.csv")
set (input03 "dev-test-data-mini.csv")
set (input04 "dev-test-target-mini.csv")


# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03 input04 )
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 17)
add_executable(${EXE_NAME} main.cpp strand.cpp bayes.cpp tweet.cpp)
# add_executable(${EXE_NAME} main.cpp strand.cpp analyzer.cpp bayes.cpp tweet.cpp catch.cpp)