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
    micro_interface->stop_request_crackers();
    micro_interface->retract_actuator();
    //linefollower_interface->follow_line(80,true,4,true);
    linefollower_interface->wheel_speed_offset = -38;
	linefollower_interface->proportional_gain = 5;
		
	cout << endl << "Forward junction detected" << endl << endl;
	
	double distances[] = {10.0,0.4,0.4};

	for (int i = 0; i < 3 && i < nPickup; i++) {
		
		double stop_distance = distances[i];
		cout << "Moving to distance: " << stop_distance << endl;
	
		linefollower_interface->follow_line(stop_distance,false);
			
		cout << "In position, rotating wheel..." << endl;
		
		//TODO: may have to implement different angles - eg first rotates 100, then 110 then 120 for tolerances
		rotate_wheel(130);
	
	}

    cout << "Pickup complete. Extending actuator" << endl;
    micro_interface->extend_actuator();
    
    linefollower_interface->reverse_after_pickup();
	
	linefollower_interface->wheel_speed_offset = 0;
	linefollower_interface->proportional_gain = 20;
	
    return true;
}

int Pickup::dropoff(cracker_type type){
	//TODO: Measure angle detector to base
    
	int cracker_angle;
    int nCrackersDelivered = 0;
    
    linefollower_interface->wheel_speed_offset = -38;
	linefollower_interface->proportional_gain = 5;
	micro_interface->extend_actuator();
	
    cout << "Request to drop type: " << type << endl;
    
	for(int i = 0; i < 3 ; i++){
        cout << "Cracker[" << i << "] type  = " << identifier_interface->crackers[i].type << endl;
		if(identifier_interface->crackers[i].type == type){
            
            cout << "Types match! Starting drop off sequence... " << endl;
            // approach the box
			linefollower_interface->follow_line(5,false);
			
			cracker_angle = static_cast<int>(identifier_interface->get_cracker_angle(i));
            cout << "Cracker angle = " << cracker_angle << endl;
            
			cracker_angle = cracker_angle % 360;
            cout << "Cracker angle after modulus = " << cracker_angle << endl;

			
			cout << "Rotating angle: " << 360 - cracker_angle << endl;
			dropoff(360 - cracker_angle);
			
			
			// back away to junction
			linefollower_interface->reverse_after_pickup();
			
            nCrackersDelivered++;
		}
	}
	
	linefollower_interface->wheel_speed_offset = 0;
	linefollower_interface->proportional_gain = 20;
	micro_interface->extend_actuator();
    return nCrackersDelivered;
}

int Pickup::dropoff(double angle_to_rotate){
    
    //TODO: Move linefollowing commands out of here - may have to drop same cracker type more than once
    //linefollower_interface->follow_line(20,true,4,true);
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
    delay(5000);
    rotate_wheel(50);
    delay(5000);
    
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
    identifier_interface->update_angle(angle_in_degrees);
    return true;
}
