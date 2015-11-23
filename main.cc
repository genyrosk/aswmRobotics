#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>

#include "line_follower.hpp"
#include "identifier.hpp"
#include "torqueanalysis.hpp"

#define ROBOT_NUM  14                        // The id number (see below)
robot_link  rlink;                            // datatype for the robot link

// Functions 
void motors_start(int speed);
stopwatch watch;


string status_type[3] = {"CALIBRATING", "BACKGROUNDREADING", "PEAKREADING"};
string status =  status_type[1];

int main (){
	
	/*
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
	
	//cout << "starting motors" << endl;
	
	//int speed = 100;
	//motors_start(speed);
	//delay(6000);
	*/
	
	Identifier identifier;
	identifier.id_procedure();
	
	//cout << cracker[1].type2 << endl;
	
	return 0;
}

void motors_start(int speed) {

	// red -> 1
	// green -> 2
	// blue -> 3
	// yellow -> 4


	//rlink.command (BOTH_MOTORS_GO_SAME, speed);
	cout << "In motors_start, starting motor 4 " << endl;
	rlink.command (MOTOR_3_GO, speed);
	delay(1000);
	rlink.command (MOTOR_2_GO, speed);
}



