#include "maze.h"
#include <cstring>

Maze* readMazeText(char[]);

class PacMan {
private:
	Maze *m;
	Square* current;	

	vector<Square*> closedList;
	vector<Square*> openList;

	vector<Square*> goalArray;
	Square* currentGoal;

	int heuristicType;	
	int cost;
	int frontierSize;

	bool mazeFound;
public:
	
	// set start square to openList
	// initialize goal square? NO NEED.
	PacMan(Maze *maze, int type) { //Jace changes
		m = maze;
		if(m != NULL){
			current = m->getStartingSquare();		
			current->setCumulative(0);
			current->setHeuristic(type,m->getEndSquare().getCol(),m->getEndSquare().getRow());
			current->setFScore();
			
			openList.push_back(current);
			for(int i = 0; i < maze->getLength(); i++) {
				for(int j = 0; j < maze->getWidth(); j++) {
					if(maze->getSquare(i,j)->getItem() == END) {
						goalArray.push_back(m->getSquare(i,j));					
					}
				}
			}

			heuristicType = type;
			frontierSize = cost = 0;
			mazeFound = true;
		} else { mazeFound = false; }
	}

	~PacMan(){
		m = NULL;
		currentGoal = current = NULL;
		delete m;
		delete current;
		delete currentGoal;
	}

	bool addSquare(int,int);				// Refactor Comments: change to checkNeighbor
	void scoutDirections();					// Refactor Comments: change to evaluateCurrentSquare
	Square* getLowestCostSquare();
	bool inStartState() { return mazeFound; }
	void switchCurrentToClosed();			// Refactor Comments: change to moveToNextSquare; Jace suggests switchCurrentToClosed
	bool inOpenList(Square*);
	bool fin();
	bool solve();
	void pathChange(Square*, int);
	void setCurrentGoal();
	string reconstructPath();
	string pathToString(Square*);
	string mazeToString() { return m->toString(); }
	int selectClosestGoal();
	void printStatistics();	
};


string PacMan::reconstructPath() {
	string path = "";
	Square* target;
	vector<Square*>::iterator it;

	for(it = closedList.begin(); it != closedList.end(); it++) {
		if((*it)->getRow() == this->m->getEndSquare().getRow() && (*it)->getCol() == this->m->getEndSquare().getCol())
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
	cost++;
	if(t->getParent() == NULL) return t->toString();
	return (pathToString(t->getParent()) + ", " + t->toString());
}


// 3.1
// change to checkNeighbor
// add condition if square in closed set and if not in open set
bool PacMan::addSquare(int row, int col) {
	Square* sq = m->getSquare(row,col);
	if(row > m->getLength() || col > m->getWidth() || row < 0 || col < 0) {
		return false;
	}
	else if (m->getSquare(row, col)->getItem() == WALL) {
		return false;
	}	
	else if(m->getSquare(row,col)->isVisited()) {
		return false;
	}
	else if(!inOpenList(m->getSquare(row,col))) {
		Square* pushed_sq = new Square(sq->getRow(), sq->getCol(), sq->getItem());
		pushed_sq->setParent(current);
		pushed_sq->setHeuristic(this->heuristicType,row,col);
		pushed_sq->setCumulative(current->getCumulative()+1);
		pushed_sq->setFScore();
		openList.push_back(pushed_sq);		

		frontierSize++;
	}
	// special check for g-score to determine if better path; uncommented by Jace
	int tentative_cumulative_cost = current->getCumulative() + 1;
	if (tentative_cumulative_cost < m->getSquare(row, col)->getCumulative())  {
	//		This path is the best until now. Record it
		m->setParent(row,col,current);
		m->setCumulativeCost(row,col,current->getCumulative()+1);
	}
	return true;
}

// sets the new parent and cost of a path square which had computed better costs
void PacMan::pathChange(Square *target, int newCost) {
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getRow() == target->getRow() && (*it)->getCol() == target->getCol()) {
			(*it)->setParent(target);
			(*it)->setCumulative(newCost);
		}
	}
}

// checks to see if certain square is already in the open list
bool PacMan::inOpenList(Square* s) {
	for(int i = 0; i < openList.size(); i++)
		if(openList[i]->getRow() == s->getRow() && openList[i]->getCol() == s->getCol())
			return true;
	return false;
}

// 1.1
// put remove square in openlist
Square* PacMan::getLowestCostSquare() {
	int min = 100000000;	
	vector<Square*>::iterator del;

	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {		
		if((*it)->getCumulative() <= min) {
			min = (*it)->getCumulative();
			del = it;
			this->current = *it;
		}				
	}
	
	openList.erase(del);

	return current;
}

void PacMan::switchCurrentToClosed() {	
	closedList.push_back(getLowestCostSquare());
	current = closedList.back();	
	m->setVisited(current->getRow(), current->getCol());	
}

// 3
// change to evaluateCurrentSquare
void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);	
}

// checks if goal is met
bool PacMan::fin() {
	if(current->getRow() == m->getEndSquare().getRow() && current->getCol() == m->getEndSquare().getCol()) 
		return true;	
	return false;
}

// returns an index of the currently closer goal
int PacMan::selectClosestGoal() {
	int min = 0, pos = 0;
	for(vector<Square*>::iterator it = this->goalArray.begin(); it != this->goalArray.end(); it++) {
		int sourceX = current->getRow();
		int sourceY = current->getCol();
		int destX = (*it)->getRow();
		int destY = (*it)->getCol();
		int distance = (heuristicType == MD) ? computeManhattanDistance(sourceX, sourceY, destX, destY) :
									  computeStraightLineDistance(sourceX, sourceY, destX, destY);

		if(distance <= goalArray[min]->getHeuristic())
			min = pos;
		pos++;
	}

	return pos;
}

// prints statistics of maze search
void PacMan::printStatistics() {	
	cout << "Path: " << reconstructPath() << endl;	
	cout << "Path cost: " << cost << endl;
	cout << "Expanded Nodes: " << closedList.size() << endl;
	cout << "Frontier Size: " << frontierSize << endl;
}


void PacMan::setCurrentGoal() {
	currentGoal = goalArray[selectClosestGoal()];
}

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
	return found;
}

Maze* readMazeText(char fileName[]) { // Jace changes
	string path = "mazes/";
	char *pathToFile = new char[path.length()+strlen(fileName)+1];

	strcpy(pathToFile,path.c_str());
	strcat(pathToFile,fileName);

	ifstream file(pathToFile);

	if(!file.is_open()) {
		cout << "File not found." << endl;
		return NULL;
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

// pseudocode for part 2

/* 

	initialize current square to open list
	while openList is not empty
		In array of goal squares
			Look for the lowest cost goal square and set as a currentGoal
		Add current square to closedList
		See if current is goal
			if goal, see if all goal squares have been visited
				if yes, return true
				else, refresh board
		Add adjacent "path" Squares
			if not yet in openList
				Set g, h, and f costs
				set current as parent
				push back to openList
			else if already in openList
				compute and set new costs
				if better than prev
					set parent to current


	new attribs: 
		currentGoal
		goalArray

	new methods:
		bool goalChecker() 

		

*/

