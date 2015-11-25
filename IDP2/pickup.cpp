//
//  pickup.cpp
//  IDP
//
//  Created by Peter Boothroyd on 23/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include <time.h>
#include <cmath>
#include "pickup.hpp"
#include "mechanical.hpp"
#include "idp.h"


Pickup::Pickup(){
    gettimeofday(&last_reading, NULL);
    distance_from_shelf = 80;
    integral_distance=0;
    
    //TODO: Calibrate these values negative as when it is far away demand_distance - distance_from_shelf < 0 and actually want the speed to be > 0
    proportional_gain = -0.5;
    integral_gain = -0.5;
}

int Pickup::perform_pickup(){
    actuator_interface->retract();
    
    int distances[] = {20,15,10};
    
    for (int i = 0; i < 3; i++) {
        if(set_distance_to_shelf(distances[i])){
            rotate_wheel(120);
        }
    }
    
    actuator_interface->extend();
    
    return true;
}

int Pickup::dropoff(double angle_to_rotate){
    
    //TODO: Calibrate dropoff distance
    int dropoff_distance = 10;
    
    set_distance_to_shelf(dropoff_distance);
    rotate_wheel(angle_to_rotate);
    actuator_interface->retract();
    
    //rotate_wheel(**ANGLE**);
    actuator_interface->extend();
    return true;
}

int Pickup::set_wheel_speed(double demand_distance){
    //TODO: Calibrate the tolerance
    if(abs(distance_from_shelf - demand_distance) < 0.5){
        return 0;
    }
    else{
        
        double proportional = proportional_gain * (demand_distance - distance_from_shelf);
        double integral = integral_distance * integral_gain;
        double control_signal = proportional + integral;
        
        //Should be in the range -1 -> 1
        double requested_speed = control_signal * speed_gain;
        int int_speed = static_cast<int>(128*requested_speed);
        
        //Convert to sign-magnitude and force to be within bounds
        if(int_speed > 127){
            int_speed = 127;
        }
        if(int_speed<-128){
            int_speed = -128;
        }
        if(int_speed<0){
            int_speed = 128 - int_speed;
        }
        return int_speed;
    }
}

void Pickup::update_integral_distance(double demanded_distance){
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    double diff_time = diff_ms(currentTime, last_reading)/1000;
    integral_distance += (diff_time * (demanded_distance - distance_from_shelf));
}

bool Pickup::set_distance_to_shelf(double demanded_distance) {
    time_t start_time = time(NULL);
    int wheel_speed;
    
    while(difftime(time(NULL), start_time) < 10){
        wheel_speed = set_wheel_speed(demanded_distance);
        motors_interface->set_drive_motor_speed(wheel_speed, wheel_speed);
        
        if(wheel_speed != 0){
            continue;
        }
        else{
            return true;
        } 
    }
    return false;
}

bool Pickup::rotate_wheel(double angle_in_degrees){
    
    double angle_moved = 0;
    double current_speed = 0;
    double diffTime = 0;
    
    timeval time_last_speed_reading, current_time;
    gettimeofday(&time_last_speed_reading, NULL);
    
    motors_interface->set_motor_speed(3, 128);
    
    while(angle_moved < angle_in_degrees){
        
        current_speed = motors_interface->MAX_ROTATION_SPEED * motors_interface->get_motor_speed(3);
        
        gettimeofday(&current_time, NULL);
        
        diffTime = diff_ms(current_time, last_reading)/1000;
        angle_moved += current_speed * diffTime;
        
        time_last_speed_reading = current_time;
    }
    motors_interface->set_motor_speed(3, 0);
    return true;
}
