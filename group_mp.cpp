#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum {
	WALL = '%',
	OPENPATH = ' ',
	START = 'p',
	END = '.',
	MD = 1,
	SLD = 2
};
class Pos {
private:
	int row, col;
}


class Square {
private:
	char content;
	int row, col;
	bool visited;
	int cumulativeCost;
	int heuristic;	
	Square *parent;
public:
	Square() {
		row = col = 0;
		visited = false;
		item = 0;
	}
	Square(int row, int col,char item) {
		this->item = item;
		this->row = row;
		this->col = col;
		visited = false;
	}
	bool isPassable() { return (item == WALL) ? false : true; }
	bool isVisited() { return visited; }
	char getItem() { return item; }
	int getRow() { return row; }
	int getCol() { return col; }
	void setVisited(){ visited = (!visited) ? true : false; }
	void setSquare(int,int,int,int,char);
	int getHeuristic();
	int getCumulative();
	Square operator=(Square);
};

int getHeuristic(int type) {
	if(type == MD) {

	} 
	else {

	}
}

int getCumulative() {

}

// heuristic,cumulative,total costs
void Square::setSquare() {

}

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
	Square getSquare(int,int);
	void setVisited(int,int);
	void setPath(int,int);
	int getLength() { return length; }
	int getWidth() { return width; }
	string toString();
};

class PacMan {
	vector<Square> parentList;
	vector<Square> closedList;
	vector<Square> openList;


};

int main() {
	return 0;
}