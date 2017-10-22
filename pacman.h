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

	int goalCount; 
public:
	
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
						Square *pushed_goal = new Square(i,j,END);
						// cout << "inside goalArrayloop" << endl;
						// cout << "Printing i and j... " << i << " " << j << endl;
						goalArray.push_back(pushed_goal);					
					}
				}
			}

			cout << "printing maze contents: " << goalArray[0]->getItem() << endl;
			heuristicType = type;
			goalCount = frontierSize = cost = 0;
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

	bool addSquare(int,int);				
	void scoutDirections();					
	Square* getLowestCostSquare();
	bool inStartState() { return mazeFound; }
	void switchCurrentToClosed();			
	bool inOpenList(Square*);
	bool foundOneGoal();
	bool solve();
	void initializeSquareValues();
	void pathChange(Square*, int);
	void setCurrentGoal(); //new	
	string reconstructPath();
	string pathToString(Square*);
	string mazeToString() { return m->toString(); }
	int selectClosestGoal();//new
	void printStatistics();	
	void refresh();
	bool mazeCompleted() { cout << goalCount << " " << goalArray.size() << endl;
		return ((goalCount == goalArray.size()) ? true : false); }
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

// sets the new parent and cost of a path square which had computed better costs
// not used yet... check immediately previous if statement above
void PacMan::pathChange(Square *target, int newCost) {
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getRow() == target->getRow() && (*it)->getCol() == target->getCol()) {
			(*it)->setParent(target);
			(*it)->setCumulative(newCost);
		}
	}
}

// returns an index of the currently closer goal
int PacMan::selectClosestGoal() {
	int min = 0, pos = 0;
	for(vector<Square*>::iterator it = this->goalArray.begin(); it != this->goalArray.end(); it++) {
		if(!(*it)->isVisited()){
			int sourceX = current->getRow();
			int sourceY = current->getCol();
			int destX = (*it)->getRow();
			int destY = (*it)->getCol();
			int distance = (heuristicType == MD) ? computeManhattanDistance(sourceX, sourceY, destX, destY) :
										  computeStraightLineDistance(sourceX, sourceY, destX, destY);

			if(distance <= goalArray[min]->getHeuristic())
				min = pos;
		}
		pos++;			
	}

	return min;
}

// prints statistics of maze search
void PacMan::printStatistics() {	
	cout << "Path: " << reconstructPath() << endl;	
	cout << "Path cost: " << cost << endl;
	cout << "Expanded Nodes: " << closedList.size() << endl;
	cout << "Frontier Size: " << frontierSize << endl;
}

void PacMan::refresh() {
	for(int i = 0; i < m->getLength(); i++) {
		for(int j = 0; j < m->getWidth(); j++) {
			if(m->getSquare(i,j)->getItem() != END)
				m->setVisited(i,j,false);
		}
	}
}

// current goal is set from the closest unvisited goal square in goalArray
void PacMan::setCurrentGoal() {	
	Square *g = goalArray[selectClosestGoal()];
	currentGoal = new Square(g->getRow(), g->getCol(), g->getItem());
	cout << "curentGoal coordinates" << currentGoal->getRow() << " " << currentGoal->getCol() << " " << currentGoal->getItem() << endl;
}

bool PacMan::solve() {		
	bool solved = false;
	
	for(vector<Square*>::iterator it = goalArray.begin(); it!=goalArray.end(); it++){
		cout << "goal Array contents" << (*it)->getRow() << " " << (*it)->getCol() << " " << (*it)->getItem()<< endl;
	}

	initializeSquareValues();

	while(!this->openList.empty()) {
		setCurrentGoal();			// sets current goal square; only if everytime a goal is found?
		cout << "current coordinates: ";
		cout << current->getRow() << " " << current->getCol() << endl;
		cout << "currentGoal coordinates: ";
		cout << currentGoal->getRow() << " " << currentGoal->getCol() << endl;
		// if(currentGoal != NULL) cout << currentGoal->getRow() << currentGoal->getCol() << endl;
		switchCurrentToClosed();		
		if(!foundOneGoal()) {
			scoutDirections();
		}
		else {
			cout << "FOUND A GOAL" << endl;
			if(mazeCompleted()){
				solved = true;
				break;
			} else {
				refresh();
			}

		}
	}
	return solved;
}

void Pacman::initializeSquareValues() {
	for(int i = 0; i < m->getLength(); i++) {
		for(int j = 0; j < m->getWidth(); j++) {
			m->getSquare(i,j)->setHeuristic(heuristicType, currentGoal->getRow(), currentGoal->getCol());
			m->getSquare(i,j)->setFScore();
		}
	}
}

void PacMan::switchCurrentToClosed() {	
	closedList.push_back(getLowestCostSquare());
	current = closedList.back();	
	m->setVisited(current->getRow(), current->getCol());	
}

Square* PacMan::getLowestCostSquare() {
	int min = 1000000000;	
	vector<Square*>::iterator del;

	//lowest f-score
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {		
		if((*it)->getFScore() <= min) {
			min = (*it)->getFScore();
			del = it;
			this->current = *it;
		}				
	}
	
	openList.erase(del);

	return current;
}

// checks if goal is met, and increments goalCounter as side effect
bool PacMan::foundOneGoal() {	
	if(current->getRow() == currentGoal->getRow() && current->getCol() == currentGoal->getCol()) {		
		goalCount++;
		return true;		
	}
	return false;
}

void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);	
}

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
		//pushed_sq->setParent(current); // no need if fScore comparison is implemented in open list
		//pushed_sq->setHeuristic(this->heuristicType,row,col); // if fScore is used for open list comparison heuristics need to be pre-set
		//pushed_sq->setCumulative(current->getCumulative()+1); // no need if fScore comparison is implemented in open list
		//pushed_sq->setFScore(); // no need if fScore comparison is implemented in open list
		openList.push_back(pushed_sq);		

		frontierSize++;
	}
	// special check for g-score to determine if better path; uncommented by Jace
	int tentative_cumulative_cost = current->getCumulative() + 1;
	if (tentative_cumulative_cost < m->getSquare(row, col)->getCumulative())  {
	//		This path is the best until now. Record it
		m->setParent(row,col,current);
		m->setCumulativeCost(row,col,current->getCumulative()+1);
		//m->setFScore(); --> Update fScore; Needed for the fScore comparison in open list
	}
	return true;
}

// checks to see if certain square is already in the open list
bool PacMan::inOpenList(Square* s) {
	for(int i = 0; i < openList.size(); i++)
		if(openList[i]->getRow() == s->getRow() && openList[i]->getCol() == s->getCol())
			return true;
	return false;
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
		goalCount

	new methods:
		bool mazeCompleted() 
		void refresh()
		int selectClosestGoal()		
		void setCurrentGoal()

		

*/
