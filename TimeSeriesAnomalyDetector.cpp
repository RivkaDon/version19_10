// Hodaya Raz 211540174, Rivka Doniger 324584531

#include "timeseries.h"
#include "AnomalyReport.h"

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};