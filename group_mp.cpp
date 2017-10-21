#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include "pacman.h"

using namespace std;

// TODO; FINISH

// Create Constructor for Pacman
Maze* readMazeText(char[]);

int main() {
	// cout<<aStar()<<endl;
	char fileName[100] = "tinyMaze.lay.txt";		
	int option;
	cin >> option;	
	PacMan p(readMazeText(fileName),option);
	cout << p.mazeToString() << endl;
	string sol = (p.solve()) ? "Yes" : "No";
	cout << "Solved? " << sol << endl;
	cout << "\n\n" << endl;
	cout << p.reconstructPath() << endl;

	return 0;
}

Maze* readMazeText(char fileName[]) { // Jace changes
	ifstream file(fileName);

	if(!file.is_open()) {
		cout << "File not found." << endl;
		Maze *m = new Maze();
		return m;
	}

	string fileContents="", buffer;
	int lineCount = 0, lineWidth = 0;

	while(getline(file,buffer)){
		if(lineWidth == 0) {
			lineWidth = buffer.length();
		}

		fileContents += buffer.substr(0,lineWidth);
		lineCount++;
	}

	Maze* ret = new Maze(lineCount,lineWidth,fileContents);


	return ret;
}
