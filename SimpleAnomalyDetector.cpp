// Hodaya Raz 211540174, Rivka Doniger 324584531
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <vector>
SimpleAnomalyDetector::SimpleAnomalyDetector(){

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    int c;
    float p, m;
    float maxT = 0; // holding the highest threshold.
    // find the correlation for each feature
    for (int i = 0; i < ts.getFeatures().size(); i++) {
        m = 0.9;
        c = -1;
        // create array of data in i collum
        float arri[ts.getFeatures().at(i).second.size()];
        std::copy(ts.getFeatures().at(i).second.begin(), ts.getFeatures().at(i).second.end(), arri);
        float* arrip = arri;
        // copy of data in features in collum i
        std::vector<float> vi(ts.getFeatures().at(i).second);
        for (int j = i + 1; j < ts.getFeatures().size(); j++) {
            // create array of data in j collum
            float arrj[ts.getFeatures().at(j).second.size()];
            // copy of features because it is a const
            std::copy(ts.getFeatures().at(j).second.begin(), ts.getFeatures().at(j).second.end(), arrj);
            float* arrjp = arrj;
            // calculates the highest correlative feature to i
            p = abs(pearson(arrip, arrjp, ts.getFeatures().at(i).second.size()));
            if (p > m) {
                m = p;
                // c is the collum of the highest correlation
                c = j;
            }
        }
        // if c equals -1 so has no correlative feature
        if (c!= -1) {
            // copy of data in features in collum c
            std::vector<float> vc(ts.getFeatures().at(c).second);
            correlatedFeatures corrf;
            corrf.feature1 = ts.getFeatures().at(i).first;
            corrf.feature2 = ts.getFeatures().at(c).first;
            corrf.corrlation = m;
            // creates an array of pointers to Points using the data of the correlative features.
            Point** arrPoint = new Point*[ts.getFeatures().at(i).second.size()];
            for(int k = 0; k < ts.getFeatures().at(i).second.size(); k++) {
                arrPoint[k] = new Point(vi[k], vc[k]);
            }
            corrf.lin_reg = linear_reg(arrPoint, ts.getFeatures().at(i).second.size());
            // calculates the highest threshold
            for (int l = 0; l < ts.getFeatures().at(i).second.size(); l++) {
                float d = abs(arrPoint[l]->y - corrf.lin_reg.f(arrPoint[l]->x));
                if (d > maxT)
                    maxT = d;
            }
            corrf.threshold = 1.1 * maxT;
            // adds correlative feature to the vector cf
            cf.push_back(corrf);
            // delete points
            for(size_t k=0;k<ts.getFeatures().at(i).second.size();k++)
                delete arrPoint[k];
            delete[] arrPoint;
        }
    }
}
// receives a string and time series and returns the collum of the string in ts
int getFeatureCollum(string feature, const TimeSeries& ts) {
    for (int i = 0; i < ts.getFeatures().size(); i++) {
        if (ts.getFeatures().at(i).first == feature){
            return i;
        }
    }
    throw std::runtime_error("feature not found");
}
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    int colFeature1, colFeature2;
    float x, y, distance;
    vector<AnomalyReport> anomalies;
    int sizeOfPointVector = ts.getFeatures().at(0).second.size();
    // go over all rows in the vector in the correlated features data
    for (int i = 0; i < sizeOfPointVector; i++) {
        // go over correlations vector
        for (int j = 0; j < cf.size() ; j++) {
            colFeature1 = getFeatureCollum(cf.at(j).feature1, ts);
            colFeature2 = getFeatureCollum(cf.at(j).feature2, ts);
            x = ts.getFeatures().at(colFeature1).second.at(i);
            y = ts.getFeatures().at(colFeature2).second.at(i);
            // checks the distance between the point and the linear reg
            distance = abs(y - cf.at(j).lin_reg.f(x));
            // if there is an anomaly
            if (distance > cf.at(j).threshold) {
                AnomalyReport ar(cf.at(j).feature1 + "-" + cf.at(j).feature2, i + 1);
                anomalies.push_back(ar);
            }
        }
    }
    return anomalies;
}


