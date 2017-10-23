#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

enum {
	WALL = '%',
	OPENPATH = ' ',
	START = 'P',
	END = '.',
	MD = 1,
	SLD = 2
};

int computeManhattanDistance(int, int, int, int);
int computeStraightLineDistance(int, int, int, int, int);

class Square {
private:
	char content;

	// position variables
	int row;
	int col;

	bool visited;
	int cumulativeCost; // g - score
	int heuristic;		// h - score
	int fScore;
	Square *parent;
public:
	Square() {
		row = 0;
		col = 0;
		visited = false;
		content = 0;
		cumulativeCost = heuristic = fScore = 0;
	}

	Square(const Square& sq) {
		this->content = sq.content;
		this->row = sq.row;
		this->col = sq.col;
		this->visited = sq.visited;
		this->cumulativeCost = sq.cumulativeCost;
		this->heuristic = sq.heuristic;
	}

	// set cumulativeCost of each square to infinity
	Square(int row, int col,char item) {
		this->row = row;
		this->col = col;
		visited = false;
		this->content = item;
		cumulativeCost = heuristic = fScore = 0;
		// this->cumulativeCost = numeric_limits<int>::infinity();
	}

	bool isPassable() { return (content == WALL) ? false : true; }

	void setParent(Square *sq) { parent = sq; }
	Square* getParent() { return parent; }
	bool isVisited() { return visited; } // change to: ?
	// bool isVisisted(Vector<Square> closedList) {
		// return find(closedList.begin(), closedList.end(), this)
	// }

	char getItem() { return content; }
	int getRow() { return row; }
	int getCol() { return col; }

	int getHeuristic() { return heuristic; }
	int getCumulative() { return cumulativeCost; }
	int getFScore() { return fScore; }

	void setVisited(){ visited = (!visited) ? true : false; }
	void setSquare(int,int);

	void setHeuristic(int,int,int);
	void setCumulative(int newCost) { this->cumulativeCost = newCost; }
	void setFScore() { fScore = cumulativeCost + heuristic; }

	string toString() {
		char buffer[20];
		sprintf(buffer, "Square (%d, %d)", this->getRow(), this->getCol());
		string str = buffer;
		return str;
	}

	Square& operator=(const Square& sq){
		this->content = sq.content;
		this->row = sq.row;
		this->col = sq.col;
		this->visited = sq.visited;
		this->cumulativeCost = sq.cumulativeCost;
		this->heuristic = sq.heuristic;
		return *this;
	}
	// void operator++(int);
};

void Square::setSquare(int h_val, int g_val) {
	heuristic = h_val;
	cumulativeCost = g_val;
}

void Square::setHeuristic(int type, int destX, int destY) {
	if(type == MD) {
		heuristic = computeManhattanDistance(this->row, this->col, destX, destY);
	}
	else if(type == SLD) {
		heuristic = computeStraightLineDistance(this->row, this->col, destX, destY, 1);
	}
	else {
		cout << "Heuristic type not valid. Resend arguments." << endl;
	}
}



// void Square::operator++(int i) {
// 	this->cumulativeCost++;
// }

class Maze {
private:
	int length, width;
	Square *start;
	Square *end;	//Jace changes
	Square **maze;
public:
	Maze() {
		this->length = 0;
		this->width = 0;
	}

	Maze(const Maze& m) {
		this->length = m.length, this->width = m.width;
		this->maze = m.maze;

		for(int i = 0; i < length; i++)
			this->maze[i] = new Square[width];

		for(int i = 0, k = 0; i < length; i++) {
			for(int j = 0; j < width; j++, k++) {
				this->maze[i][j]=m.maze[i][j];
				if(this->maze[i][j].getItem() == START) { //Jace changes
					this->start = &this->maze[i][j];
				}
				else if(this->maze[i][j].getItem() == '.') { //Jace changes
					this->end = &this->maze[i][j];
				}
			}
		}
	}

	Maze(int length, int width, string mazeInFile) {
		this->length = length;
		this->width = width;
		this->maze = new Square*[length];

		for(int i = 0; i < length; i++)
			this->maze[i] = new Square[width];

		for(int i = 0, k = 0; i < length; i++) {
			for(int j = 0; j < width; j++, k++) {
				this->maze[i][j]=Square(i,j,mazeInFile[k]);
				if(this->maze[i][j].getItem() == START) { //Jace changes
					this->start = &this->maze[i][j];
				}
				else if(this->maze[i][j].getItem() == '.') { //Jace changes
					this->end = &this->maze[i][j];
				}
			}
		}
	}

	~Maze() {
		for (int i = 0; i < length; i++)
			delete[] maze[i];

		delete[] maze;
		delete start;
		delete end;
	}


	Square* getSquare(int x ,int y) { return &maze[x][y]; }
	Square* getStartingSquare() { return this->start; }
	Square getEndSquare() { return *this->end; }

	void setVisited(int x, int y) { maze[x][y].setVisited(); } // remove?
	void setParent(int x, int y, Square* parent) { maze[x][y].setParent(parent); }
	void setCumulativeCost(int x, int y, int cost) { maze[x][y].setCumulative(cost); }
	void setHeuristic(int type, Square goal, int x, int y) { maze[x][y].setHeuristic(type,goal.getRow(),goal.getCol()); }
	void setFScore(int x, int y) { maze[x][y].setFScore(); }

	// void setPath(int,int);
	int getLength() { return length; }
	int getWidth() { return width; }

	string toString() {
		string stringMaze = "";
		for(int i = 0; i < length; i++){
			for(int j = 0; j < width; j++){
				stringMaze += maze[i][j].getItem();
			}
			stringMaze += '\n';
		}
		cout << stringMaze << endl;
		return stringMaze;
	}

	Maze& operator=(const Maze&);

};


Maze& Maze::operator=(const Maze& maze) {
	this->length = maze.length, this->width = maze.width;
	this->maze = maze.maze;

	for(int i = 0; i < length; i++)
		this->maze[i] = new Square[width];

	for(int i = 0, k = 0; i < length; i++) {
		for(int j = 0; j < width; j++, k++) {
			this->maze[i][j]=maze.maze[i][j];
			// cout << "Happening...? " << maze.maze[i][j].getItem() << endl;
			if(this->maze[i][j].getItem() == START) { //Jace changes
				this->start = &this->maze[i][j];
				// cout << "Here" << endl;
			}
			else if(this->maze[i][j].getItem() == '.') { //Jace changes
				this->end = &this->maze[i][j];
			}
		}
	}

	return *this;
}

class PacMan {
private:
	Maze m;
	int heuristicType;
	vector<Square> parentList;//Jace: do we still need this?
	vector<Square*> closedList;
	vector<Square*> openList;
	Square *current;
	Square goal;
public:
	// Pacman(Maze m) {
	//		set start square to openList
	//		initialize goal square?
	// }
	PacMan(Maze maze, int type) { //Jace changes
		m = maze;
		cout << &m << " " << &maze << endl;
		heuristicType = type;
		openList.push_back(current);
	}

	~PacMan(){
		// delete parentList;
		// delete closedList;
		// delete openList;
		delete current;
	}

	bool addSquare(int,int);	// change to checkNeighbor
	void scoutDirections();		// change to evaluateCurrentSquare
	Square* getLowestCostSquare();
	void switchCurrentToClosed();		// change to moveToNextSquare; Jace suggests switchCurrentToClosed
	bool inOpenList(Square*);
	bool fin();
	bool solve();
	void pathChange(Square*, int);
	string reconstructPath();
	string pathToString(Square*);
	string mazeToString() { return m.toString(); }
};

bool PacMan::solve() {
	bool found = false;
	while(!this->openList.empty()) {
		switchCurrentToClosed();

		if(!fin()) {
			scoutDirections();
		}
		else {
			found = true;
			break;
		}
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
	return found;
// }

	//insert recontstructPath() here

}

string PacMan::reconstructPath() {
	string path = "";
	Square* target;
	vector<Square*>::iterator it;

	for(it = closedList.begin(); it != closedList.end(); it++) {
		if((*it)->getRow() == this->goal.getRow() && (*it)->getCol() == this->goal.getCol())
			target = *it;
	}

	if(target == NULL)
		return "Path not found.";
	else {
		path = pathToString(target);
		return path;
	}
}

string PacMan::pathToString(Square* t) {
	if(t->getParent() == NULL) return t->toString();
	return pathToString(t->getParent()) + t->toString();
}

// 3.1
// change to checkNeighbor
// add condition if square in closed set and if not in open set
bool PacMan::addSquare(int x, int y) {
	Square sq;
	if(x > m.getLength() || y > m.getWidth() || x < 0 || y < 0) {
		return false;
	}
	else if (m.getSquare(x, y)->getItem() == WALL) {
		return false;
	}
	// to be added/changed in condition ^; uncommented by Jace
	else if(m.getSquare(x,y)->isVisited()) {
		return false;
	}
	else if(!inOpenList(m.getSquare(x,y))) {
		m.setHeuristic(this->heuristicType,this->goal,x,y);
		m.setCumulativeCost(x,y,current->getCumulative()+1);
		m.setFScore(x,y);
		openList.push_back(m.getSquare(x,y));
	}


	// special check for g-score to determine if better path; uncommented by Jace
	int tentative_cumulative_cost = current->getCumulative() + 1;
	if (tentative_cumulative_cost < m.getSquare(x, y)->getCumulative())  {
	//		This path is the best until now. Record it
		m.setParent(x,y,current);
		m.setCumulativeCost(x,y,current->getCumulative()+1);
	}

	return true;
}

void PacMan::pathChange(Square *target, int newCost) {
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getRow() == target->getRow() && (*it)->getCol() == target->getCol()) {
			(*it)->setParent(target);
			(*it)->setCumulative(newCost);
		}
	}
}

//Jace changes
bool PacMan::inOpenList(Square* s) {
	for(int i = 0; i < openList.size(); i++)
		if(openList[i]->getRow() == s->getRow() && openList[i]->getCol() == s->getCol())
			return true;
	return false;
}

// 1.1
// put remove square in openlist
Square* PacMan::getLowestCostSquare() {
	int min = numeric_limits<int>::infinity();

	// supposedly it->getCumulative() + it->getHeuristic()
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getCumulative() < min) {
			min = (*it)->getCumulative();
			this->current = *it;
		}
	}

	return current;
}

// 1
// change to moveToNextSquare
void PacMan::switchCurrentToClosed() {
	closedList.push_back(getLowestCostSquare());
	current = *(closedList.end());
	m.setVisited(current->getRow(), current->getCol());
}

// 3
// change to evaluateCurrentSquare
void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);
}

// 2
bool PacMan::fin() {
	if(current->getRow() == goal.getRow() && current->getCol() == goal.getCol()){
		return true;
	}
	return false;
}

Maze readMazeText(char fileName[]);

int main() {
	// cout<<aStar()<<endl;
	char fileName[100] = "tinyMaze.lay.txt";
	int option;

	cin >> option;
	PacMan p(readMazeText(fileName),option);
	cout << p.mazeToString() << endl;
	// p.solve();
	return 0;
}

Maze readMazeText(char fileName[]) { // Jace changes
	ifstream file(fileName);

	if(!file.is_open()) {
		cout << "File not found." << endl;
		return Maze();
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

	cout<<fileContents<< endl;
	Maze ret(lineCount,lineWidth,fileContents);
	cout << ret.toString() << endl;

	return ret;
}

int computeManhattanDistance(int srcX, int srcY, int destX, int destY) {
	return abs(destX-srcX) + abs(destY-srcY);
}

int computeStraightLineDistance(int srcX, int srcY, int destX, int destY, int cost) {
	return max(abs(destX-srcX),abs(destY-srcY));
}
