
//
//  peakdetection.hpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 17/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef peakdetection_hpp
#define peakdetection_hpp

#include <stdio.h>
#include <iostream>
using namespace std;


// declare Brain class and its properties + methods
class PeakDetection {
public:
    PeakDetection();

    int add_data_point(double reading);
    double get_max_reading();
    void load_calibration_data(double loadedMean, double loadedStdDeviation);
    void reset_max_values();
    
private:
    double mean, stdDeviation, maxMean, reading1, reading2, nStdDeviations, peakDetectDelay;
    int readingLag, nReadings;
    time_t lastPeakDetected;
    
    void update_mean_stdDeviation(double dataPoint);
};

#endif /* peakdetection_hpp */
