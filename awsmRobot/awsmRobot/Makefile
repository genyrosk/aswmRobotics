# Automatically generated Makefile for arm
main.arm :  torqueanalysis.arm.o tinyxml2.arm.o RobotSettings.arm.o peakdetection.arm.o navigator.arm.o mechanical.arm.o Makefile.arm.o main.arm.o line_follower.arm.o identifier.arm.o ErrorDetection.arm.o Command.arm.o brain.arm.o 
	 arm-linux-gnueabi-g++ -L/usr/arm-unknown-linux-gnueabi/lib -o main.arm torqueanalysis.arm.o  tinyxml2.arm.o  RobotSettings.arm.o peakdetection.arm.o  navigator.arm.o mechanical.arm.o  Makefile.arm.o  main.arm.o line_follower.arm.o  identifier.arm.o  ErrorDetection.arm.o  Command.arm.o  brain.arm.o  -lrobot

torqueanalysis.arm.o: torqueanalysis.cpp torqueanalysis.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c torqueanalysis.cpp -o torqueanalysis.arm.o
tinyxml2.arm.o: tinyxml2.cpp tinyxml2.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c tinyxml2.cpp -o tinyxml2.arm.o
RobotSettings.arm.o: RobotSettings.cpp RobotSettings.hpp tinyxml2.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c RobotSettings.cpp -o RobotSettings.arm.o
peakdetection.arm.o: peakdetection.cpp peakdetection.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c peakdetection.cpp -o peakdetection.arm.o
navigator.arm.o: navigator.cpp navigator.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c navigator.cpp -o navigator.arm.o
mechanical.arm.o: mechanical.cpp mechanical.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c mechanical.cpp -o mechanical.arm.o
Makefile.arm.o: Makefile
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c Makefile -o Makefile.arm.o
main.arm.o: main.cpp brain.hpp mechanical.hpp line_follower.hpp navigator.hpp RobotSettings.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c main.cpp -o main.arm.o
line_follower.arm.o: line_follower.cpp line_follower.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c line_follower.cpp -o line_follower.arm.o
identifier.arm.o: identifier.cpp identifier.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c identifier.cpp -o identifier.arm.o
ErrorDetection.arm.o: ErrorDetection.cpp ErrorDetection.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c ErrorDetection.cpp -o ErrorDetection.arm.o
Command.arm.o: Command.cpp Command.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c Command.cpp -o Command.arm.o
brain.arm.o: brain.hpp mechanical.hpp line_follower.hpp navigator.hpp identifier.hpp
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c brain.hpp -o brain.arm.o
