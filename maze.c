/*maze.c
	author : FroxThur
	date : 26/03/2017

	this file contain every functions related to the maze.

	In the main function, an exemple show how to use these functions.

	Two main steps are executed : 
		- 1 Maze creation
		- 2 Solving the maze

	*/


/*personal includes*/
#include "includes.h"




int main(int argc, char const *argv[])
{
	/*Needed variables :
		m_data is a boxes' array wich define the maze
		start & exit are the coordinates of the start and the exit of the maze
		m is the maze*/

	box *m_data = NULL;
	coord start, m_exit;
	maze m;


	srand(time(NULL));
	
	/*Grenerate an emty maze and display it*/
	m_data = GennEmptyMaze();
	DisplayMaze(m_data);
	
	/*Configure the start and the exit coordinates*/
	start.x = 0;
	start.y = 2;

	m_exit.x = WIDTH_MAZE - 1;
	m_exit.y = 3;

	/*Generate the maze : connect randomly every boxes...*/
	m = GenerateMaze(m_data, start, m_exit);

	/*... and display the result*/
	DisplayMaze(m.data);


	/*Solve the maze and display it*/
	SolveMazeRec(m, &(m_data[CoordToInd(start)]), NULL);
	DisplayMaze(m.data);

	/*free the memory*/
	free(m.data);

	exit(EXIT_SUCCESS);
}
	


/*Solve the maze recursively*/

int SolveMazeRec(maze m, box* current, box* previous){

	/*result : variable the show if the exit has been found (= 1 if found).
		if the explored way result in a dead end, resut = 0*/
	int result = 0;

	/*display the maze and wait 0.2s*/

		
	/*Condition to terminate the recusive function :*/
	if (current == m.exit){
		/*Either the exit has been found,  the function return 1*/
		current->state = 1;
		/*usleep(20000);*/
		DisplayMaze(m.data);
		return 1;
	}
	if (DeadEnd(current, previous)){
		/*Or they'r is no escapes*/
		return 0;
	}

	/*In every test below, :
		-	current->[Direction] > (box*)1 is equivallent to :
			 current->[Direction] != NULL && current->[Direction] != 1
			 if the direction pointer equal 1, it point to the outside of the maze,
			 at the start box or at the exit one
		-	previous == NULL 	=>	use for the first iteration of the function
		-	current->[Direction] != previous	=>	Do not test the way to the previous box
		-	!result	=>	if result == 1, the way to the exit has been found, it is useless to test other
		possibilities*/


	/*try the north way*/
	if (current->North > (box*)1 && (previous == NULL || current->North != previous)){
		/*printf("North\n");*/
		result = SolveMazeRec(m, current->North, current);
	}

	/*try the south way*/
	if (current->South > (box*)1 && (previous == NULL || current->South != previous) && !result){
		/*printf("South\n");*/
		result = SolveMazeRec(m, current->South, current);
	}
	if (current->East > (box*)1 && (previous == NULL || current->East != previous) && !result){
		/*printf("East\n");*/
		result = SolveMazeRec(m, current->East, current);
	}
	if (current->West > (box*)1 && (previous == NULL || current->West != previous) && !result){
		/*printf("West\n");*/
		result = SolveMazeRec(m, current->West, current);
	}


	/*If result == 1	switch the current box.state at 1 to indicate on the DisplayMaze function the way to the exit*/
	if (result){
		current->state = 1;
		return 1;
	}
	else
		return 0;
}


/*Return 1 if the the current box is a dead end,
	else, return 0*/
int DeadEnd(box* current, box* previous){

	if (current->North != NULL && current->North != previous)
		return 0;
	if (current->South != NULL && current->South != previous)
		return 0;
	if (current->East != NULL && current->East != previous)
		return 0;
	if (current->West != NULL && current->West != previous)
		return 0;

	return 1;
}



/*Generate an empy maze of size : WIDTH_MAZE * HEIGHT_MAZE*/
box* GennEmptyMaze(void){
	box b;

	/*m_data is the maze data pointer*/
	box *m_data;		
	int i;

	/*Allocate memory for the maze*/

	m_data = (box*)malloc(sizeof(box) * WIDTH_MAZE * HEIGHT_MAZE);

	/*initiate an empty box*/
	b.state = 0;
	b.North = b.South = b.East = b.West = NULL;

	/*Copie the empty box all over the maze*/

	for (i = 0; i < (WIDTH_MAZE * HEIGHT_MAZE); ++i)
		m_data[i] = b;
	
	return m_data;
}


/*Connect box(c1) to box(c2)
Return the connected box. 
	c1 : coordinate of the first box
	c2 : box's coordinate we want to connect
*/
coord ConnectBox(box *m_data, coord c1, coord c2){

	/*Compute the direction's coordinate*/
	int x = c2.x - c1.x;
	int y = c2.y - c1.y;

	/*printf("Case1 : X %d, Y, %d\n", c1.x, c1.y);
	printf("Case2 : X %d, Y, %d\n", c2.x, c2.y);*/

	/*Boxes' connections*/

	if (y == -1){		/*Connexion to North box*/
		m_data[CoordToInd(c1)].North = &m_data[CoordToInd(c2)];
		m_data[CoordToInd(c2)].South = &m_data[CoordToInd(c1)];
		/*printf("Connexion au nord\n");*/
		}
	else if (y == 1){ 	/*Connexion to South box*/	
		m_data[CoordToInd(c1)].South = &m_data[CoordToInd(c2)];
		m_data[CoordToInd(c2)].North = &m_data[CoordToInd(c1)];
		/*printf("Connexion au sud\n");*/
	}

	else if (x == 1){	/*Connexion to East box*/
		m_data[CoordToInd(c1)].East = &m_data[CoordToInd(c2)];
		m_data[CoordToInd(c2)].West = &m_data[CoordToInd(c1)];
		/*printf("Connexion à m_data'East\n");*/
	}
	else if (x == -1){	/*Connexion to West box*/
		m_data[CoordToInd(c1)].West = &m_data[CoordToInd(c2)];
		m_data[CoordToInd(c2)].East = &m_data[CoordToInd(c1)];
		/*printf("Connexion à m_data'West\n");*/
	}

	/*change the state of both boxes to "connected"*/
	m_data[CoordToInd(c1)].state = 1;
	m_data[CoordToInd(c2)].state = 1;
	
	/*Display4Boxes(m_data, c1);*/
	return c2;

}

/*debug function that display, if possible, the state of the four boxes arround box(c)*/
void Display4Boxes(box *m_data, coord c){

	/*coordinates computing*/

	coord north;
	coord south;
	coord east;
	coord west;

	north.x = c.x;
	north.y = c.y + 1;

	south.x = c.x;
	south.y = c.y - 1;

	east.x = c.x + 1;
	east.y = c.y;

	west.x = c.x - 1;
	west.y = c.y;


	printf("[Directions]\t");
	if(TestCoord(north))
		printf("North :\t\t\t");
	if(TestCoord(south))
		printf("South\t\t\t");
	if(TestCoord(east))
		printf("east\t\t\t");
	if(TestCoord(west))
		printf("Weast");

	printf("\n");

	printf("[State]\t\t");
	if(TestCoord(north))
		printf("%d\t\t\t", m_data[CoordToInd(north)].state);
	if(TestCoord(south))
		printf("%d\t\t\t", m_data[CoordToInd(south)].state);
	if(TestCoord(east))
		printf("%d\t\t\t", m_data[CoordToInd(east)].state);
	if(TestCoord(west))
		printf("%d", m_data[CoordToInd(west)].state);

	printf("\n");
}

/*Dislay the maze*/
void DisplayMaze(box *m_data){
	/*Clear the teminal*/
	/*printf("\e[2J\e[1;1H");*/
	int i, j;

	/*c is the temporary coordinate the travel all the maze*/
	coord c;

	/*Scann all the maze*/
	for (i = 0; i < HEIGHT_MAZE; i++){
		c.y = i;
		for (j = 0; j < WIDTH_MAZE; j++){
			c.x = j;

			/*If the box is connected to the upper box*/
			if (m_data[CoordToInd(c)].North == NULL)
				printf(" ---");	/*Create a wall*/
			else
				printf("    ");
		}

		printf("\n");


		for (j = 0; j < WIDTH_MAZE; j++){
			c.x = j;

			/*If box(c) is connected to the left box*/
			if (m_data[CoordToInd(c)].West == NULL)
				printf("|");	/*Create a wall*/
			else
				printf(" ");

			/*If state == 1, box(c) is either connected or show the way to the exit*/
			if (m_data[CoordToInd(c)].state)
				printf(" . ");
			else
				printf("   ");

			/*If box(c) is at the right side of the maze*/
			if (j == WIDTH_MAZE - 1){
				/*If box(c) is not the exit*/
				if ((m_data[CoordToInd(c)].East == NULL))	
					printf("|");	/*Create a wall*/
				else
					printf(" ");
			}
		}	

		printf("\n");

		/*If box(c) is at the maze's final row*/
		if (i == HEIGHT_MAZE - 1){
			for (j = 0; j < WIDTH_MAZE; j++){
				c.x = j;
				if (m_data[CoordToInd(c)].South == NULL)
					printf(" ---");	/*Create a wall*/
				else
					printf("    ");
			}

			printf("\n");
		}

	}
	printf("\n");
}


/*Connect to a random box arround init_box
	return the connected box or init_box if it is a dead end*/
coord ConnectRandBox(box *m_data, coord init_box){
	
	/*free_box : number of possible connection*/
	int free_box = 0;

	/*rand_nb : direction randomly picked*/
	int rand_nb;
	
	coord coord_tmp = init_box;


	/*scann connections possibilitys*/
	coord_tmp.y--;	/*Coordinate North*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].North == NULL)
		free_box++;

	coord_tmp.y += 2;	/*Coordinate South*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].South == NULL)
		free_box++;

	coord_tmp.y--;
	coord_tmp.x++;	/*Coordinate East*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].East == NULL)
		free_box++;

	coord_tmp.x -= 2; /*Coordinate West*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].West == NULL)
		free_box++;

	if (free_box == 0){
		/*printf("Connexion impossible (box %d, %d)\n", init_box.x, init_box.y);*/
		return init_box;
	}

	/*Pick a rendom box : generate a random number between 0 and free_box - 1*/
	rand_nb = (rand() % (free_box));

	/*printf("Number of possible connexions : %d\n", free_box);
	printf("Random number : %d\n", rand_nb);*/

	/*Re initiate temporary coordinate*/
	coord_tmp.x = init_box.x; 
	coord_tmp.y = init_box.y;

	
	/*Connecte the box*/

	coord_tmp.y--;	/*Coordinate pNorth*/	
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].North == NULL){
		if (rand_nb == 0)
			return ConnectBox(m_data, init_box, coord_tmp);	
		else
			rand_nb--;
	}

	coord_tmp.y += 2;	/*Coordinate South*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].South == NULL){
		if (rand_nb == 0)
			return ConnectBox(m_data, init_box, coord_tmp);
		else
			rand_nb--;
	}

	coord_tmp.y--;
	coord_tmp.x++;	/*CCoordinate East*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].East == NULL){
		if (rand_nb == 0)
			return ConnectBox(m_data, init_box, coord_tmp);
		else
			rand_nb--;
	}

	coord_tmp.x -= 2; /*Coordinate West*/
	if (TestCoordConn(m_data, coord_tmp) && m_data[CoordToInd(init_box)].West == NULL){
		if (rand_nb == 0)
			return ConnectBox(m_data, init_box, coord_tmp);
	}
			
	
	return init_box;

}

/*Generate a maze, for more details, see README part "functions explanation"*/

maze GenerateMaze(box *m_data, coord start, coord exit){

	coord c1, c2;
	maze result;

	/*check arguments*/
	if (m_data == NULL || !(TestSideBox(start) && TestSideBox(exit)))
		fprintf(stderr, "%s\n", "Arguments error on GenerateMaze");

	/*create & initialize a stack*/
	stack s;
	InitStack(&s);

	/*Push the start box and initialize c1 to the start box*/
	PushStack(&s, c1 = start);

	/*Continue while the stack is not empty*/
	while(!StackEmpty(&s)){

		c2 = ConnectRandBox(m_data, c1);

		/*if c1 != c2, push c2 (newly connected box) in the stack*/ 
		if(CoordComp(c2 ,c1)){
			PushStack(&s, c2);
			c1 = c2;
		}
		/*else, pull c1*/
		else{
			c1 = PullStack(&s);
		}

		/*display the maze and wait 0.2s*/
		DisplayMaze(m_data);
		/*usleep(20000);*/
	}

	InitBoxeStat(m_data);

	/*Fill the maze structure*/
	result.data = m_data;
	result.start = &m_data[CoordToInd(start)];
	result.exit = &m_data[CoordToInd(exit)];
	result.current = NULL;
	result.row = HEIGHT_MAZE;
	result.column = WIDTH_MAZE;


	/*The start box is always on the left side as the exit box is on the right side

	(box*)1 is an invalide pointer used to connect to the outside*/
	m_data[CoordToInd(start)].West = (box*)1;
	m_data[CoordToInd(exit)].East = (box*)1;

	return result;

}

/*Initiate every boxes' state at 0*/
void InitBoxeStat(box *m_data){
	int i;
	for (i = 0; i < (WIDTH_MAZE * HEIGHT_MAZE); ++i)
	{
		m_data[i].state = 0;
	}

}

/*Compare coordinates, return 0 if they match*/
int CoordComp(coord c1, coord c2){

	if (c1.x != c2.x)
		return 1;
	if (c1.y != c2.y)
		return 1;

	return 0;
}

/*Compute the index's array from the box's coordinates*/
int CoordToInd(coord c){
	if (TestCoord(c)){
		return (c.y * WIDTH_MAZE) + c.x;
	}
	else {
		fprintf(stderr, "Index computation impossible\n");
		exit(EXIT_FAILURE);
	}
}

/*Make sure the box is in the maze*/
int TestCoord(coord c){
	if (c.x >= WIDTH_MAZE || c.x < 0 || c.y >= HEIGHT_MAZE || c.y < 0)
		return 0;	/*Coordinates are not in the maze*/
	else
		return 1;	/*Coordinates are in the maze*/
	
}


/*Check the box's coordinate and its state (if the box can be connected)*/
int TestCoordConn(box *m_data, coord c){
	if (TestCoord(c) && m_data[CoordToInd(c)].state == 0)
		return 1;
	else
		return 0;
}



/*Check if the box is on the maze's side*/
int TestSideBox(coord c){

	if (TestCoord(c) == 0)
		return 0;

	if (c.x == 0 || c.x == (WIDTH_MAZE - 1) || c.y == 0 || c.y == (HEIGHT_MAZE - 1))
		return 1;
	else
		return 0;

}
