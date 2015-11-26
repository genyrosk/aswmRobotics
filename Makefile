# Automatically generated Makefile
main :  torqueanalysis.o tinyxml2.o RobotSettings.o pickup.o navigator.o peakdetection.o mechanical.o main.o line_follower.o idp.o identifier.o ErrorDetection.o cracker.o
	 g++ -L/export/teach/1BRobot -o main  torqueanalysis.o  tinyxml2.o  RobotSettings.o  pickup.o  navigator.o  peakdetection.o  mechanical.o  main.o  line_follower.o  idp.o  identifier.o  ErrorDetection.o  cracker.o  -lrobot

torqueanalysis.o: torqueanalysis.cpp torqueanalysis.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c torqueanalysis.cpp
tinyxml2.o: tinyxml2.cpp tinyxml2.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c tinyxml2.cpp
RobotSettings.o: RobotSettings.cpp RobotSettings.hpp tinyxml2.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c RobotSettings.cpp
pickup.o: pickup.cpp pickup.hpp mechanical.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c pickup.cpp
navigator.o: navigator.cpp navigator.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c navigator.cpp
peakdetection.o: peakdetection.cpp peakdetection.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c peakdetection.cpp
mechanical.o: mechanical.cpp mechanical.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c mechanical.cpp
main.o: main.cc idp.h line_follower.hpp identifier.hpp torqueanalysis.hpp mechanical.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c main.cc
line_follower.o: line_follower.cpp line_follower.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_follower.cpp
idp.o: idp.cc idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c idp.cc
identifier.o: identifier.cpp identifier.hpp idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c identifier.cpp
ErrorDetection.o: ErrorDetection.cpp ErrorDetection.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c ErrorDetection.cpp
cracker.o: cracker.cpp cracker.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c cracker.cpp
