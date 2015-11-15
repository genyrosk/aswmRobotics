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
    int threshold = 10;
    
    int ir_values [3] = {0,0,0};
    int ir_status [3] = {0,0,0};
    int path_status;
    
    string command;
    
    void ir_values_logic();                     // convert war values into binary
    void follow_logic();                        // path decision making
    bool compare_arrays(int a1[], int a2[]);    // array comparison
    int get_path_status( int a[] );                 // identify current path situation
};


#endif /* line_follower_hpp */
