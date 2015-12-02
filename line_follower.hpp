//
//  line_follower.hpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 17/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef line_follower_hpp
#define line_follower_hpp

#include "idp.h"

#include <stdio.h>
#include <time.h>
#include <iostream>
#include "mechanical.hpp"
using namespace std;

class LineFollower {
public:

	double proportional_error, integral_error, distance_moved;
	
	//TODO: Calibrate these gain values
	double proportional_gain;
	static const double integral_gain = 10;
    int left_wheel_speed;
    
    int right_wheel_speed;
	int current_status;
    bool negative_ramp;
    bool junction_detected;
    
	LineFollower();
	LineFollower(Motors * motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr);
	int follow_line(double distance, bool toJunction);
	int follow_line(double distance, bool toJunction, double stop_obstacle_distance);
	int follow_line(double distance, bool toJunction, double stop_obstacle_distance, bool junctionDistance);
	int turn(double angle_in_degrees, int speed);
	int reverse_after_pickup();
	int reverse_after_pickup(double distance);
    
    void get_path_status();
    void turn_degrees(double degrees);

private:
    
    Motors *motors_interface;
    MicrocontrollerInterface * micro_interface;
    AnalogueInterface * analogue_interface;
};


#endif /* line_follower_hpp */
