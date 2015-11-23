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

//enum add_data_point_result { CALIBRATING, BACKGROUNDREADING, PEAKREADING } ;
//string add_data_point_result[3] = {"CALIBRATING", "BACKGROUNDREADING", "PEAKREADING"};
//string read_status;

//read_status = add_data_point_result[1];

// declare PeakDetection class and its properties + methods
class PeakDetection {
public:
    PeakDetection();
    //TODO: Implement destructor which saves mean and std. deviation to robotsettings
    //~PeakDetection();
	
	string add_data_point_result[3];
	string read_status;


    ///As readings are taken from the ADC port, the function add_data_point should be called with an argument of the reading. This function will return an int to indicate what has been detected (1 for calibration, 2 for background reading, 3 for peak).
    string add_data_point(double reading);
    
    ///After a peak has been detected and the signal returned to normal levels, this function should be called to get the max value.
    double get_max_reading();
    
    ///If calibration readings have already been taken they can be loaded into the class using the load_calibration_data public function.
    void load_calibration_data(double loadedMean, double loadedStdDeviation);
    
private:
    double mean, stdDeviation, maxMean, reading1, reading2, nStdDeviations, peakDetectDelay;
    int readingLag, nReadings;
    time_t lastPeakDetected;
    
    void update_mean_stdDeviation(double dataPoint);
    void reset_max_values();
};

#endif /* peakdetection_hpp */
