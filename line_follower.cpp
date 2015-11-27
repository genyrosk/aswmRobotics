//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "line_follower.hpp"

#include <iostream>
#include <cmath>
#include <sstream>
#include <sys/time.h>
#include "idp.h"
using namespace std;

LineFollower::LineFollower(){
    current_status = 0x02;
    negative_ramp = false;
}

LineFollower::LineFollower(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr){
    LineFollower();
    
    motors_interface = motorsPtr;
    micro_interface = microPtr;
    analogue_interface = anaPtr;
}

//TODO: Probably dont need this. Implement a method which overcomes the length issue of the robot
int LineFollower::reverse_after_pickup(){
	get_path_status();
	motors_interface->set_drive_motor_speed( -128, -128);
	if(current_status == 0x08){
		motors_interface->set_drive_motor_speed(0, 0);
		return true;
	}
	else{
		return false;
	}
}

void LineFollower::get_path_status(){
	
    //TODO: byte should be 0x0F
    int sensor_output = micro_interface->read(0xFF);
    int ir_sensor_output = sensor_output bitand 0x07;
    
    //Only interested in bits 0-2
    if(negative_ramp){
		ir_sensor_output = ir_sensor_output xor 0xFF;
	}    
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x06:
            //slightly too far left
            this->current_status = 0x06;
            this->proportional_error = -1;
            break;
        case 0x05:
            // we're fucked... must be the negative ramp, bastards
            this->current_status = 0x05;
            this->proportional_error = 0;
            this->negative_ramp = !negative_ramp;
            break;
        case 0x04:
            // too far left
            this->current_status = 0x04;
            this->proportional_error = -2;
            break;
        case 0x03:
            // slightly too far right
            this->current_status = 0x03;
            this->proportional_error = 1;
            break;
        case 0x02:
            // perfect
            this->current_status = 0x02;
            this->proportional_error = 0;
            this->negative_ramp = false;
            break;
        case 0x01:
            // too far right
            this->current_status = 0x01;
            this->proportional_error = 2;
            break;
        case 0x00:
            // lost line
            this->current_status = 0x00;
            this->proportional_error *= 3;
            break;
        case 0x07:
			// Junction detected
			this->current_status = 0x07;
			this->proportional_error = 0;
            break;
        default:
            break;
    }
    
    int junction_status = sensor_output bitand 0x08;
    
    if(junction_status == 0x08){
        current_status = 0x08;
    }
    
    cout << "proportion error: " << this->proportional_error << endl;
    cout << "current status: " << this->current_status << endl;
}


int LineFollower::follow_line(double distance){

	//TODO: Remove test code
	//int sensor[] = {0x05,0x05,0x04,0x04,0x05,0x05,0x04,0x04,0x06,0x06,0x05,0x05,0x01,0x01,0x03,0x03,0x01,0x01,0x05,0x06, 0x05,0x05,0x04,0x04,0x05,0x05,0x04,0x04,0x06,0x06,0x05,0x05,0x01,0x01,0x03,0x03,0x01,0x08,0x05,0x06};
	//int j = 0;
	
	distance_moved = 0;
	proportional_error = 0;
	integral_error = 0;
	current_status = 0x02;
	
	cout << "Following line for distance: " << distance << ". Current status: " << current_status << endl;
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	
	double currentMeanSpeed = 0;
	double time_in_s = 0;
	
	//TODO: REMOVE J
	while(distance_moved < 1.25 * distance){
		
		//TODO: Add in emergency stop if distance detector indicates obstacle?
		get_path_status();
		
		cout << "Current status: " << current_status << endl;
		cout << "Current distance moved: " << distance_moved << endl;
		
		//TODO: Add this into if || analogue_interface->get_distance() <= 10
		if(current_status == 0x08){
			cout<< "Exited while loop, status: " << current_status << " .Distance: " << analogue_interface->get_distance() << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
		}
        
		right_wheel_speed = static_cast<int>(100 + proportional_error * proportional_gain + integral_error * integral_gain);
        
        cout << "Right wheel speed, before sign-magnitude conversion: " << right_wheel_speed << endl;
        
        //Converting to sign-magnitude
        if(right_wheel_speed > 127){
			right_wheel_speed = 127;
		}
		else if(right_wheel_speed < -128){
			right_wheel_speed = -128;
		}
		else if(right_wheel_speed < 0){
			right_wheel_speed = 127 - right_wheel_speed;
		}
        
		motors_interface->set_drive_motor_speed(left_wheel_speed, right_wheel_speed);
		
		cout << "proportional error = " << proportional_error << " and proportional gain = " << proportional_gain << endl;
		cout << "integral error = " << integral_error << " and integral gain = " << integral_gain << endl;
		cout << "left speed = " << left_wheel_speed << " and right speed = " << right_wheel_speed << endl << endl;
		
		currentMeanSpeed = (left_wheel_speed + right_wheel_speed)/2;
		
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		distance_moved += (time_in_s * currentMeanSpeed * motors_interface->MAX_SPEED) / 127;
		integral_error += time_in_s * proportional_error;
		//j++;
	}
	return false;
}

//Positive degrees indicates a right turn
int LineFollower::turn(double angle_in_degrees, int speed){
	get_path_status();
	
	int angle_sign = 1;
	if(angle_in_degrees < 0){
		angle_sign = -1;
	}
	
	double angle_magnitude = abs(angle_in_degrees);
	double angle_moved = 0;
	double angle_moved_magnitude = 0;
	int current_speed = speed;
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	double time_in_s = 0;
	
	cout << "Turning " << angle_in_degrees  << " degrees. "<< "Current status: " << current_status << endl;
	
	//Turning left
	if(angle_sign == -1){
		motors_interface->set_drive_motor_speed(-current_speed, current_speed);
	}
	//Turning right
	else{
		motors_interface->set_drive_motor_speed(current_speed, -current_speed);
	}
	
	//TODO: Change this value so that it doesn't sense the line it's leaving.
	delay(200);
	
	while(angle_moved_magnitude < 1.25 * angle_magnitude){
		
		get_path_status();
		
		cout << "Current status: " << current_status << endl;
		cout << "Current angle: " << angle_moved << endl;
		
		if(angle_moved_magnitude > 0.6 * angle_magnitude && current_status == 0x05){
			cout << "Found the line!" << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
		}
		
		//Turning left
		if(angle_sign == -1){			
			if(current_status == 0x06){
				current_speed = current_speed * 2 / 3;
				cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it. Current speed = " << current_speed << endl;	
				motors_interface->set_drive_motor_speed(-current_speed, current_speed);
			}
			else if(current_status == 0x04){
				current_speed = current_speed / 2;
				cout << "Almost there, slowing turning rate. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(-current_speed, current_speed);
			}
		}
		//Turning right
		else{
			if(current_status == 0x03){
				current_speed = current_speed * 2 / 3;
				cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(current_speed, -current_speed);
			}
			else if(current_status == 0x01){
				current_speed = current_speed / 2;
				cout << "Almost there, slowing turning rate. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(current_speed, -current_speed);
			}
		}
		
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		double angle = (time_in_s * current_speed * motors_interface->MAX_SPEED  * 8.2 / 127 );
		
		angle_moved += angle * angle_sign;
		angle_moved_magnitude = abs(angle_moved);	
	}
	
	cout << "Turned too far. Turning back at slower rate" << endl;
	angle_in_degrees = angle_in_degrees * -0.25;
	
	//Don't want to go into infinite loop
	if(abs(angle_in_degrees) < 20){
		return false;
	}
	return turn(angle_in_degrees, speed/2);
}

/*
void LineFollower::get_path_status(int testHexCode){
    
    //int sensor_output = micro_interface->read(0x0F);
    
    //Only interested in bits 0-2
    int ir_sensor_output = testHexCode bitand 0x07;
    
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
    
    int junction_status = testHexCode bitand 0x08;
    
    if(junction_status == 0x08){
        current_status = 0x08;
    }
    
    cout << "get_path_status, proportion error: " << proportional_error << endl;
}
*/

