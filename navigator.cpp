#include "navigator.hpp"
#include "line_follower.hpp"

Navigator::Navigator(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr, Identifier * idenPtr){
	
	linefollower = LineFollower(motorsPtr, microPtr, anaPtr);
	identifier_interface = idenPtr;
	micro_interface = microPtr;

}

//Navigate to dock from start point
bool Navigator::go_to_dock(){
	//TODO: Measure distances
	if(linefollower.follow_line(19)){
		if(linefollower.follow_line(64)){
			if(linefollower.follow_line(84)){
				return true;
			}
		}
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::reverse_after_pickup(){
	return linefollower.reverse_after_pickup();
}

bool Navigator::deliver_to_d3(){
	//TODO: Measure distances, deal with negative ramp
	if(linefollower.turn(-90,127)){
		if(linefollower.follow_line(185)){
			return true;
		}
	}
	//LOST!
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::deliver_to_d1(){
	if(linefollower.follow_line(100)){
		return true;
	}
	return false;
}

bool Navigator::test_nav(){
	if(linefollower.follow_line(50)){
		if(linefollower.turn(90, 127)){
			return true;
		}
	}
	return false;
}
