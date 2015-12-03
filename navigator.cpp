#include "navigator.hpp"
#include "line_follower.hpp"

Navigator::Navigator(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr, Identifier * idenPtr){
	
	linefollower = LineFollower(motorsPtr, microPtr, anaPtr);
	identifier_interface = idenPtr;
	micro_interface = microPtr;

}

//Navigate to dock from start point
bool Navigator::go_to_dock(){
	linefollower.follow_line(19.0, true);
	linefollower.follow_line(19.0, true);
	linefollower.follow_line(64.0, true);
	linefollower.follow_line(84.0, false, 4, true);
	return true;
				
	//micro_interface->indicate_lost();
	//return false;
}

bool Navigator::reverse_after_pickup(){
	if(linefollower.reverse_after_pickup()){
		return true;
	}
	else{
		micro_interface->indicate_lost();
		return false;
	}
}

bool Navigator::deliver_to_d3(){
	linefollower.follow_line(185.0, false, 0, true);
	delay(500);
	linefollower.follow_line(40.0, false, 0, true);
	delay(500);
	linefollower.follow_line(40.0, false, 0, true);
	delay(500);
	linefollower.follow_line(40.0, false, 0, true);
	delay(500);
	micro_interface->request_crackers();
	return true;
}

bool Navigator::nav_to_d1(){
	//TODO: Measure distances, remember the distance to centre / 1.25
	linefollower.follow_line(80.0, true);
	linefollower.follow_line(10.0, false);
	return true;
}

void Navigator::deliver_to_d1(){
	linefollower.turn_degrees(-90.0);
}

bool Navigator::return_after_d1(){
	if(linefollower.turn(90.0, 60)){
		return true;
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::deliver_to_d2(){
	if(linefollower.follow_line(20.0, true)){
		if(linefollower.follow_line(70.0, true, 4, true)){
			return true;
		}
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::nav_to_d4(){
	if(linefollower.follow_line(185.0, true,4, true)){
		linefollower.follow_line(15, false);
		linefollower.turn_degrees(-60.0);
		linefollower.follow_line(20.0, true);
		linefollower.turn_degrees(-20.0);
		linefollower.follow_line(20.0, true);
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::deliver_to_d4(){
	//TODO: Measure distances
	if(linefollower.turn(-90,60)){
		if(linefollower.follow_line(60.0, true)){
			if(linefollower.turn(90,127)){
				return true;
			}
		}
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::return_after_d4(){
	if(linefollower.turn(90,60)){
		if(linefollower.follow_line(60.0, true, 4, true)){
			turn_left();
			linefollower.follow_line(60.0, true, 4, true);
			return true;
		}
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::return_dock(){
	if(linefollower.follow_line(84.0, true,20)){
		return true;
	}
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::turn_left(){
	linefollower.follow_line(6.0, false);
    linefollower.turn_degrees(-35.0);
    linefollower.follow_line(30.0, true);
	linefollower.reverse_after_pickup(3.5);
    linefollower.turn(-60.0, 60);
    return true;
}
