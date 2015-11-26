
#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <sstream>
#include <cmath>
#include <time.h>

#include "idp.h"
#include "line_follower.hpp"
#include "identifier.hpp"
#include "torqueanalysis.hpp"
#include "mechanical.hpp"
#include "RobotSettings.hpp"

// Functions 
/*
 * */
void motors_start(int speed);
void motors_mode(int mode);
stopwatch watch;
ostringstream oss;


int main (){
	RobotSettings robotSettings;
	if(robotSettings.load() == 0){
		cout << "Settings loaded successfully" << endl;
	}
	else{
		cout << "Settings not found" << endl;
	}
	
	if(robotSettings.save() == 0){
		cout << "Settings saved successfully" << endl;
	}
	else{
		cout << "Error saving settings" << endl;
	}
	Idp idp;
	Motors motors = Motors(&idp);
	
	Actuator actuator = Actuator(&idp);
	AnalogueInterface analogueInterface = AnalogueInterface(&idp);
	MicrocontrollerInterface microInterface = MicrocontrollerInterface(&idp);
	
	if ( idp.connected == false ){
		return -1;
	}
    
	/*
	const int bit0 = 0x01;      // ’0000 0001’ individual bits
	const int bit5 = 0x20;
	
	cout << bit0 << endl << bit5 << endl << endl;
	*/
    
	Identifier identifier;
	//identifier.id_procedure();
	
	LineFollower linefollower = LineFollower(&motors, &microInterface, &analogueInterface);
	int current_status = linefollower.current_status;
	cout << "current status: " << current_status << endl << endl;
	linefollower.follow_line(100);
	
	/*
	
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
	
	double distance = 1000;
	linefollower.follow_line( distance);
	delay(6000);
	return 0;
	*/
}





/*
 * DO NOT TRASPASS
 * ---------POLICE--------POLICE----------POLICE------------
 * ---------POLICE--------POLICE----------POLICE------------
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
			idp.rlink.command (BOTH_MOTORS_GO_SAME, speed);
			break;
		case 1: 
		// compensate right slightly
			idp.rlink.command (MOTOR_1_GO, speed - 20);
			idp.rlink.command (MOTOR_2_GO, speed + 20);
			break;
		case 2: 
		// compensate right strongly
			idp.rlink.command (MOTOR_1_GO, speed - 40);
			idp.rlink.command (MOTOR_2_GO, speed + 40);
			break;
		case 3: 
		// compensate left slightly
			idp.rlink.command (MOTOR_1_GO, speed + 20);
			idp.rlink.command (MOTOR_2_GO, speed - 20);
			break;
		case 4: 
		// compensate left strongly
			idp.rlink.command (MOTOR_1_GO, speed + 40);
			idp.rlink.command (MOTOR_2_GO, speed - 40);
			break;
		case 5: 
		// go backwards 
			idp.rlink.command (BOTH_MOTORS_GO_OPPOSITE, speed);
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
	idp.rlink.command (MOTOR_1_GO, speed);
	delay(1000);
	idp.rlink.command (MOTOR_2_GO, speed);
}
*/




