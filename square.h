using namespace std;

int computeManhattanDistance(int, int, int, int);
int computeStraightLineDistance(int, int, int, int, int);

enum {
	WALL = '%',
	OPENPATH = ' ',
	START = 'P',
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
		this->cumulativeCost = numeric_limits<int>::infinity();
	}

	~Square() {
		parent = NULL;
		delete parent;
	}

	bool isPassable() { return (content == WALL) ? false : true; }

	void setParent(Square *sq) { parent = sq; }
	Square* getParent() { return parent; }
	bool isVisited() { return visited; }

	// untackled suggestion:
	// change to: ?
	// bool isVisisted(Vector<Square> closedList) {
	// return find(closedList.begin(), closedList.end(), this)
	// }
	// reason: redundancy

	char getItem() { return content; }
	int getRow() { return row; }
	int getCol() { return col; }
	int getHeuristic() { return heuristic; }
	int getCumulative() { return cumulativeCost; }
	int getFScore() { return fScore; }

	void setVisited() { visited = (!visited) ? true : false; }
	void setSquare(int,int);
	void setCumulative(int newCost) { this->cumulativeCost = newCost; }
	void setFScore() { fScore = cumulativeCost + heuristic; }
	void setHeuristic(int,int,int);

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
};

void Square::setSquare(int h_val, int g_val) {
	heuristic = h_val;
	cumulativeCost = g_val;
}

int Square::setHeuristic(int type, int destX, int destY) {
	if(type == MD) {
		heuristic = computeManhattanDistance(this->row, this->col, destX, destY);
		return heuristic;
	}
	else if(type == SLD) {
		heuristic = computeStraightLineDistance(this->row, this->col, destX, destY, 1);
		return heuristic;
	}
	else {
		cout << "Heuristic type not valid. Resend arguments." << endl;
		return -1; //if valid
		}
}

// produce heuristics

int computeManhattanDistance(int srcX, int srcY, int destX, int destY) {
	return abs(destX-srcX) + abs(destY-srcY);
}

int computeStraightLineDistance(int srcX, int srcY, int destX, int destY, int cost) {
	return max(abs(destX-srcX),abs(destY-srcY));
}
