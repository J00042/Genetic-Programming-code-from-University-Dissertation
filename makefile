
include ./env

compile_main:
	g++ -o main.out main.cpp FileEdit.cpp Individual.cpp Generation.cpp GP.cpp -std=c++11