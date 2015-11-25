# Automatically generated Makefile
main :  torqueanalysis.o tinyxml2.o peakdetection.o mechanical.o main.o line_follower.o idp.o identifier.o cracker.o
	 g++ -L/export/teach/1BRobot -o main  torqueanalysis.o  tinyxml2.o  peakdetection.o  mechanical.o  main.o  line_follower.o  idp.o  identifier.o  cracker.o  -lrobot

torqueanalysis.o: torqueanalysis.cpp torqueanalysis.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c torqueanalysis.cpp
tinyxml2.o: tinyxml2.cpp tinyxml2.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c tinyxml2.cpp
peakdetection.o: peakdetection.cpp peakdetection.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c peakdetection.cpp
mechanical.o: mechanical.cpp mechanical.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c mechanical.cpp
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
