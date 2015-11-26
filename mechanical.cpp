//
//  mechanical.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "mechanical.hpp"
using namespace std;

MicrocontrollerInterface::MicrocontrollerInterface(Idp *idpPtr){
	idp = idpPtr;
}

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

AnalogueInterface::AnalogueInterface(Idp *idpPtr){
	idp = idpPtr;
}

double AnalogueInterface::readADC(int port){
    int request_output = 0;
    int max_reading = 255;
    
    switch(port)
    {
        case 0:
            request_output = idp->rlink.request(ADC0);
            break;
        case 1:
            request_output = idp->rlink.request(ADC1);
            break;
        case 2:
            request_output = idp->rlink.request(ADC2);
            break;
        default:
            return request_output;
    }
    return 5*(request_output/max_reading);

}


///Returns distance in cm from the distance detector 
double AnalogueInterface::get_distance(){
    //TODO: Make call to distance detector to read voltage
    //int DD_return = 255 * readADC(**DISTANCEDETECTORPORT**)/5;
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


Motors::Motors(Idp *idpPtr){
	idp = idpPtr;
	//Max speed of drive motors in cm/s
	MAX_SPEED = 6.59;
	//Max speed in degrees/s
    MAX_ROTATION_SPEED=310;
}

void Motors::set_motor_speed(int motor, int speed){
    switch(motor){
        case 1:
            idp->rlink.command (MOTOR_1_GO, speed);
            break;
        case 2:
            idp->rlink.command (MOTOR_2_GO, speed);
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
    cout << "Left before: " << left << endl;
    if(left > 127){
		left = 127;
	}
    else if(left < -128){
		left = -128;
	}
	if(left < 0){
		left = 127 - left;
	}
	cout << "Left after: " << left << endl;
    set_motor_speed(1, left);
    cout << "Right before: " << right << endl;
    
    if(right > 255){
		right = 255;
	}
	if(right > 127){
		right = right - 128;
	}
	else if(right < 0){
		right = 0;
	}
	else if(right <= 127){
		right = 128 + right;
	}
	cout << "Right after: " << right << endl;
    set_motor_speed(2, right);
}

void Motors::set_ramp_time(int ramp_time){
    idp->rlink.command (RAMP_TIME, ramp_time);
}

Actuator::Actuator(Idp *idpPtr){
	idp = idpPtr;
}

void Actuator::extend(){
	//TODO: Get port allocations
    idp->rlink.command(WRITE_PORT_1, 0xFF);
}

void Actuator::retract(){
    //TODO: Get port allocations
    idp->rlink.command(WRITE_PORT_1, 0xFF);
}
