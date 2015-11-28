#include "navigator.hpp"
#include "line_follower.hpp"

Navigator::Navigator(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr, Identifier * idenPtr){
	
	linefollower = LineFollower(motorsPtr, microPtr, anaPtr);
	identifier_interface = idenPtr;
	micro_interface = microPtr;

}

//Navigate to dock from start point
bool Navigator::go_to_dock(){
	if(linefollower.follow_line(19.0, true)){
		if(linefollower.follow_line(64.0, true)){
			if(linefollower.follow_line(84.0, true)){
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
	if(linefollower.turn(-90,127)){
		if(linefollower.follow_line(185.0, true)){
            micro_interface->request_crackers();
			return true;
		}
	}
	//LOST!
	micro_interface->indicate_lost();
	return false;
}

bool Navigator::nav_to_d1(){
	//TODO: Measure distances, remember the distance to centre / 1.25
	linefollower.turn_degrees(-45.0);
	if(linefollower.follow_line(100.0, true)){
		if(linefollower.follow_line(100.0, false)){
			return true;
		}
	}
	return false;
}

void Navigator::deliver_to_d1(){
	linefollower.turn_degrees(-90.0);
}

bool Navigator::return_after_d1(){
	if(linefollower.reverse_after_pickup()){
		if(linefollower.turn(90.0, 127)){
		return true;
		}
	}
	return false;
}

bool Navigator::deliver_to_d2(){
	if(linefollower.follow_line(50.0, true)){
		if(linefollower.follow_line(50.0, true)){
			return true;
		}
	}
	return false;
}

bool Navigator::nav_to_d4(){
	linefollower.turn_degrees(-45.0);
	if(linefollower.follow_line(185.0, true)){
		if(linefollower.follow_line(15.0, true)){
			if(linefollower.turn(-90,127)){
				if(linefollower.follow_line(19.0, true)){
					if(linefollower.follow_line(64.0, true)){
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Navigator::deliver_to_d4(){
	//TODO: Measure distances
	if(linefollower.turn(-90,127)){
		if(linefollower.follow_line(60.0, true)){
			if(linefollower.turn(90,127)){
				return true;
			}
		}
	}
	return false;
}

bool Navigator::return_after_d4(){
	linefollower.reverse_after_pickup();
	if(linefollower.turn(90,127)){
		if(linefollower.follow_line(60.0, true)){
			if(linefollower.turn(-90,127)){
				return true;
			}	
		}
	}
}

bool Navigator::return_dock(){
	if(linefollower.follow_line(84.0, true)){
		return true;
	}
}

bool Navigator::test_nav(){
	if(linefollower.follow_line(50.0, true)){
		if(linefollower.turn(-90, 127)){
			return true;
		}
	}
	return false;
}
