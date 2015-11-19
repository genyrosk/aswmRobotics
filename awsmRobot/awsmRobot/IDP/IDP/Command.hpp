//
//  Command.hpp
//  IDP
//
//  Created by Peter Boothroyd on 17/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include <time.h>

class Command {
public:
    
    int command_type;
    int speed;
    time_t start_time;
    time_t elapsed_time;
    
    void set_elapsed_time();
    void reverse_command();
};


#endif /* Command_hpp */
