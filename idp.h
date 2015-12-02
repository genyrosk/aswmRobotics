#ifndef idp_h
#define idp_h

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <sstream>
#include <time.h>
#define ROBOT_NUM  14 	// The id number (see below)

class Idp {
public:

	Idp();
	int connect_remotely();
	
	robot_link  rlink;                            // datatype for the robot link
	bool connected;    
	stopwatch watch;
	ostringstream oss;

};

inline double diff_ms(timeval t1, timeval t2){
    return (((t1.tv_sec - t2.tv_sec) * 1000000) + (t1.tv_usec - t2.tv_usec))/1000;
}


#endif
