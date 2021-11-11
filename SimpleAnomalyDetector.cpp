#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <string.h>
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
            correlatedFeatures corrf;
            corrf.feature1 = ts.getFeatures().at(i).first;
            corrf.feature2 = ts.getFeatures().at(c).first;
            cout<< corrf.feature1 << "      " << corrf.feature2<<endl;
            corrf.corrlation = m;
            Point **arrPoint[ts.getFeatures().at(i).second.size()];
            // creates an array of pointers to Points using the data of the correlative features
            for (int k = 0; k < ts.getFeatures().at(i).second.size(); k++) {
                Point p(ts.getFeatures().at(i).second.at(k), ts.getFeatures().at(c).second.at(k));
                Point *t = &p;
                arrPoint[k] = &t;
            }
            corrf.lin_reg = linear_reg(*arrPoint, ts.getFeatures().at(i).second.size());
            // calculates the highest threshold
            for (int l = 0; l < ts.getFeatures().at(i).second.size(); l++) {
                if (dev(*(*arrPoint[l]), corrf.lin_reg) > maxT)
                    maxT = dev(*(*arrPoint[l]), corrf.lin_reg);
                //cout << dev(**arrPoint[l], corrf.lin_reg) << "%%%%%%"<<endl;
            }
            corrf.threshold = maxT;
            // adds correlative feature to the vector cf
            cf.push_back(corrf);
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
    int colFeature1, colFeature2, x, y, distance;
    vector<AnomalyReport> anomalies;
    // to know how many correlations we need to check
    int sizeOfCorrelations = cf.size();
//    cout << getFeatureCollum("B", ts) << endl;
    // go over all features
    // the number of row.
    for (int i = 0; i < ts.getFeatures().at(0).second.size(); i++) {
        // go over correlation vector
        for (int j = 0; j < cf.size() ; j++) {
            colFeature1 = getFeatureCollum(cf.at(j).feature1, ts);
            colFeature2 = getFeatureCollum(cf.at(j).feature2, ts);
            x = ts.getFeatures().at(colFeature1).second.at(i);
            y = ts.getFeatures().at(colFeature2).second.at(i);
            Point p(x, y);
            distance = dev(p, cf.at(j).lin_reg);
            cout<< "cf.at(j).threshold   " << cf.at(j).threshold << endl;
            cout<< "distance    " << distance << endl;
            if (distance > cf.at(j).threshold) {
                cout<< "cf.at(j).threshold" << cf.at(j).threshold << endl;
                cout<< "distance" << distance << endl;
                AnomalyReport ar(cf.at(j).feature1 + "-" + cf.at(j).feature2,i);
                cout<< "********" << cf.at(j).feature1 + "-" + cf.at(j).feature2 << endl;
                anomalies.push_back(ar);
            }
        }
    }
    return anomalies;
}

