//
//  peakdetection.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd  on 17/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "peakdetection.hpp"
#include  <cmath>
#include  <sys/time.h>
#include  <stdio.h>
using namespace std;

// declare class instances here (needed in order to access their methods and properties

PeakDetection::PeakDetection(){
	   
    readingLag = 15;
    nStdDeviations = 3.0;
    nReadings = 0;
    
    backgroundMean = 0.0;
    backgroundStdDeviation = 0.0;
    peakMean = 0.0;
    reading1 = 0.0;
    reading2 = 0.0;
    
    peakBeingDetected = false;
}

void PeakDetection::load_calibration_data(double loadedMean, double loadedStdDeviation){
    backgroundMean = loadedMean;
    backgroundStdDeviation = loadedStdDeviation;
    nReadings = readingLag;
}

void PeakDetection::reset_max_values(){
    peakMean = 0;
}

string PeakDetection::add_data_point(double dataPoint){
    cout << "Adding data point to PeakDetectionClass = " << dataPoint << endl;
    
    double currentMean = (dataPoint + reading1 + reading2)/3;
    reading2 = reading1;
    reading1 = dataPoint;
    
    //First few readings must be taken before we can calculate if subsequent readings vary significantly
    if(nReadings < readingLag){
		cout << "Calibration in progress. " << nReadings << " readings taken" << endl; 
		update_mean_stdDeviation(dataPoint);
        return "CALIBRATING";
	}
	//Enough readings have now been taken to distinguish peaks from background readings
	else{
		if(nReadings == readingLag){
			cout << "Calibration complete" << endl;
		}
		
		//Process for dealing with background readings
		if(!(abs(dataPoint - backgroundMean) > nStdDeviations * backgroundStdDeviation)){
			update_mean_stdDeviation(dataPoint);
            hysteresis(false);
            return "BACKGROUNDREADING";
		}
		
		//This is invoked when a peak has been detected. Note we dont want mean or stdDeviation to be altered from now on
		else{
            cout << "Peak detected! Value = " << dataPoint << endl;
            hysteresis(true);
            if(currentMean > peakMean){
                peakMean = currentMean;
                gettimeofday(&peakDetectionTime, NULL);
            }
            return "PEAKREADING";
		}
	}
}

double PeakDetection::get_max_reading(){
    return peakMean;
}

timeval PeakDetection::get_max_reading_time(){
    return peakDetectionTime;
}

void PeakDetection::update_mean_stdDeviation(double dataPoint){
    
	//Updating estimate of mean, weighting the value based compared to previous readings (note this is an unbiased estimator)
    backgroundMean = (backgroundMean * nReadings + dataPoint)/(nReadings+1);
    
    //Updating the estimate of the standard deviation
    if(nReadings == 0){
        backgroundStdDeviation = 0;
    }
    else{
        double Ex2 = (((nReadings-1) * backgroundStdDeviation * backgroundStdDeviation + nReadings * backgroundMean * backgroundMean + dataPoint * dataPoint))/nReadings;
        double variance = fabs(Ex2 - (backgroundMean * backgroundMean * (nReadings + 1)/nReadings));
        backgroundStdDeviation = sqrt(variance);
    }
    nReadings++;
}

void PeakDetection::hysteresis(bool peakDetected){
	cout << "Threshold before = " << backgroundMean + nStdDeviations * backgroundStdDeviation << endl;
    if(!peakBeingDetected && peakDetected){
        nStdDeviations *= 2;
        peakBeingDetected = true;
    }
    else if(peakBeingDetected && !peakDetected){
        nStdDeviations /= 2;
        peakBeingDetected = false;
    }
    cout << "Threshold after = " << backgroundMean + nStdDeviations * backgroundStdDeviation << endl;
}
// modify PeakDetection class methods
