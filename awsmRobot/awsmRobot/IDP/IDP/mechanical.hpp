//
//  mechanical.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef mechanical_hpp
#define mechanical_hpp

#include <stdio.h>
#include <iostream>
using namespace std;


class Actuator {
public:
    int active = 0;
};

class Collector {
public:
    int active = 0;
};

class Motors {
public:
    int active = 0;
    void set_motor_speed(int motor, int speed);
    int get_motor_speed(int motor);
    void set_ramp_time(int ramp_time);
};

class MicrocontrollerInterface {
public:
    bool write(int output_byte);
    int read(int port_activation_byte);
};

class AnalogueInterface{
public:
    float readDistanceDetector();
    float readLDRDetector();
    float readTorque();
};

#endif /* mechanical_hpp */
