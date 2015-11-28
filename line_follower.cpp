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
	
    //TODO: may have to change to 0xFF for testing
    int sensor_output = micro_interface->read(0x0F);
    cout << "Sensor output: " << sensor_output << endl;
    
    int ir_sensor_output = sensor_output bitand 0x07;
    cout << "IR sensor output: " << ir_sensor_output << endl;
        
    int junction_status = sensor_output bitand 0x08;
    cout << "Junction status: " << junction_status << endl;
    
    //Only interested in bits 0-2
    if(negative_ramp){
		ir_sensor_output = ir_sensor_output xor 0x07;
		junction_status = junction_status xor 0x08;
		cout << "Negative ramp inversion: " << ir_sensor_output << endl;
	}    
    
    //Note bit is high when black detected, low when white detected
    switch (ir_sensor_output) {
        case 0x06:
            //slightly too far left
            current_status = 0x06;
            proportional_error = -1;
            break;
        case 0x05:
            // we're fucked... must be the negative ramp, bastards
            current_status = 0x05;
            proportional_error = 0;
            negative_ramp = !negative_ramp;
            break;
        case 0x04:
            // too far left
            current_status = 0x04;
            proportional_error = -2;
            break;
        case 0x03:
            // slightly too far right
            current_status = 0x03;
            proportional_error = 1;
            break;
        case 0x02:
            // perfect
            current_status = 0x02;
            proportional_error = 0;
            break;
        case 0x01:
            // too far right
            current_status = 0x01;
            proportional_error = 2;
            break;
        case 0x00:
            // lost line
            current_status = 0x00;
            proportional_error *= 3;
            break;
        case 0x07:
			// Junction detected
			current_status = 0x07;
			proportional_error = 0;
            break;
        default:
            break;
    }
    
    if(junction_status == 0x08){
        current_status = 0x08;
    }
    
    cout << "proportion error: " << proportional_error << endl;
    cout << "current status: " << current_status << endl;
}


int LineFollower::follow_line(double distance, bool toJunction){

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
	double distance_to_obstacle;
	
	//TODO: REMOVE J
	while(distance_moved < 1.25 * distance){
		
		//TODO: Add in emergency stop if distance detector indicates obstacle?
		get_path_status();
		distance_to_obstacle = analogue_interface->get_distance();
		
		cout << "Current status: " << current_status << endl;
		cout << "Current distance moved: " << distance_moved << endl;
		
		//TODO: Add this into if ||  distance_to_obstacle <= 10
		if(distance_moved > 0.6 * distance && current_status == 0x08){
			cout<< "Exited while loop, status: " << current_status << " .Distance: " << distance_to_obstacle << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			
			//Made it to junction!
			if(toJunction){
				return true;
			}
			//If wanting to go set distance something has gone wrong
			else{
				return false;
			}	
		}
        
		right_wheel_speed = static_cast<int>(left_wheel_speed + proportional_error * proportional_gain + integral_error * integral_gain);
        
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
	
	//Hasn't found junction
	if(toJunction){
		return false;
	}
	//Got to distance without finding junction
	else{
		return true;
	}
}

//True for right, false for left
void LineFollower::turn_degrees(double degrees){

	double delta_angle = 0;
	double angle_moved_magnitude = 0;
	double speed = 60;
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	double time_in_s = 0;
	
	cout << "Turning " << degrees << " degrees. Current status: " << current_status << endl;
	
	//Turning left
	if(degrees < 0){
		motors_interface->set_drive_motor_speed(-speed, speed);
	}
	//Turning right
	else{
		motors_interface->set_drive_motor_speed(speed, -speed);
	}
	
	while(angle_moved_magnitude < degrees){
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		delta_angle = ((time_in_s * speed * motors_interface->MAX_SPEED * 180) / (127 * 8.2 * M_PI));
		angle_moved_magnitude += delta_angle;
	}
	cout << "Turned 45 degrees"<< endl;
}

//Positive degrees indicates a right turn
int LineFollower::turn(double angle_in_degrees, int speed){
	
	//Starting turn
	
	int angle_sign = 1;
	if(angle_in_degrees < 0){
		angle_sign = -1;
	}
	
	double angle_magnitude = abs(angle_in_degrees);
	double delta_angle = 0;
	double angle_moved_magnitude = 0;
	int current_speed = abs(speed);
	
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
		cout << "Current angle: " << angle_moved_magnitude << endl;
		
		if(angle_moved_magnitude > 0.6 * angle_magnitude && (current_status == 0x05 ||current_status == 0x07)){
			cout << "Found the line!" << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
		}
		
		//Turning left
		if(angle_sign == -1){			
			if(current_status == 0x01){
				current_speed = current_speed * 2 / 3;
				cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it. Current speed = " << current_speed << endl;	
				motors_interface->set_drive_motor_speed(-current_speed, current_speed);
			}
			else if(current_status == 0x03){
				current_speed = current_speed / 2;
				cout << "Almost there, slowing turning rate. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(-current_speed, current_speed);
			}
			else if(current_status == 0x06){
				current_speed = current_speed / 2;
				cout << "Overshot slightly, turning back with speed 20" << endl;
				motors_interface->set_drive_motor_speed(20, -20);
			}
			else if(current_status == 0x04){
				current_speed = current_speed / 2;
				cout << "Overshot a bit more, turning back with speed 30" << current_speed << endl;
				motors_interface->set_drive_motor_speed(30, -30);
			}
		}
		//Turning right
		else{
			if(current_status == 0x04){
				current_speed = current_speed * 2 / 3;
				cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(current_speed, -current_speed);
			}
			else if(current_status == 0x06){
				current_speed = current_speed / 2;
				cout << "Almost there, slowing turning rate. Current speed = " << current_speed << endl;
				motors_interface->set_drive_motor_speed(current_speed, -current_speed);
			}
			else if(current_status == 0x03){
				current_speed = current_speed / 2;
				cout << "Overshot slightly, turning back with speed 20" << endl;
				motors_interface->set_drive_motor_speed(-20, 20);
			}
			else if(current_status == 0x01){
				current_speed = current_speed / 2;
				cout << "Overshot a bit more, turning back with speed 30" << current_speed << endl;
				motors_interface->set_drive_motor_speed(-30, 30);
			}
		}
		
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		delta_angle = ((time_in_s * current_speed * motors_interface->MAX_SPEED * 180) / (127 * 8.2 * M_PI));
		angle_moved_magnitude += delta_angle;
	}
	
	cout << "Turned too far. Turning back at slower rate" << endl;
	double new_angle_in_degrees = angle_in_degrees * -0.25;
	
	//Don't want to go into infinite loop
	if(abs(new_angle_in_degrees) < 20){
		return false;
	}
	
	return turn(new_angle_in_degrees, speed/2);
}
