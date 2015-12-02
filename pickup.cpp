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
}

Pickup::Pickup(Motors *motorsPtr, MicrocontrollerInterface *microPtr, Identifier *idenPtr, LineFollower *linePtr){
    
	motors_interface = motorsPtr;
	micro_interface = microPtr;
	identifier_interface = idenPtr;
	linefollower_interface = linePtr;
}

int Pickup::perform_pickup(int nPickup){
    
    micro_interface->retract_actuator();
    linefollower_interface->follow_line(80,true,4,true);
    delay(500);
		
		cout << endl << "Forward junction detected" << endl << endl;
		/* * */
		double distances[] = {8.0,0.7,0.5};
    
		for (int i = 0; i < 3 && i < nPickup; i++) {
			
			linefollower_interface->left_wheel_speed = 60;
			linefollower_interface->proportional_gain = 5;
			
			double stop_distance = distances[i];
			cout << "Moving to distance: " << stop_distance << endl;
		
			linefollower_interface->follow_line(stop_distance,false);
				
			cout << "In position, rotating wheel..." << endl;
			
			//TODO: may have to implement different angles - eg first rotates 100, then 110 then 120 for tolerances
			rotate_wheel(120);
		
		}

    cout << "Pickup complete. Extending actuator" << endl;
    micro_interface->extend_actuator();
    
    linefollower_interface->reverse_after_pickup();
	
	linefollower_interface->left_wheel_speed = 100;
	linefollower_interface->proportional_gain = 20;
	
    return true;
}

int Pickup::dropoff(cracker_type type){
	//TODO: Measure angle detector to base
    
	int cracker_angle;
    int nCrackersDelivered = 0;
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
            nCrackersDelivered++;
		}
	}
    return nCrackersDelivered;
}

int Pickup::dropoff(double angle_to_rotate){
    
    //TODO: Calibrate dropoff distance
    int dropoff_distance = 10;
    
    cout << "Moving to distance: " << dropoff_distance << endl;
    
    
    linefollower_interface->follow_line(20,true,4,true);
    delay(500);
    
    linefollower_interface->left_wheel_speed = 60;
	linefollower_interface->proportional_gain = 5;
	
    linefollower_interface->follow_line(5,false);
    delay(500);
    
    cout << "In position, rotating wheel..." << endl;
    rotate_wheel(angle_to_rotate);
    cout << "Retracting actuator" << endl;
    micro_interface->retract_actuator();
    
    cout << "Final rotation to drop cracker" << endl;
    rotate_wheel(60);
    
    cout << "Extending actuator" << endl;
    micro_interface->extend_actuator();
    
    // back away to junction
    linefollower_interface->reverse_after_pickup();
    
    linefollower_interface->left_wheel_speed = 100;
	linefollower_interface->proportional_gain = 20;
	
    return true;
}


bool Pickup::rotate_wheel(double angle_in_degrees){
    
    cout << "Started rotating wheel. Demanded angle = " << angle_in_degrees << endl;
    
    int delayTime = static_cast<int>(1000 * angle_in_degrees / motors_interface->MAX_ROTATION_SPEED);
    motors_interface->set_motor_speed(3, 127);
    delay(delayTime);
    
    cout << "Reached demanded angle." << endl;
    motors_interface->set_motor_speed(3, 0);
    return true;
}

/*
bool Pickup::set_distance_to_shelf(double demanded_distance) {
    time_t start_time = time(NULL);
    int wheel_speed = 0;
    gettimeofday(&last_reading, NULL);
    
    //TODO: Check it can reach required distance in time
    while(difftime(time(NULL), start_time) < 10){
        wheel_speed = set_wheel_speed(demanded_distance);
        
        cout << "Demanded wheel speed: " << wheel_speed << endl;
        
        motors_interface->set_drive_motor_speed(wheel_speed, wheel_speed);
        
        if(wheel_speed != 0){
            continue;
        }
        else{
            cout << "--------------- Reached demanded_distance! ---------------" << endl << endl;
            return true;
        } 
    }
    //Hasn't reached required distance
    cout << "Failed to reach demanded_distance... " << endl;
    motors_interface->set_drive_motor_speed(0, 0);
    return false;
}

int Pickup::set_wheel_speed(double demand_distance){
    
    //TODO: Calibrate the tolerance
    distance_from_shelf = analogue_interface->get_distance();
    update_integral_distance(demand_distance);
    
    if(abs(distance_from_shelf - demand_distance) < 1){
        return 0;
    }
    else{
        
        double proportional = proportional_gain * (demand_distance - distance_from_shelf);
        double integral = integral_distance * integral_gain;
        double control_signal = proportional + integral;
        cout << "Proportional signal: " << proportional << ". Integral signal: " << integral << ". Control signal: " << control_signal << endl;
        
        //Should be in the range -1 -> 1
        double requested_speed = control_signal * speed_gain;
        cout << "Requested speed: " << requested_speed << endl;
        int int_speed = static_cast<int>(requested_speed);
        
        return int_speed;
    }
}

void Pickup::update_integral_distance(double demanded_distance){
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    
    double diff_time = diff_ms(currentTime, last_reading)/1000;
    last_reading = currentTime;
    
    cout << "Difference in time (s): " << diff_time << endl;
    
    integral_distance += (diff_time * (demanded_distance - distance_from_shelf));
    cout << "Integral distance: " << integral_distance << endl;
}
*/
