//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 17/11/15.
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
    current_status = 0x02;
    negative_ramp = false;
    junction_detected = false;
    
    proportional_gain = 20;
    left_wheel_speed = 100;
    
    motors_interface = motorsPtr;
    micro_interface = microPtr;
    analogue_interface = anaPtr;
}

int LineFollower::reverse_after_pickup(){
	get_path_status();
	
	
	motors_interface->set_drive_motor_speed( -128, -128);
	
	while(current_status != 7){
		get_path_status();
	}
	
	motors_interface->set_drive_motor_speed(0, 0);
	return true;
	
}

int LineFollower::reverse_after_pickup(double distance){
	
	get_path_status();
	double distance_moved = 0;
	double time_in_s = 0;
	double delta_distance = 0;
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	
	motors_interface->set_drive_motor_speed( -100, -100);
	
	while(distance_moved < distance){
		get_path_status();
		gettimeofday(&lastReadingTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		
		cout << "distance moved: " << distance_moved << "in time:" << time_in_s<< endl;
		
		delta_distance = (time_in_s * left_wheel_speed * motors_interface->MAX_SPEED) / 127;
		distance_moved += delta_distance;
	
		lastReadingTime = currentTime;
		
	}
	
	motors_interface->set_drive_motor_speed(0, 0);
	return true;
	
}

void LineFollower::get_path_status(){

    int sensor_output = micro_interface->read();
    int ir_sensor_output = sensor_output bitand 0x07;
    int junction_status = sensor_output bitand 0x08;
    
    //Only interested in bits 0-2
    if(negative_ramp){
		ir_sensor_output = ir_sensor_output xor 0x07;
		//junction_status = junction_status xor 0x08;
		cout << "Found negative ramp! Sensor output: " << ir_sensor_output << endl;
	}    
    
    //Note bit is high when white detected, low when black detected
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
            
            if(left_wheel_speed > 90){
				left_wheel_speed = 70;
			} else {
				left_wheel_speed = 100;
			}
            
            //change ramp time
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
            break;
        case 0x07:
			// Junction detected
			current_status = 0x07;
			proportional_error = 0;
            break;
        default:
            break;
    }
    
    
    if(junction_status == 0x08 && !negative_ramp){
		cout << "Junction detected!. Negative ramp: " << negative_ramp << endl;
        junction_detected = true;
    }
    else{
		junction_detected = false;
	}
}

int LineFollower::follow_line(double distance, bool toJunction){
	return follow_line(distance, toJunction, 0, false);
}

int LineFollower::follow_line(double distance, bool toJunction, double stop_obstacle_distance){
	return follow_line(distance, toJunction, stop_obstacle_distance, false);
}

int LineFollower::follow_line(double distance, bool toJunction, double stop_after_distance, bool junctionDistance){
	
	distance_moved = 0;
	proportional_error = 0;
	integral_error = 0;
	current_status = 0x02;
	
	cout << "Following line for distance: " << distance << ". Current status: " << current_status <<  ". Stop after distance: " << stop_after_distance << endl;
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	
	double currentMeanSpeed = 0;
	double time_in_s = 0;
	double delta_distance = 0;
	double distance_after_junction = 0;
	
	while(distance_moved < 1.25 * distance){
		get_path_status();
		
		if(current_status == 0x07 && junctionDistance){
			cout << "Front junction detected" << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
		}
			
		cout << "Current status: " << current_status << endl;
		cout << "Current distance moved: " << distance_moved << endl;
		cout << "Current distance from junction: " << distance_after_junction << endl;
		
		if(distance_moved > distance && !toJunction){
			cout << "Got to required distance "<< endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
		}
		
		if(distance_moved > 10 && junction_detected){
			cout<< "Exited while loop, status: " << current_status << ". Distance moved: " << distance_moved << endl;
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
		
		cout << "proportional error = " << proportional_error << " and integral error = " << integral_error << endl;
		cout << "left speed = " << left_wheel_speed << " and right speed = " << right_wheel_speed << endl << endl;
		
		currentMeanSpeed = (left_wheel_speed + right_wheel_speed)/2;
		
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		cout << "Time in s: " << time_in_s << endl;
		
		delta_distance = (time_in_s * currentMeanSpeed * motors_interface->MAX_SPEED) / 127;
		distance_moved += delta_distance;
		
		integral_error += time_in_s * proportional_error;
	}
	
	//Hasn't found junction
	if(toJunction){
		motors_interface->set_drive_motor_speed(0, 0);
		return false;
	}
	//Got to distance without finding junction
	else{
		motors_interface->set_drive_motor_speed(0, 0);
		return true;
	}
}

//Positive for right, negative for left
void LineFollower::turn_degrees(double degrees){
	
	cout << "Turning " << degrees << " degrees." << endl;

	double abs_degrees = abs(degrees);
	int speed = 65;
	
	int delayTime = static_cast<int>(1000 * abs_degrees / ((speed * motors_interface->MAX_SPEED * 180) / (127 * 8.2 * M_PI)));
    
	
	//Turning left
	if(degrees < 0.0){
		cout << "Setting wheel speeds to turn left" << endl;
		motors_interface->set_drive_motor_speed(-speed, speed);
	}
	//Turning right
	else{
		cout << "Setting wheel speeds to turn right" << endl;
		motors_interface->set_drive_motor_speed(speed, -speed);
	}
	cout << "Delaying for " << delayTime << " seconds" << endl;
	
	delay(delayTime);
	motors_interface->set_drive_motor_speed(0, 0);
	cout << "Finished turning" << endl;
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
    double angle_moved = 0;
	double angle_moved_magnitude = 0;
	int current_speed = abs(speed);
	
	timeval lastReadingTime;
	timeval currentTime;
	gettimeofday(&lastReadingTime, NULL);
	double time_in_s = 0;
	
	cout << "Turning " << angle_in_degrees  << " degrees, with speed: " << current_speed << "Current status: " << current_status << endl;
	
	//Turning left
	if(angle_sign == -1){
		motors_interface->set_drive_motor_speed(-current_speed, current_speed);
	}
	//Turning right
	else{
		motors_interface->set_drive_motor_speed(current_speed, -current_speed);
	}
	
	//TODO: Change this value so that it doesn't sense the line it's leaving.
	cout << "Delaying to allow robot to leave line" << endl;
	delay(200);
	cout << "Can now detect path" << endl;
	
	while(angle_moved_magnitude < 1.25 * angle_magnitude){
		
		get_path_status();
		
		cout << endl << "Current status: " << current_status << endl;
		cout << "Current angle: " << angle_moved_magnitude << endl;
		
		if(angle_moved_magnitude > 0.6 * angle_magnitude){
			
			if(current_status == 0x02 || current_status == 0x07){
			cout << "Found the line!" << endl << endl;
			motors_interface->set_drive_motor_speed(0, 0);
			return true;
			}
		
			//Turning left
			if(angle_sign == -1){			
				if(current_status == 0x01){
					cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it." << endl;
					current_speed = 30;
					motors_interface->set_drive_motor_speed(-current_speed, current_speed);
				}
				else if(current_status == 0x03){
					cout << "Almost there, slowing turning rate" << endl;
					current_speed = 20;
					motors_interface->set_drive_motor_speed(-current_speed, current_speed);
					}
				else if(current_status == 0x06){
					cout << "Overshot slightly, turning back with speed 20" << endl;
					current_speed = 10;
					motors_interface->set_drive_motor_speed(current_speed, -current_speed);
					angle_sign *= -1;
					}
				else if(current_status == 0x04){
					cout << "Overshot a bit more, turning back with speed 30" << endl;
					current_speed = 15;
					motors_interface->set_drive_motor_speed(current_speed, -current_speed);
					angle_sign *= -1;
					}
			}
			//Turning right
			else{
				if(current_status == 0x04){
					cout << "Just spotted line, slow down turning rate so we don't overshoot and miss it." << endl;
					current_speed = 30;
					motors_interface->set_drive_motor_speed(current_speed, -current_speed);
				}
				else if(current_status == 0x06){
					cout << "Almost there, slowing turning rate." << endl;
					current_speed = 20;
					motors_interface->set_drive_motor_speed(current_speed, -current_speed);
				}
				else if(current_status == 0x03){
					cout << "Overshot slightly, turning back with speed 20" << endl;
					current_speed = 10;
					motors_interface->set_drive_motor_speed(-current_speed, current_speed);
					angle_sign *= -1;
				}
				else if(current_status == 0x01){
					cout << "Overshot a bit more, turning back with speed 30" << current_speed << endl;
					current_speed = 15;
					motors_interface->set_drive_motor_speed(-current_speed, current_speed);
					angle_sign *= -1;
				}
			}
		}
		
		gettimeofday(&currentTime, NULL);
		time_in_s = diff_ms(currentTime, lastReadingTime)/1000;
		lastReadingTime = currentTime;
		
		delta_angle = ((time_in_s * current_speed * motors_interface->MAX_SPEED * 180) / (127 * 8.2 * M_PI));
        
        angle_moved += delta_angle * angle_sign;
		angle_moved_magnitude = abs(angle_moved);
		cout << "Angle moved: " << angle_moved << endl;
		cout << "Angle moved magnitude: " << angle_moved_magnitude << endl;
	}
	
	cout << "Turned too far. Turning back at slower rate" << endl;
	double new_angle_in_degrees = angle_in_degrees * -0.25;
	
	//Don't want to go into infinite loop
	if(abs(new_angle_in_degrees) < 20){
		cout << "didn't find the line lol" << endl << endl;
		return false;
	}
	
	return turn(new_angle_in_degrees, speed/2);
}
