//
//  mechanical.hpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#ifndef mechanical_hpp
#define mechanical_hpp

#include <stdio.h>
#include <iostream>
using namespace std;


class Actuator {
public:
    int active = 0;
};

class Collector {
public:
    int active = 0;
};

class Motors {
public:
    int active = 0;
};



#endif /* mechanical_hpp */
