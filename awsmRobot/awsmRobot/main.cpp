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
    // insert code here...
    cout << "Hello, World!\n";
    
    Brain brain;
    
    cout << brain.status << endl;
    brain.status_check();
    
    return 0;
    
};



