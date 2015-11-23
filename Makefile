# Automatically generated Makefile
main :  main.o line_follower.o
	 g++ -L/export/teach/1BRobot -o main  main.o  line_follower.o  -lrobot

main.o: main.cc line_follower.hpp identifier.hpp torqueanalysis.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c main.cc
line_follower.o: line_follower.cpp line_follower.hpp
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_follower.cpp
