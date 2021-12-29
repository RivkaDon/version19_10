// Hodaya Raz 211540174, Rivka Doniger 324584531
#include "CLI.h"
#include <fstream>
#include <vector>

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new UploadCSVFile(dio));
    commands.push_back(new AlgorithmSettings(dio));
    commands.push_back(new DetectAnomalies(dio));
    commands.push_back(new DisplayResults(dio));
    commands.push_back(new UploadAnomaliesAnalyzeResults(dio));
    commands.push_back(new Exit(dio));
}

void CLI::start(){
    currentStatus CT;
    float choice = 0;
    while (choice != 5) {
        dio->write("Welcome to the Anomaly Detection Server.\n"
                   "Please choose an option:\n");
        for (int i = 0; i < commands.size(); i++) {
            dio->write(i+1);
            dio->write(".");
            dio->write(commands.at(i)->description + "\n");
        }
        string input = dio->read();
        choice = input[0]-'0'-1;
        commands[choice]->execute(&CT);
    }
}


CLI::~CLI() {
}

