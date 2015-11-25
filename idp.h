#ifndef idp_h
#define idp_h

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <sstream>
#define ROBOT_NUM  14 	// The id number (see below)


class Idp {
public:

	Idp();
	int connect_remotely();
	
	robot_link  rlink;                            // datatype for the robot link
	bool connected;    
	stopwatch watch;
	ostringstream oss;
	int speed;

};

#endif
