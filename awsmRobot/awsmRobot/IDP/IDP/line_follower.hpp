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
#include <iostream>
using namespace std;

class LineFollower {
public:
    string get_path_status(int ir_sensor_output);                        // path decision making
};


#endif /* line_follower_hpp */
