cmake_minimum_required(VERSION 3.20)
project(version18_10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(version18_10 SimpleAnomalyDetector.cpp timeseries.cpp anomaly_detection_util.cpp main.cpp
        TimeSeriesAnomalyDetector.cpp AnomalyReport.cpp AnomalyReport.h minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h commands.h Server.h Server.cpp)
