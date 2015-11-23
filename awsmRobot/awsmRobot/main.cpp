//
//  main.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 09/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include <iostream>
#include "brain.hpp"
#include "mechanical.hpp"
#include "line_follower.hpp"
#include "navigator.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    
    Brain brain;
    LineFollower lineFollower;
    
    //cout << brain.status << endl;
    //brain.status_check();
    
    int a1[3] = {0,0,0};
    int a2[3] = {0,0,0};
    
    cout << "compare arrays: " << lineFollower.compare_arrays(a1,a2) << endl;
    
    lineFollower.ir_values[0] = 12;
    lineFollower.ir_values[1] = 8;
    lineFollower.ir_values[2] = 5;
    
    cout << "ir_values[2] = " <<lineFollower.ir_values[2] << endl ;
    
    lineFollower.follow_logic();
    cout << "path status:" << lineFollower.path_status << endl;
    cout << "path command:" << lineFollower.command << endl;
    
    
    
    return 0;
};



