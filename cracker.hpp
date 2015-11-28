//
//  cracker.hpp
//  IDP
//
//  Created by Peter Boothroyd on 21/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#ifndef cracker_hpp
#define cracker_hpp

#include <stdio.h>
#include <iostream>
using namespace std;

enum cracker_type { RED, BLACK, WOOD, WHITE, UNKNOWN };
const static char* cracker_type_strings[] = {"RED", "BLACK", "WOOD", "WHITE", "UNKNOWN"};

class Cracker {
public:
    Cracker();
    Cracker(double angle);

    cracker_type type;
    
    double angle_from_reference;
    
    //Red, Black, Wood, White
    const static int max_limit = 4;
    double type_probabilities[max_limit];
    
    int calculate_probabilities(double peak_reading);

private:
    double normal_dist_cdf(double x);
};

#endif /* cracker_hpp */
