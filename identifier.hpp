//
//  identifier.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef identifier_hpp
#define identifier_hpp

#include <stdio.h>
#include <iostream>
#include "peakdetection.hpp"
#include "mechanical.hpp"
#include "cracker.hpp"
using namespace std;

class Identifier {
public:
    Identifier();
    
    //enum { maxLimit = 3 };
    const static int maxLimit = 3;
    Cracker crackers[maxLimit];
    
    void id_procedure();
    string read_status;
    int value_simulator[60];
    
//private:
    PeakDetection peak_detector;
    AnalogueInterface analogue_interface;
    
};

#endif /* identifier_hpp */
