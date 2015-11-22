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

enum class cracker_type { RED, BLACK, WOOD, WHITE, UNKNOWN };

class Cracker {
public:
    Cracker();
    Cracker(float angle);
    
    cracker_type type;
    float angle_from_reference;
    bool delivered;
    int nReadings;
    
    //Red, Black, Wood, White
    //enum { maxLimit = 4 };
    const static int max_limit = 4;
    double type_probabilities[max_limit];
    
    //Returns 1 if cracker has been identified, 0 if not
    int calculate_probabilities(double peak_reading);

private:
    double normal_dist_cdf(float x);
};

#endif /* cracker_hpp */
