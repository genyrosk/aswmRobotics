//
//  RobotSettings.hpp
//  IDP
//
//  Created by Peter Boothroyd on 19/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef RobotSettings_hpp
#define RobotSettings_hpp

#include <stdio.h>
#include <time.h>

//A class for storing the critical settings from the robot, in the case that something goes wrong and the robot has to be restarted. In this case a file should be parsed and the RobotSettings class populated from the saved data.
class RobotSettings{
public:
    RobotSettings();
    ~RobotSettings();
    
    long start_time;
    int n_crackers_delivered;
    bool returned_start_box_bonus;
    double ldr_mean;
    double ldr_std_deviation;
    
    int save();
    int load();
};

#endif /* RobotSettings_hpp */
