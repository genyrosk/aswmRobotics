//
//  torqueanalysis.cpp
//  IDP
//
//  Created by Peter Boothroyd on 19/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include "torqueanalysis.hpp"

TorqueAnalysis::TorqueAnalysis(){
    mean = 0;
    maxMean = 0;
    minMean = 0;
    nMaxReading = 0;
    nMinReading = 0;
    nReadings = 0;
    reading1 = 0;
    reading2 = 0;
    
    //TODO: Experiment to find these  values
    threshold1 = 2;
    threshold2 = 3;
    threshold3 = 4;
}
void TorqueAnalysis::add_data_point(double dataPoint){
    //Updating estimate of mean, weighting the value based compared to previous readings (note this is an unbiased estimator)
    mean = (mean * nReadings + dataPoint)/(nReadings+1);
    
    double tempMean = (dataPoint + reading1 + reading2)/3;
    if(tempMean > maxMean){
        maxMean = tempMean;
        nMaxReading = nReadings;
    }
    if(tempMean < minMean){
        minMean = tempMean;
        nMinReading = nReadings;
    }
    
    nReadings++;
    reading2 = reading1;
    reading1 = dataPoint;
}

int TorqueAnalysis::nWoodCracker(){
    if(mean > threshold3){
        nWoodCrackers = 3;
        return 3;
    }
    if(mean > threshold2){
        nWoodCrackers = 2;
        return 2;
    }
    if(mean > threshold1){
        nWoodCrackers = 1;
        return 1;
    }
    nWoodCrackers = 0;
    return 0;
}

int TorqueAnalysis::location_unique_weight(bool findMin){
    if(findMin){
        double proportionAroundRotation = nMinReading/nReadings;
        if(proportionAroundRotation>=0.0833 && proportionAroundRotation <=0.4167 ){
            return 1;
        }
        if(proportionAroundRotation>=0.4167 && proportionAroundRotation <=0.75 ){
            return 0;
        }
        else{
            return 2;
        }
    }
    else{
        double proportionAroundRotation = nMaxReading/nReadings;
        if(proportionAroundRotation>=0.0833 && proportionAroundRotation <=0.4167 ){
            return 1;
        }
        if(proportionAroundRotation>=0.4167 && proportionAroundRotation <=0.75 ){
            return 0;
        }
        else{
            return 2;
        }
    }
}