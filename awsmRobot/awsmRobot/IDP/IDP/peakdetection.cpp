//
//  peakdetection.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd  on 17/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "peakdetection.hpp"
#include  <cmath>
using namespace std;

// declare class instances here (needed in order to access their methods and properties

PeakDetection::PeakDetection(){
	   
    readingLag = 10;
    nStdDeviations = 3.0;
    nReadings = 0;
    peakDetectDelay = 3.0;
    time(&lastPeakDetected);
    
    mean = 0.0;
    stdDeviation = 0.0;
    max = 0.0;
    maxMean = 0.0;
    reading1 = 0.0;
    reading2 = 0.0;
}

int PeakDetection::add_data_point(double dataPoint){
    cout << "Adding data point to PeakDetectionClass = " << dataPoint << endl;
    
    reading2 = reading1;
    reading1 = dataPoint;
    
    //First few readings must be taken before we can calculate if subsequent readings vary significantly
    if(nReadings < readingLag){
		cout << "Calibration in progress. " << nReadings << " readings taken" << endl; 
		update_mean_stdDeviation(dataPoint);
		//Status returned when setting up
		return 1;
	}
	//Enough readings have now been taken to distinguish peaks from background readings
	else{
		if(nReadings == 10){
			cout << "Calibration complete" << endl;
		}
		
		//Process for dealing with background readings
		if(!(abs(dataPoint - mean) > nStdDeviations * stdDeviation)){
			update_mean_stdDeviation(dataPoint);
			//Return 2 indicates that a background reading has been taken
			return 2;
		}
		
		//This is invoked when a peak has been detected. Note we dont want mean or stdDeviation to be altered from now on
		else{
			//Checking if the last peak was detected a minimum time ago
			if(difftime(lastPeakDetected, time(NULL)) > peakDetectDelay){
				cout << "Peak detected! Value = " << dataPoint << endl;
				if(dataPoint > max){
					max = dataPoint;
					maxMean = (max + reading1 + reading2)/3;
				}
				//Return 3 to indicate that a peak has been detected
				return 3;
			}
			//If not, noise must've pushed the reading back over the threshold
			else{
				return 2;
			}
		}
	}
}

double PeakDetection::get_max_reading(){
	return maxMean;
}

void PeakDetection::update_mean_stdDeviation(double dataPoint){
	//Updating estimate of mean, weighting the value based compared to previous readings (note this is an unbiased estimator)
    mean = (mean * nReadings + dataPoint)/(nReadings+1);
    //Updating the estimate of the standard deviation
    stdDeviation = sqrt((((nReadings-1) * stdDeviation * stdDeviation + nReadings * mean * mean + dataPoint * dataPoint) - mean * mean * (nReadings + 1))/nReadings);
		nReadings++;
}
// modify PeakDetection class methods
