#include "navigator.hpp"
#include "line_follower.hpp"

Navigator::Navigator(){
	
	LineFollower linefollower;

}

//Navigate to dock from start point
bool Navigator::go_to_dock(){
	//TODO: Measure distances
	if(linefollower.follow_line(15)){
		if(linefollower.follow_line(80)){
			if(linefollower.follow_line(80)){
				return true;
			}
		}
	}

	return true;
}
	
void Navigator::go_to_junction(){

}

void Navigator::skip_junction(){

}

void Navigator::turn(int degrees){
	
}
