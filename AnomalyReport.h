
#ifndef VERSION18_10_ANOMALYREPORT_H
#define VERSION18_10_ANOMALYREPORT_H
#include <vector>
#include <string>

class AnomalyReport{
public:
    const std::string description;
    const long timeStep;
    AnomalyReport(std::string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
#endif //VERSION18_10_ANOMALYREPORT_H
