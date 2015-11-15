//
//  brain.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "brain.hpp"
using namespace std;

Actuator actuator;

void Brain::status_check(){
    cout << actuator.active << endl;
}
// modify Brain class methods
