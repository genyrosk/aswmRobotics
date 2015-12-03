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
	micro_interface->extend_actuator();
	timeval startTime, currentTime;
	double angle = 130;
	double time_to_rotate_ms = 1000 * angle / motors_interface->MAX_ROTATION_SPEED;
	cout << "Calculated time in ms: " << time_to_rotate_ms << endl;
	
	for (int i = 0; i < 3; i++) {
		// iterate 3 times, for 3 separate identifications
        cout << "------------------ " << endl;
        
        double data_value = 0;
		double max_data_value = 0;
		double diffms = 0;
		gettimeofday(&startTime, NULL);
        
        // start rotating with constant speed
        motors_interface->set_motor_speed(3, 127);
        while( diffms < time_to_rotate_ms ) {

            data_value = analogue_interface->readLDR();
            if(data_value > max_data_value){
				max_data_value = data_value;
			}
			gettimeofday(&currentTime, NULL);
			diffms = diff_ms(currentTime, startTime);
		}
		
		cout << "Maximum: " << max_data_value << endl;
		motors_interface->set_motor_speed(3, 0);
		update_angle(angle);
		crackers[i].calculate_probabilities(max_data_value); // will assign a cracker_type to our cracker object
		indicate_cracker_type(crackers[i].type);
	}
}

/*
void Identifier::id_procedure(){
    
    for (int i = 0; i<3; i++) {
        // iterate 3 times, for 3 separate identifications
        cout << "------------------ " << endl;
        cout << "initiating for loop, i = " << i << endl;
        
        // start rotating with constant speed
        motors_interface->set_motor_speed(3, 127);
        
        bool cracker_unknown = true;
        bool reading_peaks = false;
        
        while( cracker_unknown) {

            double data_value = analogue_interface->readLDR();

            cout << data_value << endl;
            
            read_status = peak_detector.add_data_point( data_value );
            //cout << "read status: " << read_status << endl;
            
            if ( read_status == "PEAKREADING" ){
                reading_peaks = true;
                continue;
            }
            if ( reading_peaks && read_status != "PEAKREADING" ) { // i.e. peakdetection has been finished
                
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
                
                motors_interface->set_motor_speed(3, 0);
                crackers[i].calculate_probabilities(peakReading); // will assign a cracker_type to our cracker object
                indicate_cracker_type(crackers[i].type);
                cracker_unknown = false;
                delay(2000);
            }
        }
    }
}
*/
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

double Identifier::get_cracker_angle(int cracker_index){
    double crackerAngle = crackers[cracker_index].angle_from_reference + angle_cracker1_from_detector;
    cout << "Cracker angle = " <<crackerAngle << endl;
    return crackerAngle;
}
