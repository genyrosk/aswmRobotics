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
string LineFollower::get_path_status(int ir_sensor_output){
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x01:
            return "slightly too far left";
            
        case 0x02:
            return "we're fucked..." ;
            
        case 0x03:
            return "too far left" ;
            
        case 0x04:
            return "slightly too far right";

        case 0x05:
            return "perfect";

        case 0x06:
            return "too far right";

        case 0x07:
            return "lost line";
        
        default:
            return "invalid";
    }
    
}
