#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int computeManhattanDistance(int, int, int, int);
int computeStraightLineDistance(int, int, int, int, int);	

enum {
	WALL = '%',
	OPENPATH = ' ',
	START = 'p',
	END = '.',
	MD = 1,
	SLD = 2
};
	
class Square {
private:
	char content;

	// position variables
	int row;
	int col;

	bool visited;
	int cumulativeCost;
	int heuristic;
	Square *parent;
public:
	Square() {
		row = 0;
		col = 0;
		visited = false;
		content = 0;
	}

	Square(int row, int col,char item) {
		row = row;
		col = col;
		visited = false;
		this->content = item;		
	}

	bool isPassable() { return (content == WALL) ? false : true; }
	bool isVisited() { return visited; }

	char getItem() { return content; }
	int getRow() { return row; }
	int getCol() { return col; }
	int getHeuristic() { return heuristic; }
	int getCumulative() { return cumulativeCost; }

	void setVisited(){ visited = (!visited) ? true : false; }	
	void setSquare(int,int,int,int,char);
	
	void setHeuristic();

	void setCumulative() { cumulativeCost = parent->cumulativeCost + 1; }
	Square operator=(Square);
};

void Square::setHeuristic(int type, int destX, int destY) {
	if(type == MD) {
		heuristic = computeManhattanDistance(this->x, this->y, destX, destY);
	} 
	else if(type == SLD) {
		heuristic = computeStraightLineDistance(this->x, this->y, destX, destY, 1);
	}
	else {
		cout << "Heuristic type not valid. Resend arguments." << endl;
	}
}

Square Square::operator=(Square sq){
	this->content = sq.content;
	this->row = sq.row;
	this->col = sq.col;
	this->visited = sq.visited;
	this->cumulativeCost = sq.cumulativeCost;
	this->heuristic = sq.heuristic;
}


// TODO; FINISH


class Maze {
private:
	int length, width;
	Square **maze;
public:
	Maze(int length, int width, string mazeInFile) {
		this->length = length;
		this->width = width;
		this->maze = new Square*[length];

		for(int i = 0; i < length; i++)
			this->maze[i] = new Square[width];

		for(int i = 0, k = 0; i < length; i++) {
			for(int j = 0; j < width; j++, k++) {				
				this->maze[i][j].setSquare(i, j, mazeInFile[k]);
			}
		}
		cout<<toString()<<endl;		
	}
	~Maze() {
		for (int i = 0; i < length; i++)
			delete[] maze[i];

		delete[] maze;
	}

	Square getSquare(int x ,int y) { return maze[x][y]; }
	void setVisited(int x, int y) { maze[x][y].setVisited(); }
	// void setPath(int,int);
	int getLength() { return length; }
	int getWidth() { return width; }

	string toString() {
		for(int i = 0; i < length; i++){
			for(int j = 0; j < width; j++){
				cout<<maze[i][j];
			}
			cout << endl;
		}
	}

};


class PacMan {
private:
	Maze m;
	vector<Square> parentList;
	vector<Square> closedList;
	vector<Square> openList;
	Square current;
	Square goal;
public:
	bool addSquare(int,int);
	void scoutDirections();
	Square getLowestCost();
	void switchClosed();
	bool fin();
};

bool PacMan::addSquare(int x, int y) {
	if(x > m.getLength() || y m.getWidth() || x < 0 || y < 0) {
		return false;
	} else if (m.getSquare(x, y).getItem() == WALL) {
		return false;
	} else {
		openList.add(m.getSquare(x,y));
	}
}

Square PacMan::getLowestCostSquare() {
	int min = numeric_limits<int>::infinity();
	Square current;

	for(vector<Square>::iterator it = openList().begin; it != openList.end(); it++) {
		if(it->getCumulative() < min) {
			min = it->getCumulative();
			current = *(it);
		}
	}

	return current;
}

bool PacMan::switchClosed() {
	closedList.add(getLowestCostSquare());
	current = *(closedList.begin());	
}

void PacMan::scoutDirections() {
	addSquare(current.getRow()-1,current.getCol());
	addSquare(current.getRow(),current.getCol()-1);
	addSquare(current.getRow()+1,current.getCol());
	addSquare(current.getRow(),current.getCol()+1);
}

bool PacMan::fin() {
	if(current.getRow() == goal.getRow() && current.getCol() == goal.getCol()){
		return true;
	}
	return false;
}

int main() {
	return 0;
}

/*
	pacman starts at Pos current
	pacman checks surroundings
		if passable, it adds to the closed list
		<Square>.setParent(current);
		<Square>.setCumulativeCosts;
		closedList.add(<Square>)
		checkIfBetterPathFunction();


		
*/ 


// produce heuristics

int computeManhattanDistance(int srcX, int srcY, int destX, int destY) {
	return abs(destX-srcX) + abs(destY-srcY);
}

int computeStraightLineDistance(int srcX, int srcY, int destX, int destY, int cost) {
	return max(abs(destX-srcX),abs(destY-srcY));
}