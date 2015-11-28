
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
#include "mechanical.hpp"
#include "RobotSettings.hpp"
#include "navigator.hpp"
#include "pickup.hpp"
#include "cracker.hpp"

// Functions 
/*
 * */
void motors_start(int speed);
void motors_mode(int mode);
stopwatch watch;
ostringstream oss;


int main (){
	/*
	RobotSettings robotSettings;
	if(robotSettings.load() == 0){
		cout << "Settings loaded successfully" << endl;
	}
	else{
		cout << "Settings not found" << endl;
	}
	*/
	
	//Setting up interfaces
	Idp idp;
	Motors motors = Motors(&idp);	
	AnalogueInterface analogueInterface = AnalogueInterface(&idp);
	MicrocontrollerInterface microInterface = MicrocontrollerInterface(&idp);
	
	if ( idp.connected == false ){
		return -1;
	}
	
	// ------- TEST CODE -------
	
	//TESTING DRIVE MOTORS:
	//motors.set_drive_motor_speed(127,127);
	//delay(100000);
	
	
    //LINE FOLLOWING TESTS:
	LineFollower linefollower = LineFollower(&motors, &microInterface, &analogueInterface);
	cout << "current status: " << linefollower.current_status << endl;
	linefollower.get_path_status();
    
    //FOLLOW STRAIGHT LINE
	linefollower.follow_line(100.0, true);
    
    //FOLLOW SET DISTANCE
    linefollower.follow_line(50.0, false);
	
    //PERFORM TURN
    linefollower.turn(-90,127);
    
    //TURN SET ANGLE
    linefollower.turn_degrees(45);
    
    
    
    //NAVIGATION TESTS:
    Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
    Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
    
    //LINKING FOLLOW LINE AND TURN
    nav.test_nav();
    
    
    
    //PICKUP TESTS:
    Pickup pickup = Pickup(&motors, &analogueInterface, &microInterface, &identifier);
    
    //STOP AT DISTANCE
    pickup.set_distance_to_shelf(10);
    
    //ALIGNS CORRECTLY
    pickup.perform_pickup();
    
    
    
    //DROPOFF TESTS:
    
    //CALCULATE ANGLE TO ROTATE
    identifier.crackers[0].type == WHITE;
    identifier.crackers[1].type == RED;
    identifier.crackers[2].type == WOOD;
    identifier.angle_cracker1_from_detector = 57;
    pickup.dropoff(WHITE);
    
    //DEALING WITH MULTIPLE CRACKERS OF SAME TYPE
    identifier.crackers[0].type == WHITE;
    identifier.crackers[1].type == WHITE;
    identifier.crackers[2].type == WHITE;
    identifier.angle_cracker1_from_detector = 76;
    pickup.dropoff(WHITE);
    
    
    
    //IDENTIFICATION TESTS:
    
    //ANGLE UPDATED CORRECTLY
    identifier.angle_cracker1_from_detector = 78;
    cout << "Initial angle: "<< identifier.angle_cracker1_from_detector << endl;
    pickup.rotate_wheel(40, false);
    cout << "Predicted final angle: "<< 78 + 40 << ". Actual final angle: " << identifier.angle_cracker1_from_detector << endl;
    
    
    
    //LED INDICATION TESTS:
    
    //CRACKER REQUEST
    microInterface.request_crackers();
    
    //FLASH LEDS
    microInterface.flash_leds(100);
    microInterface.flash_leds(100);
    
    //SOS
    microInterface.indicate_lost();
    
    //LED1&2 ON/OFF
    microInterface.led1(true);
    delay(5000);
    microInterface.led1(false);
    delay(5000);
    microInterface.led2(true);
    delay(5000);
    microInterface.led2(false);
    delay(5000);
    
    //INDICATE TYPES
    microInterface.indicate_red();
    microInterface.indicate_black();
    microInterface.indicate_white();
    microInterface.indicate_wood();
    
    //ACTUATOR TESTS:
    microInterface.extend_actuator();
    delay(5000);
    microInterface.retract_actuator();
    
    
    
    //ANALOGUE TESTS:
    
    //READ FROM LDR
    analogueInterface.readLDR();
    
    //READ DISTANCE
    analogueInterface.get_distance();
	
	//Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
	//identifier.indicate_cracker_type(RED);
	
	
    
    
     // ---------------- COMPETITION CODE ----------------
     
     /*
     Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
     Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
     Pickup pickup = Pickup(&motors, &analogueInterface, &microInterface, &identifier);
     
     nav.go_to_dock();
     pickup.perform_pickup();
     identifier.id_procedure();
     nav.reverse_after_pickup();
     nav.deliver_to_d3();
     int nCrackers = identifier.n_crackers_present(WHITE);
     if(nCrackers > 0){
     pickup.dropoff(WHITE);
     }
     nav.nav_to_d1();
     nCrackers = identifier.n_crackers_present(BLACK);
     if(nCrackers > 0){
     nav.deliver_to_d1();
     pickup.dropoff(BLACK);
     }
     nav.deliver_to_d2();
     nCrackers = identifier.n_crackers_present(RED);
     if(nCrackers > 0){
     pickup.dropoff(RED);
     }
     nav.nav_to_d4();
     nCrackers = identifier.n_crackers_present(WOOD);
     if(nCrackers > 0){
     nav.deliver_to_d4();
     pickup.dropoff(RED);
     nav.return_after_d4();
     }
     nav.return_dock();
     
     //Made it around the board once!
     
     pickup.perform_pickup();
     identifier.id_procedure();
     nav.deliver_to_d3();
     nCrackers = identifier.n_crackers_present(WHITE);
     if(nCrackers > 0){
     pickup.dropoff(WHITE);
     }
     nav.nav_to_d1();
     nCrackers = identifier.n_crackers_present(BLACK);
     if(nCrackers > 0){
     nav.deliver_to_d1();
     pickup.dropoff(BLACK);
     }
     nav.deliver_to_d2();
     nCrackers = identifier.n_crackers_present(RED);
     if(nCrackers > 0){
     pickup.dropoff(RED);
     }
     nav.nav_to_d4();
     nCrackers = identifier.n_crackers_present(WOOD);
     if(nCrackers > 0){
     nav.deliver_to_d4();
     pickup.dropoff(RED);
     nav.return_after_d4();
     }
     nav.return_dock();
     */
    

    // ---------------- COMPETITION CODE ----------------
    
    
	/*
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
	
	/*
	if(robotSettings.save() == 0){
		cout << "Settings saved successfully" << endl;
	}
	else{
		cout << "Error saving settings" << endl;
	}
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




