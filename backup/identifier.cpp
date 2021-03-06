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
    
    PeakDetection peak_detector;
    string read_status = "";
    angle_cracker1_from_detector = 0;
}

void Identifier::id_procedure(){
    
    int value_simulator[60] =
    {9,5,1,6,10,1,8,2,5,3,3,7,8,5,6,
        176,173,189,180,196,194,189,171,178,189,181,195,186,198,199,179,193,176,180,178,
        160,150,140,130,110,100,90,80,70,60,
        40,36,23,6,10,1,8,2,5,3,3,7,8,5,6};//15 + 20 + 10 +15
    
    for (int i = 0; i<3; i++) {
        // iterate 3 times, fo 3 separate identifications
        cout << "------------------ " << endl;
        cout << "initiating for loop, i = " << i << endl;
        
        // start rotating with constant speed
        bool cracker_unknown = true;
        bool reading_peaks = false;
        int j = 0;
        
        while( cracker_unknown && j<60) {
            //sleep(1);
            // int data_value = read LDR -> returns an int between 1 and 255
            double data_value = value_simulator[j];
            cout << "index: " << j << endl;
            cout << "data_value: " << data_value << endl;
            
            read_status = peak_detector.add_data_point( data_value );
            cout << "read status: " << read_status << endl;
            
            if ( read_status == "PEAKREADING" ){ // i.e. it equals "PEAKREADING"
                reading_peaks = true;
                j++;
                continue;
            }
            if ( reading_peaks == true && read_status != "PEAKREADING" ) { // i.e. peakdetection has been finished
                
                time_t peakReadingTime = peak_detector.get_max_reading_time();
                double peakReading = peak_detector.get_max_reading();
                
                //TODO: Find out rotation speed.
                double rotation_speed = 1;
                
                double angleTurnedAfterPeak = difftime(time(NULL), peakReadingTime) * rotation_speed;
                angle_cracker1_from_detector = 120 * i + angleTurnedAfterPeak;
                
                cout << "------------------ " << endl;
                cout << "------------------ " << endl;
                cout << "------------------ " << endl;
                cout << "peak detected at : " << peakReading << endl;
                crackers[i].calculate_probabilities( peakReading ); // will assign a cracker_type to our cracker object
                break;
            }
            
            j++;
            
        }
    }
    
}
