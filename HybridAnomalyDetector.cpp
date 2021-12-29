// Hodaya Raz 211540174, Rivka Doniger 324584531
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    float m = 0.5;
    learnNormalHelper(ts, m);
}

float HybridAnomalyDetector::sendToCalcThreshold(correlatedFeatures corf, int size, Point **arrPoint) {
    if (corf.corrlation >= threshold) {
        return SimpleAnomalyDetector::sendToCalcThreshold(corf, size, arrPoint);
    }
    return corf.min_Circle.radius;
}

float HybridAnomalyDetector::calculateDistance(correlatedFeatures cf, float x, float y) {
    if (cf.corrlation >= threshold) {
        return SimpleAnomalyDetector::calculateDistance(cf, x, y);
    }
    return sqrt( pow((cf.min_Circle.center.x - x),2) + pow((cf.min_Circle.center.y - y), 2));
}



