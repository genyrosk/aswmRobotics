# Automatically generated Makefile
main :  peakdetection.o main.o line_follower.o idp.o identifier.o cracker.o
	 g++ -L/export/teach/1BRobot -o main  peakdetection.o  main.o  line_follower.o  idp.o  identifier.o  cracker.o  -lrobot

peakdetection.o: peakdetection.cpp peakdetection.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c peakdetection.cpp
main.o: main.cc idp.h line_follower.hpp identifier.hpp torqueanalysis.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c main.cc
line_follower.o: line_follower.cpp line_follower.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_follower.cpp
idp.o: idp.cc idp.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c idp.cc
identifier.o: identifier.cpp identifier.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c identifier.cpp
cracker.o: cracker.cpp cracker.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c cracker.cpp
