#include <iostream>
#include <vector>
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;

// this is a simple test to put you on the right track
void generateTrainCSV(float a1,float b1, float a2, float b2){
    ofstream out("trainFile1.csv");
    out<<"A,B,C,D"<<endl;
    Line ac(a1,b1);
    Line bd(a2,b2);
    for(int i=1;i<=100;i++){
        float a=i;
        float b=rand()%40;
        out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
    }
    out.close();
}

void generateTestCSV(float a1,float b1, float a2, float b2, int anomaly){
    ofstream out("testFile1.csv");
    out<<"A,B,C,D"<<endl;
    Line ac(a1,b1);
    Line bd(a2,b2);
    for(int i=1;i<=100;i++){
        float a=i;
        float b=rand()%40;
        if(i!=anomaly)
            out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
        else
            out<<a<<","<<b<<","<<ac.f(a)+1<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
    }
    out.close();
}

void checkCorrelationTrain(correlatedFeatures c,string f1, string f2, float a, float b){
    if(c.feature1==f1){
        if(c.feature2!=f2)
            cout<<"wrong correlated feature of "<<f1<<" (-20)"<<endl;
        else{
            if(c.corrlation<0.99)
                cout<<f1<<"-"<<f2<<" wrong correlation detected (-5)"<<endl;
            if(c.lin_reg.a<a-0.5f || c.lin_reg.a>a+0.5f)
                cout<<f1<<"-"<<f2<<" wrong value of line_reg.a (-5)"<<endl;
            if(c.lin_reg.b<b-0.5f || c.lin_reg.b>b+0.5f)
                cout<<f1<<"-"<<f2<<" wrong value of line_reg.b (-5)"<<endl;
            if(c.threshold>0.3)
                cout<<f1<<"-"<<f2<<" wrong threshold detected (-5)"<<endl;
        }
    }

}

int main(){
    srand (time(NULL));
    float a1=1+rand()%10, b1=-50+rand()%100;
    float a2=1+rand()%20 , b2=-50+rand()%100;


    // test the learned model: (40 points)
    // expected correlations:
    //	A-C: y=a1*x+b1
    //	B-D: y=a2*x+b2

    generateTrainCSV(a1,b1,a2,b2);
    TimeSeries ts("trainFile1.csv");

//    SimpleAnomalyDetector ad;
//    ad.learnNormal(ts);
//    vector<correlatedFeatures> cf=ad.getNormalModel();
//
//    if(cf.size()!=2)
//        cout<<"wrong size of correlated features (-40)"<<endl;
//    else
//        for_each(cf.begin(),cf.end(),[&a1,&b1,&a2,&b2](correlatedFeatures c){
//            checkCorrelationTrain(c,"A","C",a1,b1); // 20 points
//            checkCorrelationTrain(c,"B","D",a2,b2); // 20 points
//        });
//
//    // test the anomaly detector: (60 points)
//    // one simply anomaly is injected to the data
//    int anomaly=5+rand()%90; // one anomaly injected in a random time step
//    generateTestCSV(a1,b1,a2,b2,anomaly);
//    TimeSeries ts2("testFile1.csv");
//    vector<AnomalyReport> r = ad.detect(ts2);
//
//    bool anomlyDetected=false;
//    int falseAlarms=0;
//    for_each(r.begin(),r.end(),[&anomaly,&anomlyDetected,&falseAlarms](AnomalyReport ar){
//        if(ar.description=="A-C" && ar.timeStep == anomaly)
//            anomlyDetected=true;
//        else
//            falseAlarms++;
//    });
//
//    if(!anomlyDetected)
//        cout<<"the anomaly was not detected (-30)"<<endl;
//
//    if(falseAlarms>0)
//        cout<<"you have "<<falseAlarms<<" false alarms (-"<<min(30,falseAlarms*3)<<")"<<endl;
//
//    cout<<"done"<<endl;
    return 0;
}
//#include <iostream>
//#include "anomaly_detection_util.h"
//#include "timeseries.h"
//
//using namespace std;
//
//bool wrong(float val, float expected){
//    return val<expected-0.001 || val>expected+0.001;
//}
//
//// this is a simple test to put you on the right track
//int main(){
//    TimeSeries("data.csv");
//    const int N=10;
//    float x[]={1,2,3,4,5,6,7,8,9,10};
//    float y[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};
//
//    Point* ps[N];
//    for(int i=0;i<N;i++)
//        ps[i]=new Point(x[i],y[i]);
//
//    Line l=linear_reg(ps,N);
//    Point p(4,8);
//
//    float v[]={var(x,N),cov(x,y,N),pearson(x,y,N),l.a,l.b,l.f(4),dev(p,l)};
//    float e[]={8.25,16.63,0.999,2.015,0.113,8.176,0.176};
//
//
//    for(int i=0;i<7;i++)
//        if(wrong(v[i],e[i]))
//            cout<<"error for check "<<i<<" (-14)"<<endl;
//
//
//    for(int i=0;i<N;i++)
//        delete ps[i];
//
//    cout<<"done"<<endl;
//    return 0;
//}