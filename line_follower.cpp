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

Idp *idp;

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


void LineFollower::stay_in_line( int current_status ){
	
	switch (current_status) {
        case 0x01:
            //slightly too far left
            cout << "slightly too far left" << endl;
            motors_mode(3);
            break;
        case 0x02:
            // we're fucked... must be the negative ramp, bastards
            cout << "we're fucked... must be the negative ramp, bastards" << endl;
            motors_mode(5);
            break;
        case 0x03:
            // too far left
            cout << "too far left" << endl;
            motors_mode(4);
            break;
        case 0x04:
            // slightly too far right
            cout << "slightly too far right" << endl;
            motors_mode(1);
            break;
        case 0x05:
            // perfect
            cout << "perfect" << endl;
            motors_mode(0);
            break;
        case 0x06:
            // too far right
            cout << "too far right" << endl;
            motors_mode(2);
            break;
        case 0x07:
            // lost line
            cout << "lost line" << endl;
            motors_mode(5);
            break;
        default:
            break;
    }
	
	
}

void LineFollower::motors_mode(int mode){
	
	switch(mode){
		case 0: 
		// continue going straight
			idp->rlink.command (BOTH_MOTORS_GO_SAME, idp->speed);
			break;
		case 1: 
		// compensate right slightly
			idp->rlink.command (MOTOR_1_GO, idp->speed - 20);
			idp->rlink.command (MOTOR_2_GO, idp->speed + 20);
			break;
		case 2: 
		// compensate right strongly
			idp->rlink.command (MOTOR_1_GO, idp->speed - 40);
			idp->rlink.command (MOTOR_2_GO, idp->speed + 40);
			break;
		case 3: 
		// compensate left slightly
			idp->rlink.command (MOTOR_1_GO, idp->speed + 20);
			idp->rlink.command (MOTOR_2_GO, idp->speed - 20);
			break;
		case 4: 
		// compensate left strongly
			idp->rlink.command (MOTOR_1_GO, idp->speed + 40);
			idp->rlink.command (MOTOR_2_GO, idp->speed - 40);
			break;
		case 5: 
		// go backwards 
			idp->rlink.command (BOTH_MOTORS_GO_OPPOSITE, idp->speed);
			break;
		default:
			break;
	}
	

}
