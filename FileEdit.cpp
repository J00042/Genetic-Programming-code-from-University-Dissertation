/* FileEdit.cpp */

#include "FileEdit.h"

using namespace std;

FileEdit::FileEdit(){
	path = "_";
	filename = "_";
}

FileEdit::FileEdit(string pathIn, string filenameIn){
	path = pathIn.c_str();
	filename = filenameIn.c_str();
	
	/* try to open the file. */	
	inputFile.open((path + filename).c_str());	//the input file.
	if(inputFile.is_open()){
		cout << "The file '" << filename << "' was found." << endl;
	}else{
		cout << "The file '" << path << filename << "' could not be found." << endl;
	}
	inputFile.close();
}


int FileEdit::setFile(string pathIn, string filenameIn){ //A method to set the file path and name. Returns 0 if the file is found, -1 if not.
	int i;	
	path = pathIn.c_str();
	filename = filenameIn.c_str();
	
	/* try to open the file. */	
	inputFile.open((path + filename).c_str());	//the input file.
	if(inputFile.is_open()){
		cout << "The file '" << filename << "' was found." << endl;
		i=0;
	}else{
		cout << "The file '" << path << filename << "' could not be found." << endl;
		i=-1;
	}
	inputFile.close();
	return i;
}
	
string FileEdit::getPath(){	//returns the file path.
	return path;
}

string FileEdit::getFilename(){	//returns the file name.
	return filename;
}

int FileEdit::findText(string text){	//returns the line number in the file of a given string. 
	string line;
	int lineNumber = 1;
	int loop = 1;	
	inputFile.open((path + filename).c_str());
	
	//Iterate over each line until one that equals the requested string is found. 
	if(inputFile.is_open()){
		while((!inputFile.eof()) && (loop)){
			getline(inputFile, line);
			//cout << line << endl;			
			if(line == text){
				loop = 0;
			}else{
				lineNumber++;
			}
		}
	}else{
		cout << "The file '" << filename << "' could not be opened." << endl;
	}
	inputFile.close();
	//If a matching line was not found, return 0. Otherwise, return the line number of the mathcing line. 
	if(loop){
		return 0;
	}else{
		return lineNumber;
	}
}

int FileEdit::replaceLine(int lineNumberChange, string text){	//replaces the line at the given line number with the given string. 
	//In order to edit lines in a file in C++, the file has to be transferred line by line to a new file with the alteration made during. 
	outputFile.open("tmp.cpp");
	inputFile.open((path + filename).c_str());
	string line;
	int lineNumber = 1;
	if(!inputFile.is_open() || !outputFile.is_open()){
		cout << "The files '" << filename << "' and 'tmp.cpp' could not be opened." << endl;
		return -1;
	}
	//cout << "copy and change:" << endl;
	//while(!inputFile.eof()){
	while(getline(inputFile, line)){
		//getline(inputFile, line);
		if(lineNumber == lineNumberChange){
			outputFile << text << endl;
			//cout << "line " << lineNumber << " replaced with: " << text << endl;
		}else{
			outputFile << line << endl;
			//cout << "line " << lineNumber << ": " << line << endl;
		}
		lineNumber++;
	}
	//cout << endl;
	inputFile.close();
	outputFile.close();
	//Overwrite the old file with the contents of the tmp file. 
	inputFile.open("tmp.cpp");
	outputFile.open((path + filename).c_str()); //delete the old file.
	//cout << "rename file:" << endl;
	//while(!inputFile.eof()){
	while(getline(inputFile, line)){
		//getline(inputFile, line);
		outputFile << line << endl;
		//cout << "out:" << line << endl;
	}
	inputFile.close();
	outputFile.close();
	remove("tmp.cpp");	//delete the tmp file. 
	return 0;
}

string FileEdit::readCSV(int row, int column){	//A method to read a CSV file and return a specific item. 
	ifstream inputFile;	
	string line, sub, output = "_";
	int i=0, start=0, end=0;
	
	//try to open the file.	
	inputFile.open((path + filename).c_str());	//the input file.
	if(inputFile.is_open()){
		//cout << "The file '" << filename << "' was found." << endl;
	}else{
		cout << "The file '" << path << filename << "' could not be found." << endl;
	}
	//read and remember the correct row.
	for(i=0; i<row; i++){
		getline(inputFile, line);
	}
	//extract the contents from the correct column (note empty slots causes later slots to be read incorrectly). 
	for(i=0; i<column-1; i++){
		start = line.find(",", start+1) +1;
	}
	end = line.find(",", start+1);
	output = line.substr(start, end-start);
	inputFile.close();
	return output;
}

