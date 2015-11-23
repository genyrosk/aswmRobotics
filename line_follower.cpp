//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "line_follower.hpp"
#include <iostream>
#include <sstream>
using namespace std;

LineFollower::LineFollower(){
	int current_status = 0x04;
    //time_t time_on_line = 0;
    //int speed_right = 0;
    //int speed_left = 0;
}
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
            break;
        case 0x02:
            // we're fucked... must be the negative ramp, bastards
            current_status = 0x02;
            break;
        case 0x03:
            // too far left
            current_status = 0x03;
            break;
        case 0x04:
            // slightly too far right
            current_status = 0x04;
            break;
        case 0x05:
            // perfect
            current_status = 0x05;
            break;
        case 0x06:
            // too far right
            current_status = 0x06;
            break;
        case 0x07:
            // lost line
            current_status = 0x07;
            break;
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

void stay_in_line(){
	
}
