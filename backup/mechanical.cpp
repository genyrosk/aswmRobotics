//
//  mechanical.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "mechanical.hpp"
using namespace std;


Idp *idp;

bool MicrocontrollerInterface::write(int output_byte){
    //TODO: Error handling
    return idp->rlink.command (WRITE_PORT_1, output_byte);
}

int MicrocontrollerInterface::read(int port_activation_byte){
    //TODO: Error handling
    if(idp->rlink.command (WRITE_PORT_1, port_activation_byte)){
    return idp->rlink.request(READ_PORT_1);
    }
    else {
        return 0;
    }
}


double AnalogueInterface::readADC(int port){
    double request_output = 0.0;
    double max_reading = 255.0;
    
    switch(port)
    {
        case 0:
            request_output = idp->rlink.request(ADC0);
            return request_output/max_reading;
        case 1:
            request_output = idp->rlink.request(ADC1);
            return request_output/max_reading;
        case 2:
            request_output = idp->rlink.request(ADC2);
            return request_output/max_reading;
        default:
            return request_output;
    }

}

void Motors::set_motor_speed(int motor, int speed){
    switch(motor){
        case 1:
            idp->rlink.command (MOTOR_1_GO, speed);
            break;
        case 2:
            idp->rlink.command(MOTOR_2_GO, speed);
            break;
        case 3:
            idp->rlink.command (MOTOR_3_GO, speed);
            break;
        default:
            break;
    }
    
}

int Motors::get_motor_speed(int motor){
    switch(motor){
        case 1:
            return idp->rlink.request (MOTOR_1);
        case 2:
            return idp->rlink.request (MOTOR_2);
        case 3:
            return idp->rlink.request (MOTOR_3);
            return 0;
        default:
            return 0;
            break;
    }
}

void Motors::set_drive_motor_speed(int left, int right){
    set_motor_speed(1, left);
    set_motor_speed(2, right);
}

void Motors::set_ramp_time(int ramp_time){
    idp->rlink.command (RAMP_TIME, ramp_time);
}


///Returns distance in cm from the distance detector 
double AnalogueInterface::get_distance(){
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
