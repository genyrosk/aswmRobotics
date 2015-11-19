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
#include "RobotSettings.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    
    Brain brain;
    LineFollower lineFollower;
    
    //cout << brain.status << endl;
    //brain.status_check();
    
    
    cout << "path command: " << lineFollower.get_path_status(0x05) << endl;
    
    RobotSettings rSettings = RobotSettings();
    if(rSettings.load() == 0){
        cout << "Loaded previous settings successfully" << endl;
    }
    else{
        cout << "Failed to load previous settings" << endl;
    }
    
    if(rSettings.save() == 0){
        cout << "Saved settings successfully" << endl;
    }
    else{
        cout << "Failed to save settings" << endl;
    }
    return 0;
};



