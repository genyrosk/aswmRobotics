//
//  mechanical.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "mechanical.hpp"
using namespace std;

MicrocontrollerInterface::MicrocontrollerInterface(Idp *idpPtr){
	idp = idpPtr;
}

bool MicrocontrollerInterface::write(int output_byte){
    int requestResponse = idp->rlink.command(WRITE_PORT_4, output_byte);
    if(requestResponse == -1){
		link_err error = idp->rlink.get_err();
		cout << "Error reading, code: "<< error << endl;
		return -1;
	}
	else{
		return requestResponse;
	}
}

int MicrocontrollerInterface::read(){
	int requestResponse = idp->rlink.request(READ_PORT_4);
	cout << "Reading from microcontroller, returned: " << requestResponse << endl;
	if(requestResponse == -1){
		link_err error = idp->rlink.get_err();
		cout << "Error reading, code: "<< error << endl;
		return -1;
	}
	else{
		return requestResponse;
	}
}

void MicrocontrollerInterface::indicate_lost(){
	//SOS message
	flash_leds(100);
	flash_leds(100);
	flash_leds(100);
	delay(200);
	flash_leds(200);
	flash_leds(200);
	flash_leds(200);
	delay(200);
	flash_leds(100);
	flash_leds(100);
	flash_leds(100);
}

void MicrocontrollerInterface::flash_leds(int time){
	int current_status = read();
    cout << "Read from microcontroller: " << current_status << endl;
    
	int output_byte_off = current_status bitor 0x6F;
    int output_byte_on = current_status bitand 0x9F;
    cout << "Output byte on: " << output_byte_on << ". Output byte off: " << output_byte_off << endl;
	
	write(output_byte_on);
	delay(time);
	write(output_byte_off);
	delay(time);
}

void MicrocontrollerInterface::request_crackers(){
	int current_status = read();
    int output_byte_on = current_status bitand 0xEF;
	
	write(output_byte_on);
}

void MicrocontrollerInterface::stop_request_crackers(){
    int current_status = read();
    int output_byte_on = current_status bitor 0x1F;
    
    write(output_byte_on);
}

void MicrocontrollerInterface::led1(bool on){
	int current_status = read();
	int output_byte_off = current_status bitor 0x4F;
	int output_byte_on = current_status bitand 0xBF;
	
	if(on){
		write(output_byte_on);
	}
	else{
		write(output_byte_off);
	}
}

void MicrocontrollerInterface::led2(bool on){
	int current_status = read();
	int output_byte_off = current_status bitor 0x2F;
	int output_byte_on = current_status bitand 0xDF;
	
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
	delay(1000);
	
	led1(led1on);
	led2(led2on);
	delay(2000);
	led1(false);
	led2(false);
	
	delay(1000);
	flash_leds(100);
	flash_leds(100);
}

void MicrocontrollerInterface::retract_actuator(){
	int current_status = read();
	cout << "Extending actuator, current status: " << current_status << endl;
	int output_byte = current_status bitor 0x8F;
	
	cout << "Writing, output byte: " << output_byte << endl;
	write(output_byte);
}
void MicrocontrollerInterface::extend_actuator(){
	int current_status = read();
	cout << "Retracting actuator, current status: " << current_status << endl;
	int output_byte = current_status bitand 0x7F;
	
	cout << "Writing, output byte: " << output_byte << endl;
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
    double max_reading = 255.0;
    
    switch(port)
    {
        case 0:
        //Distance detector
            request_output = idp->rlink.request(ADC0);
            break;
        case 1:
        //LDR
            request_output = idp->rlink.request(ADC0);
            break;
		case 2:
			request_output = idp->rlink.request(ADC2);
			break;
		case 3:
			request_output = idp->rlink.request(ADC3);
			break;
        default:
            return request_output;
    }
    return 5*(request_output/max_reading);

}

/*
///Returns distance in cm from the distance detector 
double AnalogueInterface::get_distance(){
    
    double DD_return = readADC(1);
    cout << "DD returned: " << DD_return << endl;
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
		cout << "Distance = " << m/DD_return <<  endl;
		return m/DD_return;
	}
    else{
		return 120;
	}
}
*/

Motors::Motors(Idp *idpPtr){
	idp = idpPtr;
	//Max speed of drive motors in cm/s
	MAX_SPEED = 7.95;
	//Max speed in degrees/s
    MAX_ROTATION_SPEED=59.86;
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
            idp->rlink.command (MOTOR_4_GO, speed);
            break;
		case 4:
			idp->rlink.command (MOTOR_4_GO, speed);
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
    
	right *= -1;
	
    if(left > 127){
		left = 127;
	}
    else if(left < -127){
		left = -127;
	}
	if(left < 0){
		left = 127 - left;
	}
    set_motor_speed(1, left);
    
    if(right > 127){
		right = 127;
	}
	else if(right < -127){
		right = -127;
	}
	if(right < 0){
		right = 127 - right;
	}
    set_motor_speed(2, right);
}

void Motors::set_ramp_time(int ramp_time){
    idp->rlink.command (RAMP_TIME, ramp_time);
}
