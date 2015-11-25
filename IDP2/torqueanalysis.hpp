
//
//  torqueanalysis.hpp
//  IDP
//
//  Created by Peter Boothroyd on 19/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef torqueanalysis_hpp
#define torqueanalysis_hpp

#include <stdio.h>

class TorqueAnalysis{
public:
    TorqueAnalysis();
    
    void add_data_point(double dataPoint);
    int nWoodCracker();
    int location_unique_weight(bool findMin);

private:

    double mean, maxMean, minMean, nMaxReading, nMinReading, reading1, reading2, threshold1, threshold2, threshold3;
    int nReadings, nWoodCrackers;
};

#endif /* torqueanalysis_hpp */
