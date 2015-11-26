#include "navigator.hpp"
#include "line_follower.hpp"

Navigator::Navigator(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr, Identifier * idenPtr){
	
	linefollower = LineFollower(motorsPtr, microPtr, anaPtr);
	identifier_interface = idenPtr;

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

bool Navigator::reverse_after_pickup(){
	return linefollower.reverse_after_pickup();
}

bool Navigator::deliver_to_d3(){
	//TODO: Measure distances, deal with negative ramp
	if(linefollower.follow_line(100)){
		if(identifier_interface->cracker_present(WHITE)){
			return true;
		}
		linefollower.turn(-90,127);
		return false;
	}
	else{
		//LOST!
		return false;
	}
}

bool Navigator::deliver_to_d1(){
	if(linefollower.follow_line(100)){
		
	}
}

bool Navigator::test_nav(){
	if(linefollower.follow_line(50)){
		if(linefollower.turn(90, 127)){
			return true;
		}
	}
	return false;
}
