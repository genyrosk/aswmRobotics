//
//  line_follower.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
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

	LineFollower();
	LineFollower(Motors * motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr);
	int follow_line(double distance);
    int current_status;
    bool negative_ramp;

private:
    
    double time_on_line, proportional_error, proportional_gain, integral_error, integral_gain, distance_moved;
    int left_wheel_speed, right_wheel_speed;
    timeval start_time_on_line;
    
    Motors *motors_interface;
    MicrocontrollerInterface * micro_interface;
    AnalogueInterface * analogue_interface;
    
    void get_path_status();                       // path decision making
    int diff_ms(timeval t1, timeval t2);
    
    //void stay_in_line( int current_status );
    //void motors_mode(int mode);
};


#endif /* line_follower_hpp */
