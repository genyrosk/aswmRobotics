//
//  pickup.cpp
//  IDP
//
//  Created by Peter Boothroyd on 23/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include <sys/time.h>
#include <time.h>
#include <cmath>
#include "pickup.hpp"
#include "mechanical.hpp"
#include "idp.h"
#include "identifier.hpp"
#include "cracker.hpp"


Pickup::Pickup(){
    gettimeofday(&last_reading, NULL);
    distance_from_shelf = 80;
    integral_distance=0;
    
    //TODO: Calibrate these values negative as when it is far away demand_distance - distance_from_shelf < 0 and actually want the speed to be > 0
    proportional_gain = -5;
    integral_gain = -5;
}

Pickup::Pickup(Motors *motorsPtr, AnalogueInterface * anaPtr, MicrocontrollerInterface *microPtr, Identifier *idenPtr){
	Pickup();
	analogue_interface = anaPtr;
	motors_interface = motorsPtr;
	micro_interface = microPtr;
	identifier_interface = idenPtr;
}

int Pickup::perform_pickup(){
    micro_interface->retract_actuator();
    
    int distances[] = {20,15,10};
    
    for (int i = 0; i < 3; i++) {
		cout << "Moving to distance: " << distances[i] << endl;
        if(set_distance_to_shelf(distances[i])){
            cout << "In position, rotating wheel..." << endl;
            //TODO: may have to implement different angles - eg first rotates 100, then 110 then 120 for tolerances
            rotate_wheel(120, false);
        }
        else{
			cout << "Failed to get to set distance" << endl;
		}
    }
    
    cout << "Extending actuator" << endl;
    micro_interface->extend_actuator();
    
    return true;
}

void Pickup::dropoff(cracker_type type){
	//TODO: Measure angle detector to base
	int cracker_angle;
	double angle_detector_base = 90;
    cout << "Request to drop type: " << type << endl;
    
	for(int i = 0; i < 3 ; i++){
        cout << "Cracker[" << i << "] type  = " << identifier_interface->crackers[i].type << endl;
		if(identifier_interface->crackers[i].type == type){
            
            cout << "Types match! Starting drop off sequence... " << endl;
            
			cracker_angle = static_cast<int>(identifier_interface->crackers[i].angle_from_reference + identifier_interface->angle_cracker1_from_detector);
            cout << "Cracker angle = " << cracker_angle << endl;
            
			cracker_angle = cracker_angle % 360;
            cout << "Cracker angle after modulus = " << cracker_angle << endl;
            
			if(cracker_angle < angle_detector_base){
                cout << "Between LDR and dropoff positions, rotating angle: " << angle_detector_base - cracker_angle << endl;
				dropoff(angle_detector_base - cracker_angle);
			}
			else{
                cout << "Past dropoff position, rotating angle: " << 360 + angle_detector_base - cracker_angle << endl;
				dropoff(360 + angle_detector_base - cracker_angle);
			}
		}
	}
}

int Pickup::dropoff(double angle_to_rotate){
    
    //TODO: Calibrate dropoff distance
    int dropoff_distance = 10;
    
    cout << "Moving to distance: " << dropoff_distance << endl;
    set_distance_to_shelf(dropoff_distance);
    cout << "In position, rotating wheel..." << endl;
    rotate_wheel(angle_to_rotate, false);
    cout << "Retracting actuator" << endl;
    micro_interface->retract_actuator();
    
    cout << "Final rotation to drop cracker" << endl;
    rotate_wheel_for_dropoff();
    
    cout << "Extending actuator" << endl;
    micro_interface->extend_actuator();
    return true;
}

int Pickup::set_wheel_speed(double demand_distance){
    //TODO: Calibrate the tolerance
    if(abs(distance_from_shelf - demand_distance) < 2){
        return 0;
    }
    else{
        
        double proportional = proportional_gain * (demand_distance - distance_from_shelf);
        double integral = integral_distance * integral_gain;
        double control_signal = proportional + integral;
        cout << "Proportional signal: " << proportional << ". Integral signal: " << integral << ". Control signal: " << control_signal << endl;
        
        //Should be in the range -1 -> 1
        double requested_speed = control_signal * speed_gain;
        int int_speed = static_cast<int>(128*requested_speed);
        
        cout << "Calculated speed after sign magnitude: " << int_speed << endl;
        return int_speed;
    }
}

void Pickup::update_integral_distance(double demanded_distance){
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    
    double diff_time = diff_ms(currentTime, last_reading)/1000;
    cout << "Difference in time (s): " << diff_time << endl;
    
    integral_distance += (diff_time * (demanded_distance - distance_from_shelf));
    cout << "Integral distance: " << integral_distance << endl;
}

bool Pickup::set_distance_to_shelf(double demanded_distance) {
    time_t start_time = time(NULL);
    int wheel_speed;
    
    //TODO: Check it can reach required distance in time
    while(difftime(time(NULL), start_time) < 10){
        wheel_speed = set_wheel_speed(demanded_distance);
        cout << "Demanded wheel speed" << wheel_speed << endl;
        motors_interface->set_drive_motor_speed(wheel_speed, wheel_speed);
        
        if(wheel_speed != 0){
            continue;
        }
        else{
            cout << "Reached demanded_distance" << endl;
            return true;
        } 
    }
    //Hasn't reached required distance
    motors_interface->set_drive_motor_speed(0, 0);
    return false;
}

bool Pickup::rotate_wheel(double angle_in_degrees, bool final_drop){
    
    cout << "Started rotating wheel. Demanded angle = " << angle_in_degrees << endl;
    
    double angle_moved = 0;
    double current_speed = 0;
    double diffTime = 0;
    int setRotationSpeed = 127;
    
    if(final_drop){
		setRotationSpeed = 60;
	}
    
    timeval time_last_speed_reading, current_time;
    gettimeofday(&time_last_speed_reading, NULL);
    
    motors_interface->set_motor_speed(3, setRotationSpeed);
    
    while(angle_moved < angle_in_degrees){
        
        current_speed = motors_interface->MAX_ROTATION_SPEED * setRotationSpeed / 127;
        
        gettimeofday(&current_time, NULL);
        
        diffTime = diff_ms(current_time, last_reading)/1000;
        
        //Updating angles of crackers
		identifier_interface->update_angle(current_speed * diffTime);
		
        angle_moved += current_speed * diffTime;
        
        cout << "Current angle: " << angle_moved << endl;
        time_last_speed_reading = current_time;
    }
    cout << "Reached demanded angle." << endl;
    motors_interface->set_motor_speed(3, 0);
    return true;
}

bool Pickup::rotate_wheel_for_dropoff(){
	cout << "Doing final rotation to drop off the cracker" << endl;
	return rotate_wheel(60, true);
}
