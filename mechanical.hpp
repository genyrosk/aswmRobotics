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


class interface{
public:
    //robot_link rlink;
};

class Actuator : public interface {
public:
    void extend();
    void retract();
};

class Collector {
public:
};

class Motors : public interface {
public:
    void set_drive_motor_speed(int left, int right);
    void set_motor_speed(int motor, int speed);
    int get_motor_speed(int motor);
    void set_ramp_time(int ramp_time);
};

class MicrocontrollerInterface : public interface {
public:
    bool write(int output_byte);
    int read(int port_activation_byte);
};

class AnalogueInterface : public interface {
public:
    double readADC(int port);
    double get_distance();
};

#endif /* mechanical_hpp */
