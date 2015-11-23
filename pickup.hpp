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

class Pickup{
public:
    Pickup();
private:
    double distance_from_shelf, integral_distance, proportional_gain, integral_gain, speed_gain;
    time_t last_reading;
    
    int set_wheel_speed(double demanded_distance);
    double get_distance();
    void update_integral_distance();
};

#endif /* pickup_hpp */
