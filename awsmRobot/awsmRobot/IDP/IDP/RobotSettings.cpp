//
//  RobotSettings.cpp
//  IDP
//
//  Created by Peter Boothroyd on 19/11/2015.
//  Copyright © 2015 Peter Boothroyd. All rights reserved.
//

#include "RobotSettings.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;

RobotSettings::RobotSettings(){
    start_time = static_cast<long> (time(NULL));
    n_crackers_delivered=0;
    returned_start_box_bonus=false;
    ldr_mean=0;
    ldr_std_deviation=0;
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

    XMLError eResult = xmlDoc.SaveFile("/Users/peterboothroyd/Documents/Computing/IDP/aswmRobotics/m201robotsettings.xml");
    return eResult;
};

int RobotSettings::load(){
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile("/Users/peterboothroyd/Documents/Computing/IDP/aswmRobotics/m201robotsettings.xml");
    
    if(eResult != 0){
        return eResult;
    }
    
    XMLNode * pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr)
    {
        return XML_ERROR_FILE_READ_ERROR;
    }
    
    XMLElement * pTimeStarted = pRoot->FirstChildElement("start_time");
    XMLElement * pNCrackers = pRoot->FirstChildElement("n_crackers_delivered");
    XMLElement * pReturnBonus = pRoot->FirstChildElement("returned_start_box_bonus");
    XMLElement * pLdrMean = pRoot->FirstChildElement("ldr_mean");
    XMLElement * pLdrStdDeviation = pRoot->FirstChildElement("ldr_std_deviation");
    if (pTimeStarted == nullptr || pNCrackers == nullptr || pReturnBonus == nullptr || pLdrMean == nullptr || pLdrStdDeviation == nullptr)
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