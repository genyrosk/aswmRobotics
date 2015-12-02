
#include <iostream>
#include <fstream>
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
	
	//Save cout to file for debugging
	//ofstream out("debug.txt");
	//cout.rdbuf(out.rdbuf());
	
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
	
	/*
    //LINE FOLLOWING TESTS:
	
	if(linefollower.follow_line(50.0, false, 10.0)){
		cout << "Reached set distance, stopping and turning..." << endl;
		linefollower.turn_degrees(-45.0);
		linefollower.follow_line(40, true, 10.0);
	}
	
	cout << "current status: " << linefollower.current_status <<". Negative ramp: " << linefollower.negative_ramp << endl;
	
	*/
	cout << "ADC0: " << analogueInterface.readADC(0) <<  "ADC1: " << analogueInterface.readADC(1) << "ADC2: " << analogueInterface.readADC(2) <<"ADC3: " << analogueInterface.readADC(3) << endl;
	cout << "Moving to distance 20cm" << endl;
	LineFollower linefollower = LineFollower(&motors, &microInterface, &analogueInterface);
	microInterface.extend_actuator();
	Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
    Pickup pickup = Pickup(&motors, &microInterface, &identifier, &linefollower);
	Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
    /*
    if(linefollower.follow_line(10.0, true)){
		if(linefollower.follow_line(19.0, true)){
			if(linefollower.follow_line(64.0, true)){
				if(linefollower.follow_line(84.0, true, 30)){
					
				}
			}
		}
	}
	*/
	
	/*
	
	linefollower.follow_line(30.0, false);
	linefollower.follow_line(30.0, false);
	linefollower.follow_line(100.0, false);
	*/
	// perform pickup + back away a little
	pickup.perform_pickup(3);
    
	// turn left towards negative ramp
    linefollower.follow_line(170.0, false,4,true);
	nav.turn_left();
    linefollower.follow_line(170.0, false,4,true);
    linefollower.follow_line(60.0, false,4,true);
    linefollower.follow_line(60.0, false,4,true);
	
	
	pickup.dropoff(100);
	
	
	

    //READ FROM LDR
    //analogueInterface.readLDR();
	
	
    /*
    if(linefollower.follow_line(50, true, 30)){	
		cout << "Picking up..." << endl;
		pickup.perform_pickup(3);
	}
	*/
	
	//linefollower.follow_line(50.0, true, 4.0);
	
	/*
	for(int i=0; i<1000; i++){
		analogueInterface.get_distance();
		delay(1000);
	}
	*/
	
	
    /*
    //FOLLOW SET DISTANCE
    if(linefollower.follow_line(16.0, false)){
		cout << "Reached set distance, stopping and turning..." << endl;
		linefollower.turn_degrees(-90.0);
	}
	*/
	
	
    //TURN SET ANGLE
    /*
    linefollower.follow_line(30.0, true,0,true);
    linefollower.follow_line(30.0, true,0,true);
    linefollower.turn_degrees(-75.0);
    linefollower.follow_line(100.0, false);
    */

	//Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
	//linefollower.follow_line(50, true);
	//nav.go_to_dock();
 
    
    //PICKUP TESTS:
    //Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
    
    
    //STOP AT DISTANCE
    
    /*
    //ALIGNS CORRECTLY
    pickup.perform_pickup();
    
    
    
    //DROPOFF TESTS:
    
    //CALCULATE ANGLE TO ROTATE
    identifier.crackers[0].set_cracker_type(WHITE);
    identifier.crackers[1].set_cracker_type(RED);
    identifier.crackers[2].set_cracker_type(WOOD);
    identifier.angle_cracker1_from_detector = 57;
    pickup.dropoff(WHITE);
    
    //DEALING WITH MULTIPLE CRACKERS OF SAME TYPE
    identifier.crackers[0].set_cracker_type(RED);
    identifier.crackers[1].set_cracker_type(RED);
    identifier.crackers[2].set_cracker_type(RED);
    identifier.angle_cracker1_from_detector = 76;
    pickup.dropoff(WHITE);
    
    
    
    //IDENTIFICATION TESTS:
    
    //ANGLE UPDATED CORRECTLY
    identifier.angle_cracker1_from_detector = 78;
    cout << "Initial angle: "<< identifier.angle_cracker1_from_detector << endl;
    pickup.rotate_wheel(40, false);
    cout << "Predicted final angle: "<< 78 + 40 << ". Actual final angle: " << identifier.angle_cracker1_from_detector << endl;
    
    
    */

    
    
    //ANALOGUE TESTS:
    
    
    //READ DISTANCE
   //
    //analogueInterface.get_distance();
	    /*
	//Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
	 
	
	* //TESTING DRIVE MOTORS:
	//motors.set_ramp_time(50);
	//cout << "Setting motor speeds."<< endl;
	//motors.set_drive_motor_speed(60,127);
	//delay(10000);
	
	*/
    
    
     // ---------------- COMPETITION CODE ----------------
     
     /*
     Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
     Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
     Pickup pickup = Pickup(&motors, &analogueInterface, &microInterface, &identifier);
     
     nav.go_to_dock();
     int nCrackerPickup = 6 - robotSettings.n_crackers_delivered;
     pickup.perform_pickup(nCrackerPickup);
     identifier.id_procedure();
     nav.reverse_after_pickup();
     nav.deliver_to_d3();
     int nCrackers = identifier.n_crackers_present(WHITE);
     if(nCrackers > 0){
     robotSettings.n_crackers_delivered += pickup.dropoff(WHITE);
     }
     nav.nav_to_d1();
     nCrackers = identifier.n_crackers_present(BLACK);
     if(nCrackers > 0){
     nav.deliver_to_d1();
     robotSettings.n_crackers_delivered += pickup.dropoff(BLACK);
     }
     nav.deliver_to_d2();
     nCrackers = identifier.n_crackers_present(RED);
     if(nCrackers > 0){
     robotSettings.n_crackers_delivered += pickup.dropoff(RED);
     }
     nav.nav_to_d4();
     robotSettings.n_crackers_delivered += nCrackers = identifier.n_crackers_present(WOOD);
     if(nCrackers > 0){
     nav.deliver_to_d4();
     robotSettings.n_crackers_delivered += pickup.dropoff(RED);
     nav.return_after_d4();
     }
     nav.return_dock();
     
     //Made it around the board once!
     
     int nCrackerPickup = 6 - robotSettings.n_crackers_delivered;
     pickup.perform_pickup(nCrackerPickup);
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
	
	//LED INDICATION TESTS:
    
    //CRACKER REQUEST
    //microInterface.stop_request_crackers();
    //delay(2000);
    //microInterface.request_crackers();
    //delay(2000);
    //microInterface.stop_request_crackers();
    
    //FLASH LEDS
    //microInterface.flash_leds(100);
    //microInterface.flash_leds(100);
    
    //SOS
    //cout<< "SOS" << endl;
    //microInterface.indicate_lost();
    
    //LED1&2 ON/OFF
    //microInterface.led1(false);
    //delay(2000);
    //microInterface.led2(true);
    //delay(2000);
    //microInterface.led2(false);
    //delay(5000);
    
    //INDICATE TYPES
    //microInterface.indicate_red();
    //microInterface.indicate_black();
    //microInterface.indicate_white();
    //microInterface.indicate_wood();
    
    //NAVIGATION TESTS:
    //Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
    //Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
    
    
    //LINKING FOLLOW LINE AND TURN
    //nav.test_nav();
    
        
    //ACTUATOR TESTS:
    //cout << "Retracting actuator" << endl;
    //microInterface.retract_actuator();
    //motors.set_motor_speed(3,127);
    //delay(20000);
    //microInterface.extend_actuator();
    
    cout << "Reached end of main" << endl;
}
