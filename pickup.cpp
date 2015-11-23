//
//  pickup.cpp
//  IDP
//
//  Created by Peter Boothroyd on 23/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include <time.h>
#include "pickup.hpp"


Pickup::Pickup(){
    time(&last_reading);
    distance_from_shelf = 80;
    integral_distance=0;
    
    //TODO: Calibrate these values negative as when it is far away demand_distance - distance_from_shelf < 0 and actually want the speed to be > 0
    proportional_gain = -0.5;
    integral_gain = -0.5;
}

int Pickup::set_wheel_speed(double demand_distance){
    double proportional = proportional_gain * (demand_distance - distance_from_shelf);
    double integral = integral_distance * integral_gain;
    double control_signal = proportional + integral;
    //Should be in the range -1 -> 1
    double requested_speed = control_signal * speed_gain;
    int int_speed = static_cast<int>(128*requested_speed);
    
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

double Pickup::get_distance(){
    //TODO: Make call to distance detector to read voltage
    int DD_return = 0;
    double m;
    if(DD_return> 10 && DD_return <= 38){
        m = 28.42;
    }
    else if(DD_return > 38 && DD_return <= 66){
        m = 24.40;
    }
    else if(DD_return > 66 && DD_return <= 118){
        m = 24.40;
    }
    else if(DD_return > 118 && DD_return <= 141){
        m = 20.00;
    }
    else if(DD_return > 141 && DD_return <= 153){
        m = 12.98;
    }
    return m/DD_return;
}

void Pickup::update_integral_distance(){
    double diff_time = difftime(time(NULL), last_reading);
    integral_distance += (diff_time * distance_from_shelf);
}

