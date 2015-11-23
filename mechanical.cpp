/
//  mechanical.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "mechanical.hpp"
using namespace std;

bool MicrocontrollerInterface::write(int output_byte){
    //TODO: Error handling
    return rlink.command (WRITE_PORT_1, output_byte);
}

int MicrocontrollerInterface::read(int port_activation_byte){
    //TODO: Error handling
    if(rlink.command (WRITE_PORT_1, port_activation_byte)){
    return rlink.request(READ_PORT_1);
    }
    else return 0;
}


float AnalogueInterface::readADC(int port)(){
    float request_output = 0;
    float max_reading = 255.0;
    switch(port)
    {
        case 0:
            request_output = (float)rlink.request(request_instruction::ADC0);
            return request_output/max_reading;
        case 1:
            request_output = (float)rlink.request(request_instruction::ADC1);
            return request_output/max_reading;
        case 2:
            request_output = (float)rlink.request(request_instruction::ADC2);
            return request_output/max_reading;
            default:
            return request_output;
    }
}

void Motors::set_motor_speed(int motor, int speed){
    switch(motor){
        case 1:
            rlink.command (MOTOR_1_GO, speed);
            break;
        case 2:
            rlink.command (MOTOR_2_GO, speed);
            break;
        case 3:
            rlink.command (MOTOR_3_GO, speed);
            break;
        default:
            break;
    }
    
}

int Motors::get_motor_speed(int motor){
    switch(motor){
        case 1:
            return rlink.request (MOTOR_1);
            break;
        case 2:
            return rlink.request (MOTOR_2);
            break;
        case 3:
            return rlink.request (MOTOR_3);
            break;
        default:
            return false;
            break;
    }
}

void Motors::set_drive_motor_speed(int left, int right){
    set_motor_speed(1, left);
    set_motor_speed(2, right);
}

void Motors::set_ramp_time(int ramp_time){
    rlink.command (RAMP_TIME, ramp_time);
}
