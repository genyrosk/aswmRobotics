//
//  cracker.cpp
//  IDP
//
//  Created by Peter Boothroyd on 21/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include "cracker.hpp"
#include <cmath>
using namespace std;

Cracker::Cracker(){
    Cracker(0);
}

Cracker::Cracker(double angle){
    type = UNKNOWN;
    angle_from_reference = angle;
    for(int i = 0; i<4; i++){
        type_probabilities[i] = 0;
    }
}

int Cracker::calculate_probabilities(double peak_value){
    //TODO: Calculate these values from experiments (Again Red, Black, Wood, White)
    double mean[] = {2.235,2.718,1.923,1.401};
    double std_deviation[] = {0.10437,0.17504,0.04738,0.14027};
    
    for (int i = 0; i < 4; i++) {
        double normalised_reading = fabs(peak_value - mean[i]) / std_deviation[i];
        double calculated_probability = 2*(1 - normal_dist_cdf(normalised_reading));
        type_probabilities[i] = calculated_probability;
    }
    
    int iterator = 0;
    double max = type_probabilities[0];
    for (int i = 1; i<4; i++) {
		cout << "Probability of " << cracker_type_strings[i] << " = " << type_probabilities[i] << endl;
        if(type_probabilities[i]>max){
            iterator = i;
            max = type_probabilities[i];
        }
    }
    
    type = static_cast<cracker_type>(iterator);
    cout << "Detected type: " << type << endl;
    
    return 1;
}

void Cracker::set_cracker_type(cracker_type typeInput){
	type = typeInput;
}


//Algorithm sourced from http://www.johndcook.com/blog/cpp_phi/ and is in the public domain
double Cracker::normal_dist_cdf(double x){
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;
    
    // Save the sign of x
    int sign = 1;
    if (x < 0){
        sign = -1;
    }
    x = fabs(x)/sqrt(2.0);
    
    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);
    
    return 0.5*(1.0 + sign*y);
}
