//
//  navigator.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef navigator_hpp
#define navigator_hpp

#include "idp.h"
#include "line_follower.hpp"
#include "mechanical.hpp"
#include "identifier.hpp"

#include <stdio.h>
#include <time.h>
#include <iostream>

class Navigator {
public:

	Navigator();
	Navigator(Motors *motorsPtr, MicrocontrollerInterface * microPtr, AnalogueInterface * anaPtr, Identifier * idenPtr);
	
	bool go_to_dock();
	void turn(int degrees);
	bool test_nav();
	bool reverse_after_pickup();
	bool deliver_to_d3();
	bool deliver_to_d1();
	
	Identifier * identifier_interface;
	MicrocontrollerInterface * micro_interface;
	
	LineFollower linefollower;
};


#endif /* navigator_hpp */
