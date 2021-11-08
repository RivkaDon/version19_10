cmake_minimum_required(VERSION 3.20)
project(version18_10)

set(CMAKE_CXX_STANDARD 11)

add_executable(version18_10 SimpleAnomalyDetector.cpp timeseries.cpp anomaly_detection_util.cpp main.cpp TimeSeriesAnomalyDetector.cpp AnomalyReport.cpp AnomalyReport.h)
