#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector(){

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    int m, c;
    float p;
    for (int i = 0; i < ts.getFeatures().size(); i++) {
        m = 0;
        c = -1;
        //const float* iCollum = &ts.getFeatures().at(i).second.at(0);
        float arri[ts.getFeatures().at(i).second.size()];
        std::copy(ts.getFeatures().at(i).second.begin(), ts.getFeatures().at(i).second.end(), arri);
        float* arrip = arri;
        for (int j = i + 1; j < ts.getFeatures().size(); j++) {
            //const float* jCollum = &ts.getFeatures().at(j).second.at(0);
            float arrj[ts.getFeatures().at(j).second.size()];
            std::copy(ts.getFeatures().at(j).second.begin(), ts.getFeatures().at(j).second.end(), arrj);
            float* arrjp = arrj;
            p = abs(pearson(arrip, arrjp, ts.getFeatures().at(i).second.size()));
            if (p > m) {
                m = p;
                c = j;
            }
        }
        // if c equals -1 so has no correlative feature
        if (c!= -1) {
            correlatedFeatures corrf;
            corrf.feature1 = ts.getFeatures().at(i).first;
            corrf.feature2 = ts.getFeatures().at(c).first;
            corrf.corrlation = p;
            Point **arrPoint[ts.getFeatures().at(i).second.size()];
            for (int k = 0; k < ts.getFeatures().at(i).second.size(); k++) {
                Point p(ts.getFeatures().at(i).second.at(k), ts.getFeatures().at(c).second.at(k));
                Point *t = &p;
                arrPoint[k] = &t;
            }
            corrf.lin_reg = linear_reg(*arrPoint, ts.getFeatures().at(i).second.size());
            corrf.threshold = 0;
            cf.push_back(corrf);
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> vector;
    return vector;
}

