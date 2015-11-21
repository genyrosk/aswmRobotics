//
//  identifier.cpp
//  awsmRobot
//
//  Created by Evgeny Roskach on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "identifier.hpp"
using namespace std;

Identifier::Identifier(){
    for (int i = 0; i<3; i++) {
        crackers[i] = Cracker(120*i);
    }
}