//
//  RobotSettings.cpp
//  IDP
//
//  Created by Peter Boothroyd on 19/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include "RobotSettings.hpp"
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <limits.h>

using namespace tinyxml2;

RobotSettings::RobotSettings(){
    start_time = static_cast<long> (time(NULL));
    n_crackers_delivered=0;
    returned_start_box_bonus=false;
    ldr_mean=0;
    ldr_std_deviation=0;
}

RobotSettings::~RobotSettings(){
    save();
}

int RobotSettings::save(){
    XMLDocument xmlDoc;
    xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());
    
    XMLNode *pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertEndChild(pRoot);
    
    XMLElement *pTimeStarted = xmlDoc.NewElement("start_time");
    pTimeStarted->SetText(start_time);
    pRoot->InsertEndChild(pTimeStarted);
    
    XMLElement *pNCrackers = xmlDoc.NewElement("n_crackers_delivered");
    pNCrackers->SetText(n_crackers_delivered);
    pRoot->InsertEndChild(pNCrackers);
    
    XMLElement *pReturnBonus = xmlDoc.NewElement("returned_start_box_bonus");
    pReturnBonus->SetText(returned_start_box_bonus);
    pRoot->InsertEndChild(pReturnBonus);
    
    XMLElement *pLdrMean = xmlDoc.NewElement("ldr_mean");
    pLdrMean->SetText(ldr_mean);
    pRoot->InsertEndChild(pLdrMean);
    
    XMLElement *pLdrStdDeviation = xmlDoc.NewElement("ldr_std_deviation");
    pLdrStdDeviation->SetText(ldr_std_deviation);
    pRoot->InsertEndChild(pLdrStdDeviation);

	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	std::string directory_path = std::string(result, (count>0) ? count : 0);
	std::cout << directory_path << std::endl;
	
	std::string file_path = directory_path + "m201robotsettings.xml";
	std::cout << file_path << std::endl;
	
	const char *char_file_path = file_path.c_str();
	
    XMLError eResult = xmlDoc.SaveFile(char_file_path);
    return eResult;
};

int RobotSettings::load(){
    XMLDocument xmlDoc;
 
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	std::string directory_path = std::string(result, (count>0) ? count : 0);
	std::cout << directory_path << std::endl;
	
	std::string file_path = directory_path + "m201robotsettings.xml";
	std::cout << file_path << std::endl;
	
	const char *char_file_path = file_path.c_str();
 
    XMLError eResult = xmlDoc.LoadFile(char_file_path);
    
    if(eResult != 0){
        return eResult;
    }
    
    XMLNode * pRoot = xmlDoc.FirstChild();
    if (pRoot == 0)
    {
        return XML_ERROR_FILE_READ_ERROR;
    }
    
    XMLElement * pTimeStarted = pRoot->FirstChildElement("start_time");
    XMLElement * pNCrackers = pRoot->FirstChildElement("n_crackers_delivered");
    XMLElement * pReturnBonus = pRoot->FirstChildElement("returned_start_box_bonus");
    XMLElement * pLdrMean = pRoot->FirstChildElement("ldr_mean");
    XMLElement * pLdrStdDeviation = pRoot->FirstChildElement("ldr_std_deviation");
    if (pTimeStarted == 0 || pNCrackers == 0 || pReturnBonus == 0 || pLdrMean == 0 || pLdrStdDeviation == 0)
    {
        return XML_ERROR_PARSING_ELEMENT;
    }
    
    XMLError eResult1 = pTimeStarted->QueryLongText(&start_time);
    XMLError eResult2 = pNCrackers->QueryIntText(&n_crackers_delivered);
    XMLError eResult3 = pReturnBonus->QueryBoolText(&returned_start_box_bonus);
    XMLError eResult4 = pLdrMean->QueryDoubleText(&ldr_mean);
    XMLError eResult5 = pLdrStdDeviation->QueryDoubleText(&ldr_std_deviation);
    if (eResult1 != 0 || eResult2 != 0 || eResult3 != 0 || eResult4 != 0 || eResult5 != 0)
    {
        return XML_ERROR_PARSING_ELEMENT;
    }
    
    return XML_SUCCESS;
};
