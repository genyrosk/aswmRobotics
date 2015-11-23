#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <sstream>
#include <cmath>

#include "line_follower.hpp"
#include "identifier.hpp"
#include "torqueanalysis.hpp"

#define ROBOT_NUM  14                        // The id number (see below)
robot_link  rlink;                            // datatype for the robot link

// Functions 
void motors_start(int speed);
void motors_mode(int mode);
int connect_remotely();
int get_sensor_output();
void stay_in_line( int current_status );
stopwatch watch;
ostringstream oss;
int speed;

int main (){
	
	if ( connect_remotely() == -1 ){
		return -1;
	}
	
	speed = 100;
	
	const int bit0 = 0x01;      // ’0000 0001’ individual bits
	const int bit5 = 0x20;
	
	cout << bit0 << endl << bit5 << endl << endl;
	
	//Identifier identifier;
	//identifier.id_procedure();
	
	LineFollower linefollower;
	int current_status = linefollower.current_status;
	cout << "current status: " << current_status << endl << endl;
	
	stay_in_line( current_status );
	
	
	int sensor[10] = {0x01,0x04,0x06,0x01,0x02,0x05,0x07,0x01,0x04,0x02};
	
	for (int i=0; i<10; i++){
		
		current_status = sensor[i];
		stay_in_line( current_status );
		delay(1000);
	}
	
	
	
	cout << "starting motors at speed " << speed << endl;
	motors_start(speed);
	delay(4000);
	
	cout << "hex codes" << endl ;
	int hex = get_sensor_output();
	cout << hex << endl;
	
	return 0;
}

int get_sensor_output(){
	
	int iBus [4];
	int hex;
	string hex_str;
	
	iBus[0] = rlink.request (READ_PORT_0);
	iBus[1] = rlink.request (READ_PORT_1);
	iBus[2] = rlink.request (READ_PORT_2);
	iBus[3] = rlink.request (READ_PORT_3);
	
	for(int i=0; i<4; i++){
		oss << abs(iBus[i]);
		hex_str = oss.str();
		cout << hex_str << endl;
	}
	
	cout << hex_str << endl;
	istringstream ( hex_str ) >> hex;
	
	return hex;
}

void stay_in_line( int current_status ){
	
	switch (current_status) {
        case 0x01:
            //slightly too far left
            cout << "slightly too far left" << endl;
            motors_mode(3);
            break;
        case 0x02:
            // we're fucked... must be the negative ramp, bastards
            cout << "we're fucked... must be the negative ramp, bastards" << endl;
            motors_mode(5);
            break;
        case 0x03:
            // too far left
            cout << "too far left" << endl;
            motors_mode(4);
            break;
        case 0x04:
            // slightly too far right
            cout << "slightly too far right" << endl;
            motors_mode(1);
            break;
        case 0x05:
            // perfect
            cout << "perfect" << endl;
            motors_mode(0);
            break;
        case 0x06:
            // too far right
            cout << "too far right" << endl;
            motors_mode(2);
            break;
        case 0x07:
            // lost line
            cout << "lost line" << endl;
            motors_mode(5);
            break;
        default:
            break;
    }
	
	
}


void motors_mode(int mode){
	
	switch(mode){
		case 0: 
		// continue going straight
			rlink.command (BOTH_MOTORS_GO_SAME, speed);
			break;
		case 1: 
		// compensate right slightly
			rlink.command (MOTOR_1_GO, speed - 20);
			rlink.command (MOTOR_2_GO, speed + 20);
			break;
		case 2: 
		// compensate right strongly
			rlink.command (MOTOR_1_GO, speed - 40);
			rlink.command (MOTOR_2_GO, speed + 40);
			break;
		case 3: 
		// compensate left slightly
			rlink.command (MOTOR_1_GO, speed + 20);
			rlink.command (MOTOR_2_GO, speed - 20);
			break;
		case 4: 
		// compensate left strongly
			rlink.command (MOTOR_1_GO, speed + 40);
			rlink.command (MOTOR_2_GO, speed - 40);
			break;
		case 5: 
		// go backwards 
			rlink.command (BOTH_MOTORS_GO_OPPOSITE, speed);
			break;
		default:
			break;
	}
	

}

void motors_start(int speed) {

	// red -> 1
	// green -> 2
	// blue -> 3
	// yellow -> 4


	//rlink.command (BOTH_MOTORS_GO_SAME, speed);
	cout << "In motors_start, starting motor 4 " << endl;
	rlink.command (MOTOR_1_GO, speed);
	delay(1000);
	rlink.command (MOTOR_2_GO, speed);
}

int connect_remotely(){
	watch.start();
	cout << "..." << endl;

	int   val;
	// data from microprocessor
	if (!rlink.initialise(ROBOT_NUM)) {      // setup the link
		cout << "Cannot initialise link" << endl;
		rlink.print_errs("    ");
		return -1;
	}
	
	val = rlink.request(TEST_INSTRUCTION);   // send test instruction
	if (val == TEST_INSTRUCTION_RESULT) {     // check result
		cout << "Connected" << endl;
		//return 0;                             // all OK, finish
	}
	else if (val == REQUEST_ERROR) {
		cout << "Fatal errors on link:" << endl;
		rlink.print_errs();
		return -1;
	}
	else{
		cout << "Test failed (bad value returned)" << endl;
		return -1;
	// error, finish
	}
	cout << "connection time " << watch.stop() << endl;
	return 1;
}



