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
#include "identifier.hpp"
#include "cracker.hpp"

//TODO: Move private methods back
class Pickup{
public:
    
    Pickup();
    Pickup(Motors *motorsPtr, AnalogueInterface * anaPtr, MicrocontrollerInterface *microPtr, Identifier *idenPtr);
    
    int perform_pickup();
    int dropoff(double angle_to_rotate);
    int dropoff(cracker_type type);
    bool set_distance_to_shelf(double demanded_distance);
    bool rotate_wheel(double angle_in_degrees, bool final_drop);
    
private:
    
    double distance_from_shelf, integral_distance, proportional_gain, integral_gain, speed_gain;
    timeval last_reading;
    AnalogueInterface *analogue_interface;
    Motors *motors_interface;
    MicrocontrollerInterface *micro_interface;
    Identifier *identifier_interface;
    
    
    int set_wheel_speed(double demanded_distance);
    void update_integral_distance(double demanded_distance);
    bool rotate_wheel_for_dropoff();
    
};

#endif /* pickup_hpp */
