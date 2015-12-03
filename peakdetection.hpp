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


/*!
 Concept:
 This class is used to analyse the output from the LDR sensor, and detect peaks in the signal which would indicate a reading being taken. By identifying the value of the peak the cracker colour can be identified.
 */

// declare PeakDetection class and its properties + methods
class PeakDetection {
public:
    PeakDetection();
    
    double backgroundMean, backgroundStdDeviation;

    ///As readings are taken from the ADC port, the function add_data_point should be called with an argument of the reading. This function will return an int to indicate what has been detected (1 for calibration, 2 for background reading, 3 for peak).
    string add_data_point(double reading);
    
    ///After a peak has been detected and the signal returned to normal levels, this function should be called to get the max value.
    double get_max_reading();
    timeval get_max_reading_time();
    
    ///If calibration readings have already been taken they can be loaded into the class using the load_calibration_data public function.
    void load_calibration_data(double loadedMean, double loadedStdDeviation);
    
    void hysteresis(bool peakDetected);
    
private:
    double peakMean, reading1, reading2, nStdDeviations;
    timeval peakDetectionTime;
    int readingLag, nReadings;
    bool peakBeingDetected;
    
    void update_mean_stdDeviation(double dataPoint);
    void reset_max_values();
};

#endif /* peakdetection_hpp */
