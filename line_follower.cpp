//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "line_follower.hpp"
#include <iostream>
#include <array>
using namespace std;

// ------
// ------
void LineFollower::get_path_status(int sensor_output){
    
    //Only interested in bits 0-2
    int ir_sensor_output = sensor_output bitand 0x07;
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x01:
            //slightly too far left
            current_status = 0x01;
        case 0x02:
            // we're fucked...
            current_status = 0x02;
        case 0x03:
            // too far left
            current_status = 0x03;
        case 0x04:
            // slightly too far right
            current_status = 0x04;
        case 0x05:
            // perfect
            current_status = 0x05;
        case 0x06:
            // too far right
            current_status = 0x06;
        case 0x07:
            // lost line
            current_status = 0x07;
        default:
            break;
    }
}

bool LineFollower::on_junction(int sensor_output){
    
    //Only interested in bit 3
    int junction_status = sensor_output bitand 0x08;
    if(junction_status == 0x08){
        return false;
    }
    else{
        return true;
    }
}
