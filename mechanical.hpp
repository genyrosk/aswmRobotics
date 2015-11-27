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
#include "idp.h"
using namespace std;

class Motors{
public:
	Motors(Idp * idpPtr);
	Idp *idp;
	
	double MAX_SPEED;
    double MAX_ROTATION_SPEED;
	
    void set_drive_motor_speed(int left, int right);
    void set_motor_speed(int motor, int speed);
    int get_motor_speed(int motor);
    void set_ramp_time(int ramp_time);
};

class MicrocontrollerInterface {
public:
	MicrocontrollerInterface(Idp * idpPtr);
	Idp *idp;
    bool write(int output_byte);
    int read(int port_activation_byte);
    void indicate_lost();
    void flash_leds(int time);
    void extend_actuator();
    void retract_actuator();
    void request_crackers();
    void indicate_red();
    void indicate_black();
    void indicate_white();
    void indicate_wood();
    
private:
	void led1(bool on);
    void led2(bool on);
    void indicate_type(bool led1on, bool led2on);
};

class AnalogueInterface{
public:
	AnalogueInterface(Idp * idpPtr);
	Idp *idp;
    double get_distance();
    double readLDR();
private:
    double readADC(int port);
};

#endif /* mechanical_hpp */
