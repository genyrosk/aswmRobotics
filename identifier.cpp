//
//  identifier.cpp
//  awsmRobot
//
//  Created by Peter Boothroyd on 15/11/15.
//  Copyright © 2015 Awsme. All rights reserved.
//

#include "identifier.hpp"
#include "idp.h"
#include <sys/time.h>
using namespace std;

Identifier::Identifier(){
	double angle = 120.0;
    for (int i = 0; i<3; i++) {
        crackers[i] = Cracker(angle * i);
    }
    
    PeakDetection peak_detector;
    string read_status = "";
    angle_cracker1_from_detector = 0;
}

Identifier::Identifier(Motors * motorsPtr, AnalogueInterface * anaPtr, MicrocontrollerInterface * microPtr){
    
    Identifier();
    analogue_interface = anaPtr;
    motors_interface = motorsPtr;
    micro_interface = microPtr;
}

void Identifier::update_angle(double delta_angle){
	angle_cracker1_from_detector += delta_angle;
}

void Identifier::id_procedure(){
    
    /*
    int value_simulator[60] =
    {9,5,1,6,10,1,8,2,5,3,3,7,8,5,6,
        176,173,189,180,196,194,189,171,178,189,181,195,186,198,199,179,193,176,180,178,
        160,150,140,130,110,100,90,80,70,60,
        40,36,23,6,10,1,8,2,5,3,3,7,8,5,6};//15 + 20 + 10 +15
    */
    
    for (int i = 0; i<3; i++) {
        // iterate 3 times, for 3 separate identifications
        cout << "------------------ " << endl;
        cout << "initiating for loop, i = " << i << endl;
        
        // start rotating with constant speed
        bool cracker_unknown = true;
        bool reading_peaks = false;
        //int j = 0;
        
        while( cracker_unknown) {
			//delay(10);

            double data_value = analogue_interface->readLDR();
            //double data_value = 0;
            
            //cout << "index: " << j << endl;
            cout << "data_value: " << data_value << endl;
            
            read_status = peak_detector.add_data_point( data_value );
            cout << "read status: " << read_status << endl;
            
            if ( read_status == "PEAKREADING" ){
                reading_peaks = true;
                //j++;
                continue;
            }
            if ( reading_peaks == true && read_status != "PEAKREADING" ) { // i.e. peakdetection has been finished
                
                timeval peakReadingTime = peak_detector.get_max_reading_time();
                double peakReading = peak_detector.get_max_reading();
                cout << "Peak reading value: " << peakReading <<". Peak reading time: " << peakReadingTime.tv_sec << endl;
                
                double rotation_speed = motors_interface->MAX_ROTATION_SPEED;
                
                timeval currentTime;
                gettimeofday(&currentTime, NULL);
                
                double angleTurnedAfterPeak = diff_ms(currentTime, peakReadingTime) * rotation_speed / 1000;
                
                angle_cracker1_from_detector = 120 * i + angleTurnedAfterPeak;
                
                cout << "Rotation speed: " << rotation_speed <<". Angle turned after peak: " << angleTurnedAfterPeak << endl;
                
                cout << "------------------ " << endl;
                cout << "------------------ " << endl;
                cout << "------------------ " << endl;
                cout << "Peak reading in identifier : " << peakReading << endl;
                
                crackers[i].calculate_probabilities(peakReading); // will assign a cracker_type to our cracker object
                indicate_cracker_type(crackers[i].type);
                break;
            }
            //j++;          
        }
    }
}

void Identifier::indicate_cracker_type(cracker_type type){

	switch(type){
		case RED:
			micro_interface->indicate_red();
		break;
		case WOOD:
			micro_interface->indicate_wood();
		break;
		case WHITE:
			micro_interface->indicate_white();
		break;
		case BLACK:
			micro_interface->indicate_black();
		break;
		default:
		break;
	}
}

int Identifier::n_crackers_present(cracker_type type){
	int nType = 0;
	
	for(int i = 0; i< 3; i++){
		if(crackers[i].type == type){
			nType++;
		}
	}
	return nType;
}