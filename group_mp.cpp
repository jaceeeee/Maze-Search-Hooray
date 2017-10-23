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

int main() {
	// cout<<aStar()<<endl;
	//while(true){
		cout << "Would you like to enter the maze ?" << endl;
		cout << "Please choose a number between 1 and 0" << endl;
		cout << "1 for entering the maze and 0 for quitting: ";
		int choice;
		cin >> choice;

		//switch(choice){}
		char fileName[100];
		int option;
		cout << "Enter file name: ";
		cin >> fileName;
		cout << endl << "Enter option type: ";
		cin >> option;
		cout << endl;

		PacMan p(readMazeText(fileName),option);
		if(p.inStartState()){
			cout << p.mazeToString() << endl;
			string sol = (p.solve()) ? "Yes" : "No";
			cout << "Solved? " << sol << endl << endl;
			p.printStatistics();
			cout << "\n\n" << p.mazeToString() << endl ;

		} else {
			cout << "recheck file name arguments" << endl;
		}

	//}
	return 0;
}
