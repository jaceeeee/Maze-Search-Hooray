	#include "maze.h"

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
public:
	
	// set start square to openList
	// initialize goal square? NO NEED.
	PacMan(Maze *maze, int type) { //Jace changes
		m = maze;

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
	void switchCurrentToClosed();			// Refactor Comments: change to moveToNextSquare; Jace suggests switchCurrentToClosed
	bool inOpenList(Square*);
	bool fin();
	bool solve();
	void pathChange(Square*, int);
	string reconstructPath();
	string pathToString(Square*);
	string mazeToString() { return m->toString(); }
	int selectClosestGoal();
	void printStatistics();
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
	return found;
}

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

void PacMan::pathChange(Square *target, int newCost) {
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getRow() == target->getRow() && (*it)->getCol() == target->getCol()) {
			(*it)->setParent(target);
			(*it)->setCumulative(newCost);
		}
	}
}

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

// 2
bool PacMan::fin() {
	if(current->getRow() == m->getEndSquare().getRow() && current->getCol() == m->getEndSquare().getCol()) 
		return true;	
	return false;
}

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
}

void PacMan::printStatistics() {	
	cout << "Path: " << reconstructPath() << endl;	
	cout << "Path cost: " << cost << endl;
	cout << "Expanded Nodes: " << closedList.size() << endl;
	cout << "Frontier Size: " << frontierSize << endl;
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
		Add adjacent Squares
			if not yet in openList
				Set g, h, and f costs
				set current as parent
				push back to openList

	new attribs: 
		currentGoal
		goalArray

	new methods:
		

*/