//
//  ErrorDetection.hpp
//  IDP
//
//  Created by Peter Boothroyd on 16/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef ErrorDetection_hpp
#define ErrorDetection_hpp

#include <stdio.h>
#include <stack>
#include "Command.hpp"
using namespace std;

class ErrorDetection {
public:
    
    int status;
    time_t time_switched_to_online;
    stack<Command> command_stack;
    
    void status_check();
    time_t check_time();
    
};

#endif /* ErrorDetection_hpp */
