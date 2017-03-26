
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#define WIDTH_MAZE	8
#define HEIGHT_MAZE	5
#define S_STACK	(WIDTH_MAZE * HEIGHT_MAZE)

/*state = :
		0 the box is not connected
		1 the box is connected in the creation of the labyrinth,
			in the solving part, 1 is used to show the way to the exit
		-1 if error box (used for error report) */

typedef struct box {
	int state;	
	struct box *North;
	struct box *South;
	struct box *East;
	struct box *West;
}box;



typedef struct {
	int row;
	int column;
	box *data;
	box *start;
	box *exit;
	box *current;
}maze;

typedef struct {
	int x;
	int y;
} coord;

typedef coord stackElmt;

typedef struct {
	stackElmt ElemArr[S_STACK];
	int top;
} stack;



int SolveLabyRec(maze m_data, box* current, box* previous);
int DeadEnd(box* current, box* previous);
box* GennEmptyLaby(void);
coord ConnectBox(box *m_data, coord c1, coord c2);
void Display4Boxes(box *m_data, coord c);
void DisplayMaze(box *m_data);
coord ConnectRandBox(box *m_data, coord init_box);
maze GenerateLaby(box *m_data, coord start, coord exit);
void InitBoxeStat(box *m_data);
int CoordComp(coord c1, coord c2);
int CoordToInd(coord c);
int TestCoord(coord c);
int TestCoordConn(box *m_data, coord c);
int TestSideBox(coord c);


void InitStack (stack *s);
int StackEmpty(stack *s);
int StackFull(stack *s);
void PushStack(stack *s, stackElmt e);
stackElmt PullStack(stack *p);
