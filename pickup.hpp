//
//  pickup.hpp
//  IDP
//
//  Created by Peter Boothroyd on 23/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef pickup_hpp
#define pickup_hpp

#include <stdio.h>
#include <time.h>
#include "mechanical.hpp"

class Pickup{
public:
    
    Pickup();
    
    int perform_pickup();
    int dropoff(double angle_to_rotate);
    
private:
    
    double distance_from_shelf, integral_distance, proportional_gain, integral_gain, speed_gain;
    timeval last_reading;
    AnalogueInterface *analogue_interface;
    Motors *motors_interface;
    Actuator *actuator_interface;
    
    bool set_distance_to_shelf(double demanded_distance);
    int set_wheel_speed(double demanded_distance);
    void update_integral_distance(double demanded_distance);
    bool rotate_wheel(double angle_in_degrees);
    
};

#endif /* pickup_hpp */
