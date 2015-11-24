//
//  brain.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef brain_hpp
#define brain_hpp

#include <stdio.h>
#include <iostream>
#include "mechanical.hpp"
#include "line_follower.hpp"
#include "navigator.hpp"
#include "identifier.hpp"
using namespace std;


enum class brain_status { move_to_pickup,
    picking_up,
    picked,
    identifying,
    indentified,
    dropping,
    moving_drop,
};

// declare Brain class and its properties + methods
class Brain {
public:
    
    brain_status Status;
    
    int nCrackers;
    int packed;
    
    void status_check();
    
    /*
     void change_status( uint16_t status_name ){
     status = status_name;
     };
     */
    
};

#endif /* brain_hpp */
