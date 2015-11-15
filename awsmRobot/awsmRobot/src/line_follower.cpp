//
//  line_follower.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "line_follower.hpp"
#include <iostream>
#include <array>
using namespace std;


void LineFollower::ir_values_logic(){
    for ( int i=0; i<3; i++ ){
        
        if(ir_values[i] > threshold){
            ir_status[i] = 0;
        } else {
            ir_status[i] = 1;
        }
    }
}

// ------
// ------
int LineFollower::get_path_status( int a[] ){
    
    // 0 -> white line
    // 1 -> black line
    int ir0[3] = {0,1,1}; // straight line following + errors
    int ir1[3] = {1,0,1};
    int ir2[3] = {1,1,0};
    
    int ir3[3] = {1,0,0}; // turn to the RIGHT available
    int ir4[3] = {0,0,1}; // turn to the LEFT available
    
    int ir5[3] = {0,0,0}; // turn to both RIGHT and LEFT available
    
    int ir6[3] = {1,1,1}; // Robot got lost or is int transition to a new position
    int ir7[3] = {0,1,0}; // we're fucked -> either an error, or we're on the white ramp !
    
    // int IR[8][3] = {ir0,ir1,ir2,ir3,ir4,ir5,ir6,ir7};
    
         if ( compare_arrays(ir_status, ir0 ) ){ path_status = 0; }
    else if ( compare_arrays(ir_status, ir1 ) ){ path_status = 1; }
    else if ( compare_arrays(ir_status, ir2 ) ){ path_status = 2; }
    else if ( compare_arrays(ir_status, ir3 ) ){ path_status = 3; }
    else if ( compare_arrays(ir_status, ir4 ) ){ path_status = 4; }
    else if ( compare_arrays(ir_status, ir5 ) ){ path_status = 5; }
    else if ( compare_arrays(ir_status, ir6 ) ){ path_status = 6; }
    else if ( compare_arrays(ir_status, ir7 ) ){ path_status = 7; }
    
    return path_status;
}

// ------
// ------
void LineFollower::follow_logic(){
    
    switch (get_path_status(ir_status)) {
        case 0:
            command = "too much to the left";
            cout << command << endl;
            break;
        case 1:
            command ="right on spot!" ;
            cout << command << endl;
            break;
        case 2:
            command = "too much to the right !" ;
            cout << command << endl;
            break;
        case 3:
            command = " we can turn right !";
            cout << command << endl;
            break;
        case 4:
            command = "we can turn left !";
            cout << command << endl;
            break;
        case 5:
            command = " we can turn both right and left !";
            cout << command << endl;
            break;
        case 6:
            command = " we're lost !!!";
            cout << command << endl;
            break;
        case 7:
            command = "we're fucked...";
            cout << command << endl;
            break;
            
        default:
            break;
    }
    
}

// ------
// ------
bool LineFollower::compare_arrays(int a1[],int a2[]){
    
    for (int i=0; i < 3; ++i){
        if (a1[i] != a2[i]){
            return false;
        }
    }
    return true;
}
