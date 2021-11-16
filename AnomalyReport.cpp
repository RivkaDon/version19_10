// Hodaya Raz 211540174, Rivka Doniger 324584531
#include <vector>
#include <string>

class AnomalyReport{
public:
    const std::string description;
    const long timeStep;
    AnomalyReport(std::string description, long timeStep) :
            description(description),timeStep(timeStep){}
}; 