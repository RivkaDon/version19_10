// Hodaya Raz 211540174, Rivka Doniger 324584531
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual float sendToCalcThreshold(correlatedFeatures corf, int size, Point** arrPoint);
    virtual float calculateDistance(correlatedFeatures cf, float x, float y);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
