// Hodaya Raz 211540174, Rivka Doniger 324584531

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
    Circle min_Circle;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf;
    float threshold = 0.9;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void learnNormalHelper(const TimeSeries& ts, float m);
    virtual float sendToCalcThreshold(correlatedFeatures corf, int size, Point** arrPoint);
    virtual float calculateDistance(correlatedFeatures cf, float x, float y);
    Point** createPointsArr(vector<float> &x, vector<float> &y, int size);
    void setThreshold(float num);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
