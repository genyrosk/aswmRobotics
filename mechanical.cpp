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
    return idp->rlink.command (WRITE_PORT_4, output_byte);
}

int MicrocontrollerInterface::read(int port_activation_byte){
    if(idp->rlink.command (WRITE_PORT_4, port_activation_byte)){
		return idp->rlink.request(READ_PORT_4);
    }
    else {
        return 0;
    }
}

void MicrocontrollerInterface::indicate_lost(){
	//SOS message
	flash_leds(50);
	flash_leds(50);
	flash_leds(50);
	flash_leds(200);
	flash_leds(200);
	flash_leds(200);
	flash_leds(50);
	flash_leds(50);
	flash_leds(50);
}

void MicrocontrollerInterface::flash_leds(int time){
	int current_status = read(0xFF);
	int output_byte_on = current_status bitor 0x60;
	int output_byte_off = current_status bitand 0x9F;
	
	write(output_byte_on);
	delay(time);
	write(output_byte_off);
	delay(time);
}

void MicrocontrollerInterface::request_crackers(){
	int current_status = read(0xFF);
	int output_byte_on = current_status bitor 0x10;
	int output_byte_off = current_status bitand 0xEF;
	
	write(output_byte_on);
	delay(5000);
	write(output_byte_off);
}
void MicrocontrollerInterface::led1(bool on){
	int current_status = read(0xFF);
	int output_byte_on = current_status bitor 0x20;
	int output_byte_off = current_status bitand 0xDF;
	
	if(on){
		write(output_byte_on);
	}
	else{
		write(output_byte_off);
	}
}

void MicrocontrollerInterface::led2(bool on){
	int current_status = read(0xFF);
	int output_byte_on = current_status bitor 0x40;
	int output_byte_off = current_status bitand 0xBF;
	
	if(on){
		write(output_byte_on);
	}
	else{
		write(output_byte_off);
	}
}

void MicrocontrollerInterface::indicate_black(){
	indicate_type(true, true);
}

void MicrocontrollerInterface::indicate_red(){
	indicate_type(true, false);
}

void MicrocontrollerInterface::indicate_white(){
	indicate_type(false, false);
}

void MicrocontrollerInterface::indicate_wood(){
	indicate_type(false, true);
}

void MicrocontrollerInterface::indicate_type(bool led1on, bool led2on){
	
	flash_leds(100);
	flash_leds(100);
	
	led1(led1on);
	led2(led2on);
	delay(2000);
	led1(false);
	led2(false);
	
	flash_leds(100);
	flash_leds(100);
}

void MicrocontrollerInterface::extend_actuator(){
	int current_status = read(0xFF);
	int output_byte = current_status bitor 0x80;
	write(output_byte);
}
void MicrocontrollerInterface::retract_actuator(){
	int current_status = read(0xFF);
	int output_byte = current_status bitor 0x7F;
	write(output_byte);
}

AnalogueInterface::AnalogueInterface(Idp *idpPtr){
	idp = idpPtr;
}

double AnalogueInterface::readLDR(){
	return readADC(1);
}

///Returns scaled voltage read from the ADC ports, with a reference of 5V
double AnalogueInterface::readADC(int port){
    int request_output = 0;
    int max_reading = 255;
    
    switch(port)
    {
        case 0:
        //Distance detector
            request_output = idp->rlink.request(ADC0);
            break;
        case 1:
        //LDR
            request_output = idp->rlink.request(ADC1);
            break;
        default:
            return request_output;
    }
    return 5*(request_output/max_reading);

}


///Returns distance in cm from the distance detector 
double AnalogueInterface::get_distance(){
    
    double DD_return = readADC(0);
    double m = 0;
    
    if(DD_return> 0.1961 && DD_return <= 0.7451){
        m = 28.42;
    }
    else if(DD_return > 0.7451 && DD_return <= 1.294){
        m = 24.40;
    }
    else if(DD_return > 1.294 && DD_return <= 2.3137){
        m = 24.40;
    }
    else if(DD_return > 2.3137 && DD_return <= 2.7647){
        m = 20.00;
    }
    else if(DD_return > 2.7647 && DD_return <= 3.0000){
        m = 12.98;
    }
    else if(DD_return > 3.0000 && DD_return <= 3.144){
		m = 7.00;
	}
	
	if(DD_return != 0){
		return m/DD_return;
	}
    else{
		return false;
	}
}


Motors::Motors(Idp *idpPtr){
	idp = idpPtr;
	//TODO: Check these values again.
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
        default:
            return 0;
    }
}

void Motors::set_drive_motor_speed(int left, int right){
	//Converting to sign magnitude, ensuring within correct bounds and inverting speed of left wheel as
	//it is mounted in a flipped orientation
	left *= -1;
	
    if(left > 127){
		left = 127;
	}
    else if(left < -128){
		left = -128;
	}
	if(left < 0){
		left = 127 - left;
	}
    set_motor_speed(1, left);
    
    if(right > 127){
		right = 127;
	}
	if(right < -128){
		right = 128;
	}
	else if(right < 0){
		right = 127 - right;
	}
	
    set_motor_speed(2, right);
}

void Motors::set_ramp_time(int ramp_time){
    idp->rlink.command (RAMP_TIME, ramp_time);
}
