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
//TODO: Change the function of this method to do something more useful, remove return string statements
string LineFollower::get_path_status(int sensor_output){
    
    //Only interested in bits 0-2
    int ir_sensor_output = sensor_output bitand 0x07;
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x01:
            current_status = 0x01;
            return "slightly too far left";
            
        case 0x02:
            current_status = 0x02;
            return "we're fucked..." ;
            
        case 0x03:
            current_status = 0x03;
            return "too far left" ;
            
        case 0x04:
            current_status = 0x04;
            return "slightly too far right";

        case 0x05:
            current_status = 0x05;
            return "perfect";

        case 0x06:
            current_status = 0x06;
            return "too far right";

        case 0x07:
            current_status = 0x07;
            return "lost line";
        
        default:
            break;
    }
    
    //Has a junction been detected?
    int junction_status = sensor_output bitand 0x08;
    if(junction_status == 0x08){
        current_status = 0x08;
        return "junction sensed";
    }
    return "invalid";
}
