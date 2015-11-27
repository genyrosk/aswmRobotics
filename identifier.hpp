//
//  identifier.hpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 15/11/15.
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
    Identifier(Motors * motorsPtr, AnalogueInterface * anaPtr, MicrocontrollerInterface * microPtr);
    
    const static int maxLimit = 3;
    Cracker crackers[maxLimit];
    string read_status;
    
    double angle_cracker1_from_detector;
    void indicate_cracker_type(cracker_type type);
    int n_crackers_present(cracker_type type);
    
    void id_procedure();
    void update_angle(double delta_angle);
    
private:
    PeakDetection peak_detector;
    AnalogueInterface *analogue_interface;
    Motors *motors_interface;
    MicrocontrollerInterface *micro_interface;
    
};

#endif /* identifier_hpp */
