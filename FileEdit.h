/* FileEdit.h */

#ifndef FILEEDIT_H
#define FILEEDIT_H

#include <stdio.h>	/* remove */
#include <stdlib.h>	/* system, NULL, EXIT_FAILURE */
#include <unistd.h>	/* chdir */
#include <iostream>	/* cout, cin */
#include <fstream>	/* fin, fout */
#include <string>	/* string */
#include <cmath>

using namespace std;

class FileEdit{
   private:
	string path;	//Declare the path to the file being edited.
	string filename;	//Declare the name of the file being edited.
	ofstream outputFile;	//Declare the output file.
	ifstream inputFile;	//Declare the input file.
   public:
	FileEdit();	//constructor.
	FileEdit(string pathIn, string filenameIn);	//parameterised constructor.
	
	int setFile(string pathIn, string filenameIn); //Declare a method to set the file path and file name. Returns 0 if the file found, else -1.
	string getPath();	//Declare a method to return the file path.
	string getFilename();	//Declare a method to return the file name. 
	
	int findText(string text);	//A method to find a specific string of text in a file and return the line number it starts at.
	int replaceLine(int lineNumberChange, string text); //A method to delete the text at the specified line and replace it with the specified string.
	string readCSV(int row, int column);	//A method to read a CSV file and return a specific item. 

};	

#endif
