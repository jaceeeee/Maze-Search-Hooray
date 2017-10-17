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
	//cout << p.mazeToString() << endl;
	p.solve();
	cout << "\n\n" << endl;
	cout << p.mazeToString() << endl;
	return 0;
}

// String aStar() {
	// while(openSet.isNotEmpty) {
		// moveToNextSquare(); //
		// if(fin()) {
			// return reconstructPath()
		// }
		//
		// evaluateCurrentSquare();
	//}
	// return "Failure, no possible path";

// }

// String reconstructPath() {
	// Square pathSquare = current;
	// while(m.getSquare(pathSquare.getRow(), pathSquare.getCol).getContent != START) {
		// Square parent = pathSquare.getParent();
		// parent.setContent('.');
	// }
	// return m;
// }

/*
	pacman starts at Pos current
	pacman checks surroundings
		if passable, it adds to the closed list
		<Square>.setParent(current);
		<Square>.setCumulativeCosts;
		closedList.add(<Square>)
		checkIfBetterPathFunction();

*/

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
	cout << "yo naa ko diri" << endl;
	//cout<<fileContents<< endl;
	Maze* ret = new Maze(lineCount,lineWidth,fileContents);
	cout << ret->toString() << endl;
	cout << "hi" << endl;
	return ret;
}
