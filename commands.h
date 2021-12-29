// Hodaya Raz 211540174, Rivka Doniger 324584531

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

struct currentStatus {
    float threshold;
    vector <AnomalyReport> reports;
    float lineCount = 0;
    currentStatus(){
        threshold = 0.9;
    }
};

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

    void readAndCreateFile(string fileName, currentStatus* CT){
        CT->lineCount = -1;
        ofstream out(fileName);
        string s="";
        while((s=read())!="done"){
            out<<s<<endl;
            CT->lineCount++;
        }
        out.close();
    }
};

//class StandardIO:public DefaultIO {
//public:
//    StandardIO();
//    virtual ~StandardIO();
//    virtual string read() {
//        string str;
//        cin >> str;
//        return str;
//    }
//
//    virtual void write(string text) {
//        cout << text << endl;
//    }
//
//    virtual void write(float f) {
//        cout << f << endl;
//    }
//
//    virtual void read(float* f) {
//        cin >> *f;
//    }
//};

// you may add here helper classes


// you may edit this class
class Command{
protected:
    DefaultIO* dio;
public:
    const string description;
    Command(DefaultIO* dio,const string description):dio(dio),description(description){}
	virtual void execute(currentStatus* CT)=0;
	virtual ~Command(){}
};

class UploadCSVFile: public Command{
public:
    UploadCSVFile(DefaultIO* dio): Command(dio, "upload a time series csv file"){}
    void execute(currentStatus* CT){
        dio->write("Please upload your local train CSV file.\n");
        dio->readAndCreateFile("TrainFile.csv", CT);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->readAndCreateFile("TestFile.csv", CT);
        dio->write("Upload complete.\n");
    }
};


class AlgorithmSettings: public Command{
public:
    AlgorithmSettings(DefaultIO* dio): Command(dio, "algorithm settings"){}
    virtual void execute(currentStatus* CT){
        float chosenThreshold;
        bool good=false;
        while(!good){
            dio->write("The current correlation threshold is ");
            dio->write(CT->threshold);
            dio->write("\nType a new threshold\n");
            dio->read(&chosenThreshold);
            if(chosenThreshold > 0 && chosenThreshold <= 1){
                CT->threshold=chosenThreshold;
                good=true;
            }
            else
                dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

class DetectAnomalies: public Command{
public:
    DetectAnomalies(DefaultIO* dio): Command(dio, "detect anomalies"){}
    virtual void execute(currentStatus* CT){
        HybridAnomalyDetector hybridAnomalyDetector;
        // update threshold of anomaly detector to have the current threshold
        hybridAnomalyDetector.setThreshold(CT->threshold);
        // create two time series
        TimeSeries train("TrainFile.csv");
        TimeSeries test("TestFile.csv");

        hybridAnomalyDetector.learnNormal(train);
        // put anomalies into vector
        CT->reports = hybridAnomalyDetector.detect(test);

        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResults: public Command{
public:
    DisplayResults(DefaultIO* dio): Command(dio, "display results"){}
    virtual void execute(currentStatus* CT){
        for (AnomalyReport r1: CT->reports) {
             dio->write(r1.timeStep);
             dio->write("\t");
             dio->write(r1.description);
             dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

class UploadAnomaliesAnalyzeResults: public Command{
public:
    UploadAnomaliesAnalyzeResults(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    virtual void execute(currentStatus* CT){
        vector<pair<int, int>> startAndEndReport;
        vector<pair<int, int>> startAndEndAnomaly;
        float N = CT->lineCount;
        float reportCounter = 0;
        float FP = 0, TP = 0;
        int token[2];
        dio->write("Please upload your local anomalies file.\n");
        string s="";
        string description;
        vector<AnomalyReport>::iterator looping = CT->reports.begin();
        // inserts the anomalies after shrinking them together
        while (looping != CT->reports.end()) {
            int start = looping->timeStep;
            int end = 0;
            description = looping->description;
            while (looping->description == description) {
                end = looping->timeStep;
                looping++;
            }
            startAndEndAnomaly.push_back(make_pair(start, end));
        }
        // inserts the reports into vector
        while((s = dio->read())!="done") {
            // extracting the anomaly report start time and end time
            stringstream string_stream(s);  // creating string stream object
            int i = 0;
            while (string_stream.good()) {
                string a;
                getline(string_stream, a, ',');
                token[i] = stoi(a);
                i++;
            }
            startAndEndReport.push_back(make_pair(token[0], token[1]));
            reportCounter++;
            N = N - (token[1] - token[0] + 1);
        }
        sort(startAndEndReport.begin(), startAndEndReport.end());
        vector<pair<int, int>>::iterator itReport = startAndEndReport.begin();
        vector<pair<int, int>>::iterator itAnomaly = startAndEndAnomaly.begin();
        // go over all reports
        while (itAnomaly != startAndEndAnomaly.end()) {
            // if we finished going over reports not over anomalies
            if (itReport == startAndEndReport.end()) {
                while (itAnomaly != startAndEndAnomaly.end()) {
                    itAnomaly++;
                    FP++;
                }
                break;
            }
            // anomaly ends before report starts
            while (itAnomaly->second < itReport->first) {
                FP++;
                break;
            }
            // anomaly starts in the middle of report
            if ( itAnomaly->first <= itReport->first && itAnomaly->second >= itReport->first ) {
                itReport++;
                TP++;
            }
            // anomaly ends in the middle of report
            else if ( itAnomaly->first <= itReport->second && itAnomaly->second >= itReport->second ) {
                itReport++;
                TP++;
            }
            // anomaly included in the middle of report
            else if ( itAnomaly->first >= itReport->first && itAnomaly->second <= itReport->second ) {
                itReport++;
                TP++;
            }
            // report included in the middle of anomaly
            else if ( itAnomaly->first <= itReport->first && itAnomaly->second >= itReport->second ) {
                itReport++;
                TP++;
            }
            // anomaly starts after the end time in report
            else if (itReport->second < itAnomaly->first) {
                itReport++;
                continue;
            }
            itAnomaly++;
        }
        dio->write("Upload complete.\n");
        float truePositiveRate=((int)(1000.0*TP/reportCounter))/1000.0f;
        float falseAlarmRate=((int)(1000.0*FP/N))/1000.0f;
        dio->write("True Positive Rate: ");
        dio->write(truePositiveRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(falseAlarmRate);
        dio->write("\n");
    }
};

class Exit: public Command{
public:
    Exit(DefaultIO* dio):Command(dio,"exit"){}
    virtual void execute(currentStatus* CT){
        return;
    }
};


#endif /* COMMANDS_H_ */
