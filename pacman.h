#include "maze.h"

class PacMan {
private:
	Maze m;
	int heuristicType;
	// vector<Square> parentList;
	vector<Square*> closedList;
	vector<Square*> openList;
	Square* current;	
public:
	// Pacman(Maze m) {
	//		set start square to openList
	//		initialize goal square?
	// }
	PacMan(Maze maze, int type) { //Jace changes
		m = maze;
		heuristicType = type;
		current = m.getStartingSquare();
		openList.push_back(current);
		current = openList.back();
		current->setCumulative(0);
		current->setHeuristic(type,m.getEndSquare().getCol(),m.getEndSquare().getRow());
		current->setFScore();
	}

	~PacMan(){		
		current = NULL;
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
	//current
	
	bool found = false;
	while(!this->openList.empty()) {
		switchCurrentToClosed();
		// cout << "solve: inf loop" << endl;
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
}

string PacMan::reconstructPath() {
	string path = "";
	Square* target;
	vector<Square*>::iterator it;

	for(it = closedList.begin(); it != closedList.end(); it++) {
		if((*it)->getRow() == this->m.getEndSquare().getRow() && (*it)->getCol() == this->m.getEndSquare().getCol())
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
		m.setHeuristic(this->heuristicType,this->m.getEndSquare(),x,y);
		m.setCumulativeCost(x,y,current->getCumulative()+1);
		m.setFScore(x,y);
		openList.push_back(m.getSquare(x,y));
		cout << "Added to open list" << endl;
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
	cout << "This is min: " << min << endl;
	// supposedly it->getCumulative() + it->getHeuristic()
	vector<Square*>::iterator del;
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		cout << (*it)->getCumulative() << " " << min << endl;
		if((*it)->getCumulative() <= min) {
			min = (*it)->getCumulative();
			del = it;
			this->current = *it;
		}		
		cout << (*it)->getItem() << endl;
		// cout << (*it)->getItem() << endl;
	}
	
	openList.erase(del);
	// cout << " ni ari ka sa lowestCostSquare" << endl;
	return current;
}

void PacMan::switchCurrentToClosed() {
	closedList.push_back(getLowestCostSquare());
	// cout << "did you come here friend ?1" << endl;

	//vector<Square*>::iterator it = openList.back();
	current = closedList.back();
	//cout << "did you come here friend ?2" << endl;

	// cout << current->getItem() << endl;
	//cout << "asa na mn ka ???" << endl;
	m.setVisited(current->getRow(), current->getCol());
	//cout << "did you come here friend ?3" << endl;
	// cout << "Went here." << endl;
}

// 3
// change to evaluateCurrentSquare
void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);
	// cout << "Scout directions executed" << endl;
}

// 2
bool PacMan::fin() {
	if(current->getRow() == m.getEndSquare().getRow() && current->getCol() == m.getEndSquare().getCol()){
		return true;
	}
	// cout << "Fin executed." << endl;
	return false;
}
