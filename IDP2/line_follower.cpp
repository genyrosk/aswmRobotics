//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "line_follower.hpp"

#include <iostream>
#include <sstream>
#include <time.h>
using namespace std;

LineFollower::LineFollower(){
    current_status = 0x02;
    left_wheel_speed = 100;
    time_on_line = 0;
    
    //TODO: Calibrate these gain values
    proportional_gain = 5;
    integral_gain = 5;
    negative_ramp = false;
}

LineFollower::LineFollower(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr){
    LineFollower();
    
    motors_interface = motorsPtr;
    micro_interface = microPtr;
    analogue_interface = anaPtr;
}
// ------
// ------

int LineFollower::follow_line(double distance){
	distance_moved = 0;
	proportional_error = 0;
	integral_error = 0;
	
	cout << current_status << endl;
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	
	double currentMeanSpeed = 0;
	int time_in_ms = 0;
	
	while(distance_moved < 1.25 * distance){
		get_path_status();
		if(current_status == 0x08){
			return true;
		}
        
		right_wheel_speed = static_cast<int>(100 + proportional_error * proportional_gain + integral_error * integral_gain);
        
		motors_interface->set_drive_motor_speed(left_wheel_speed, right_wheel_speed);
		
		cout << "proportional error = " << proportional_error << " and proportional gain = " << proportional_gain << endl;
		cout << "left speed = " << left_wheel_speed << " and right speed = " << right_wheel_speed << endl;
		
		currentMeanSpeed = (left_wheel_speed + right_wheel_speed)/2;
		
		gettimeofday(&currentTime, NULL);
		time_in_ms = diff_ms(currentTime, lastReadingTime);
		
		distance_moved += (time_in_ms * currentMeanSpeed ) / (1000 * motors_interface->MAX_SPEED);
		integral_error += (time_in_ms/1000) * proportional_error;
	}
	return false;
}

void LineFollower::get_path_status(){
    
    int sensor_output = micro_interface->read(0x0F);
    
    //Only interested in bits 0-2
    int ir_sensor_output = sensor_output bitand 0x07;
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x01:
            //slightly too far left
            current_status = 0x01;
            proportional_error = -1;
            break;
        case 0x02:
            // we're fucked... must be the negative ramp, bastards
            current_status = 0x02;
            proportional_error = 0;
            negative_ramp = true;
            break;
        case 0x03:
            // too far left
            current_status = 0x03;
            proportional_error = -2;
            break;
        case 0x04:
            // slightly too far right
            current_status = 0x04;
            proportional_error = 1;
            break;
        case 0x05:
            // perfect
            current_status = 0x05;
            proportional_error = 0;
            negative_ramp = false;
            break;
        case 0x06:
            // too far right
            current_status = 0x06;
            proportional_error = 2;
            break;
        case 0x07:
            // lost line
            current_status = 0x07;
            proportional_error *= 3;
            break;
        default:
            break;
    }
    
    int junction_status = sensor_output bitand 0x08;
    
    if(junction_status == 0x08){
        current_status = 0x08;
    }
}



/*
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
			motors_interface->idp->rlink.command (BOTH_MOTORS_GO_SAME, default_speed);
			break;
		case 1: 
		// compensate right slightly
			motors_interface->idp->rlink.command (MOTOR_1_GO, default_speed - 20);
			motors_interface->idp->rlink.command (MOTOR_2_GO, default_speed + 20);
			break;
		case 2: 
		// compensate right strongly
			motors_interface->idp->rlink.command (MOTOR_1_GO, default_speed - 40);
			motors_interface->idp->rlink.command (MOTOR_2_GO, default_speed + 40);
			break;
		case 3: 
		// compensate left slightly
			motors_interface->idp->rlink.command (MOTOR_1_GO, default_speed + 20);
			motors_interface->idp->rlink.command (MOTOR_2_GO, default_speed - 20);
			break;
		case 4: 
		// compensate left strongly
			motors_interface->idp->rlink.command (MOTOR_1_GO, default_speed + 40);
			motors_interface->idp->rlink.command (MOTOR_2_GO, default_speed - 40);
			break;
		case 5: 
		// go backwards 
			motors_interface->idp->rlink.command (BOTH_MOTORS_GO_OPPOSITE, default_speed);
			break;
		default:
			break;
	}
	

}
*/
