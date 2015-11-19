//
//  Command.cpp
//  IDP
//
//  Created by Peter Boothroyd on 17/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include "Command.hpp"
#include <stdio.h>
#include <time.h>

void Command::set_elapsed_time(){
    elapsed_time = difftime(start_time, time(NULL));
}