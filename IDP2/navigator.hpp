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

#include <stdio.h>
#include <time.h>
#include <iostream>

class Navigator {
public:
	
	Navigator();
	
	
	void go_to_dock();
	
	
	void go_to_junction();
	void skip_junction();
	void turn(int degrees);
	
	
	
	
	
}


#endif /* navigator_hpp */
