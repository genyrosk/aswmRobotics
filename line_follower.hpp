//
//  line_follower.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef line_follower_hpp
#define line_follower_hpp

#include <stdio.h>
#include <time.h>
#include <iostream>
using namespace std;

class LineFollower {
public:
    
    int current_status;
    time_t time_on_line;
    
    void get_path_status(int sensor_output);                        // path decision making
    bool on_junction(int sensor_output);
};


#endif /* line_follower_hpp */