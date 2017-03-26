/*Needed library*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*Personal defines : you can change the height & width here*/
#define WIDTH_MAZE	8
#define HEIGHT_MAZE	5
#define S_STACK	(WIDTH_MAZE * HEIGHT_MAZE)

/*Personal structure*/

/*	state :
		0 the box is not connected
		1 the box is connected in the creation of the labyrinth,
			in the solving part, 1 is used to show the way to the exit
		-1 if error box (used for error report) 
	box *North :
		pointer to the north box structure
	.
	.
	.
	box *West :
		pointer to the west box structure

	When a connection is made between two boxes, each boxes have the adress of the other one in their structure
	Exemple : Box(a) connect to  Box(b) and Box(b) is the upper box :
	Before connection :
	 --- --- --- --- --- --- --- ---
	|   |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---
	| b |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---
	| a |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---
	|   |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---

	After connection

	 --- --- --- --- --- --- --- ---
	|   |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---
	| b |   |   |   |   |   |   |   |
	     --- --- --- --- --- --- ---
	| a |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---
	|   |   |   |   |   |   |   |   |
	 --- --- --- --- --- --- --- ---

	In the two boxes structure :
	 box(a).north = adresse of box(b)
	 box(b).south = adresse of box(a)*/

typedef struct box {
	int state;	
	struct box *North;
	struct box *South;
	struct box *East;
	struct box *West;
}box;


/*Maze structure :
	row : amount of maze's rows 
	column : amount of maze's collumns
	*data : pointer to the boxes' array (maze's data)
	*start : pointer to the start box
	*exit : pointer to the exit box
	*current : current box => used for solving the maze*/

typedef struct {
	int row;
	int column;
	box *data;
	box *start;
	box *exit;
	box *current;
}maze;

/*Coordinate of one box :
	x : column number
	y : row number
	the origin is set in the upper left corner*/

typedef struct {
	int x;
	int y;
} coord;

/*The stack is used for coordinates*/
typedef coord stackElmt;

/*Stack structure*/
typedef struct {
	stackElmt ElemArr[S_STACK];
	int top;
} stack;


/*maez.c function declaration*/
int SolveMazeRec(maze m_data, box* current, box* previous);
int DeadEnd(box* current, box* previous);
box* GennEmptyMaze(void);
coord ConnectBox(box *m_data, coord c1, coord c2);
void Display4Boxes(box *m_data, coord c);
void DisplayMaze(box *m_data);
coord ConnectRandBox(box *m_data, coord init_box);
maze GenerateMaze(box *m_data, coord start, coord exit);
void InitBoxeStat(box *m_data);
int CoordComp(coord c1, coord c2);
int CoordToInd(coord c);
int TestCoord(coord c);
int TestCoordConn(box *m_data, coord c);
int TestSideBox(coord c);

/*LIFO fucntion declaration*/
void InitStack (stack *s);
int StackEmpty(stack *s);
int StackFull(stack *s);
void PushStack(stack *s, stackElmt e);
stackElmt PullStack(stack *p);
