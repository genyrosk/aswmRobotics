//
//  brain.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "brain.hpp"
using namespace std;

// declare class instances here (needed in order to access their methods and properties
Actuator actuator;
LineFollower linefollower;

void Brain::status_check(){
    cout << actuator.active << endl;
    actuator.active = 1;
    cout << actuator.active << endl;
    
    cout << Brain::status << endl;
    // Brain::status = dropping; // change class status
   
    
}
// modify Brain class methods
