

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class TimeSeries{
    // Create a vector of <string, float vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<float>>> features;
public:
	TimeSeries(const char* CSVfileName):features() {
        // Create an input filestream
        std::ifstream dataFile(CSVfileName);
        // Make sure the file is open
        if(!dataFile.is_open()) throw runtime_error("file is not opened");
        string line, featureName;
        float num;
        // Takes the first line in the file- that has the feature names
        std::getline(dataFile, line);
        // Makes a stringstream from line
        std::stringstream featuresInString(line);
        // Separate all the feature names
        while (std::getline(featuresInString, featureName, ',')) {
            features.push_back({featureName, std::vector<float>{}});
        }
        while(std::getline(dataFile, line)) {
            // Makes a stringstream from line
            std::stringstream dataInString(line);
            int index = 0;
            // separates each float
            while (dataInString >> num) {
                // Add value to vector
                features.at(index).second.push_back(num);
                // Ignore if next string is a comma
                if (dataInString.peek() == ',')
                    dataInString.ignore();
                index++;
            }
        }
        dataFile.close();
//        for (int i = 0; i < features.size(); i++) {
//            cout << features.at(i).first << endl;
//            for (int j = 0; j < features.at(i).second.size(); j++) {
//                cout << features.at(i).second.at(j) << endl;
//            }
//        }
    }
    const vector<pair<string, vector<float>>>& getFeatures() const{
        return features;
    }

    string getFeatureName (int colm) {
        return features.at(colm).first;
    }

};



#endif /* TIMESERIES_H_ */
