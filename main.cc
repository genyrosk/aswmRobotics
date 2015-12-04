
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
	ofstream out("debug.txt");
	cout.rdbuf(out.rdbuf());

	//Setting up interfaces
	Idp idp;
	Motors motors = Motors(&idp);	
	AnalogueInterface analogueInterface = AnalogueInterface(&idp);
	MicrocontrollerInterface microInterface = MicrocontrollerInterface(&idp);
	
	if ( idp.connected == false ){
		return -1;
	}
	
	RobotSettings robotSettings;
	if(robotSettings.load() == 0){
		cout << "Settings loaded successfully" << endl;
		}
	else{
		cout << "Settings not found" << endl;
	}
  
  long current_time = static_cast<long>(time(NULL));
  long seconds_passed = current_time - robotSettings.start_time;

      
 Identifier identifier = Identifier(&motors, &analogueInterface, &microInterface);
 Navigator nav = Navigator(&motors, &microInterface, &analogueInterface, &identifier);
 Pickup pickup = Pickup(&motors, &analogueInterface, &microInterface, &identifier);
 
 nav.go_to_dock();
 
 int nCrackerPickup = 6 - robotSettings.n_crackers_delivered;
 
 pickup.perform_pickup(nCrackerPickup);
 
 identifier.id_procedure();
 
 nav.reverse_after_pickup();
 nav.turn_left();
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
 
 nav.turn_left();
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
}
