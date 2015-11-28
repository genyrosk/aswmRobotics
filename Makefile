# Automatically generated Makefile
main :  tinyxml2.o RobotSettings.o pickup.o peakdetection.o navigator.o mechanical.o main.o line_follower.o idp.o identifier.o cracker.o
	 g++ -L/export/teach/1BRobot -o main  tinyxml2.o  RobotSettings.o  pickup.o  peakdetection.o  navigator.o  mechanical.o  main.o  line_follower.o  idp.o  identifier.o  cracker.o  -lrobot

tinyxml2.o: tinyxml2.cpp tinyxml2.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c tinyxml2.cpp
RobotSettings.o: RobotSettings.cpp RobotSettings.hpp tinyxml2.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c RobotSettings.cpp
pickup.o: pickup.cpp pickup.hpp mechanical.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c pickup.cpp
peakdetection.o: peakdetection.cpp peakdetection.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c peakdetection.cpp
navigator.o: navigator.cpp navigator.hpp line_follower.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c navigator.cpp
mechanical.o: mechanical.cpp mechanical.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c mechanical.cpp
main.o: main.cc idp.h line_follower.hpp identifier.hpp mechanical.hpp RobotSettings.hpp navigator.hpp pickup.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c main.cc
line_follower.o: line_follower.cpp line_follower.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_follower.cpp
idp.o: idp.cc idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c idp.cc
identifier.o: identifier.cpp identifier.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c identifier.cpp
cracker.o: cracker.cpp cracker.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c cracker.cpp
