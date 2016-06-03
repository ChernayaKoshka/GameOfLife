/*
  Summary:      This program will simulate the popular "Game of Life"
                cellular automata. It will request 4 input from the user through
				the command line. Namely: row count, column count, cell density, and
				a # representing whether they want borders enabled.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

double getRandomFloatInRange(double min, double max);
void displaySimulation(int rows, int columns, char* matrix);
void beginSimulation(int rows, int columns, double density, int boundary);
char* createSimulation(int rows, int columns, int boundary, double density);
char* runSimulation(int rows, int columns, int boundary, char* prevMatrix);
void applyBorders(int rows, int columns, char* matrix);
int countNeighborhood(int rows, int columns, int x, int y, char* matrix);
char* stepSimulation(int rows, int columns, int boundary, char* prevMatrix);

/*
Calculate Tries

name: calcTries

desc: This function will take, parse, and validate the inputs from the user through the
      commandline before starting the game.

params: (1) int argc - The number of symbols in the code
        (2) char* argv[] - The string arguments passed to the program.

returns: An exit code
*/
int main(int argc, char* argv[])
{
  if(argc < 5)
  {
     printf("Usage : ./program <rows> <columns> <density between 0.0 and 1.0> <boundary (0 for no, non-zero for yes)>\n");
     return -1;
  }

  int rows = atoi(argv[1]);
  if(rows <= 0)
  {
    printf("Row count must be greater than zero\n");
    return -1;
  }
  rows+=2; //invisible rows for detection
  
  int columns = atoi(argv[2]);
  if(columns <= 0)
  {
    printf("Column count must be greater than zero\n");
    return -1;
  }
  columns+=2; //ditto

  double density = atof(argv[3]);
  if(density <= 0 || density >= 1.0)
  {
    printf("Density must be a decimal between 0 and 1\n");
    return -1;
  }
  
  int boundary = atof(argv[4]);
  if(boundary < 0)
  {
    printf("Density must be greater than 0\n");
    return -1;
  }
  
  srand(time(NULL)); //seed random
  //start the fun
  beginSimulation(rows, columns, density, boundary);
  
  return 0;  
}

/*
Get Random Float In Range

name: getRandomFloatInRange

desc: This function will generate a random floating point number between
      the min and max

params: (1) int min - The minimum size of the floating point number
        (2) int max - The maximum size of the floating point number

returns: An integer representing the number of tries allowed to the user
*/
double getRandomFloatInRange(double min, double max)
{
  double frand = (((double)rand()/RAND_MAX)*(max-min))+min;
  return frand;
}

/*
Display Simulation

name: displaySimulation

desc: This function will generate a random floating point number between
      the min and max

params: (1) int row - The # of rows
        (2) int columns - The # of columns
		(3) char* matrix - the matrix to display to the user

returns: Nothing
*/
void displaySimulation(int rows, int columns, char* matrix)
{
  static int stepNumber = 0;
  stepNumber++;
  
  printf("\n\n\n\n\n\n"); //whitespace for formatting
  for(int y = 1; y < rows-1; y++)
  {
    for(int x = 1; x < columns-1; x++)
    {
      printf("%c ",*(matrix+(y*columns+x)));
    }
    printf("\n");
  }
  
  printf("Step # : %d\n", stepNumber);
}

/*
Create Simulation

name: createSimulation

desc: This function will generate and populate a matrix with cells.

params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) int boundary - A boolean representing whether or not to use a boundary
		(4) double density - A double representing the % of cells to be alive

returns: A pointer to a string of characters representing the cell matrix
*/
char* createSimulation(int rows, int columns, int boundary, double density)
{
  char* matrix = malloc(rows*columns);
  
  if(matrix == NULL)
	  return NULL;
  
  for(int y = 0; y < rows; y++)
	  for(int x = 0; x < columns; x++)
		  *(matrix+(y*columns+x)) = 'Z';
  
  for(int y = 1; y < rows-1; y++)
  {
    for(int x = 1; x < columns-1; x++)
    {
      if(getRandomFloatInRange(0.0, 1.0) <= density)
      {
        *(matrix+(y*columns+x)) = '#'; //manual indexing without 2d-array syntactic-sugar :(
      }
      else
      {
        *(matrix+(y*columns+x)) = '.'; //ditto :(
      }
    }
  }
  
  if(boundary) applyBorders(rows, columns, matrix);
	
  return matrix;
}

/*
Begin Simulation

name: beginSimulation

desc: This function will begin the simulation, entering a permanent loop and "stepping"
      the simulation every second.

params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) double density - A double representing the % of cells to be alive
		(4) int boundary - A boolean representing whether or not to use a boundary

returns: A pointer to a string of characters representing the cell matrix
*/
void beginSimulation(int rows, int columns, double density, int boundary)
{
  char* sim = createSimulation(rows, columns, boundary, density);
  
  if(sim == NULL)
  {
	  printf("Unable to allocate memory!\n");
	  return;
  }
  
  displaySimulation(rows, columns, sim);
  while(1)
  {
    sim = stepSimulation(rows, columns, boundary, sim);
	
	  if(sim == NULL)
      {
	     printf("Unable to allocate memory!\n");
	     return;
      }
	
    displaySimulation(rows, columns, sim);
    usleep(1000000); //2 seconds in microseconds
  }
}

/*
Apply Borders

name: applyBorders

desc: This function will begin apply cell boarders, copying the visible row into
      an invisible one to be used for calculations. These rows will not be treated
	  as though they are alive themselves.

params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3 - VOLATILE) char* matrix - the matrix to apply borders to.

returns: Nothing, but will modify the provided array

author note: I'm really sorry, I didn't see your heap-matrix thing until it was way too late.
             so everything is indexed doing the math manually.
*/
void applyBorders(int rows, int columns, char* matrix)
{
	//top/bottom
	for(int x = 1; x < columns-1; x++)
	{
		*(matrix+x) = *(matrix+x+columns*(rows-2));
		*(matrix+x+columns*(rows-1))=*(matrix+x+columns);
	}	
	
	//sides
	for(int y = 1; y < rows-1;y++)
	{
		*(matrix+y*columns) = *(matrix+y*columns+columns-2);
		*(matrix+y*columns+columns-1) = *(matrix+y*columns+1);
	}
	
	//top left corner
	*(matrix+0) = *(matrix+columns*(rows-2)+columns-2);
	//top right corner
	*(matrix+columns-1) = *(matrix+columns*(rows-2)+1);
	//bottom left corner
	*(matrix+columns*(rows-1)) = *(matrix+columns*2-2);
	//bottom right corner
	*(matrix+columns*(rows-1)+columns-1) = *(matrix+columns+1);
	
}

/*
Counter Neighborhood

name: countNeighborhood

desc: This function will count all neighboring cells that are 'alive'

params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) int x - The column # of the cell
		(4) int y - The row # of the cell
		(5) char* matrix - The matrix provided will be searched for neighoring
		                   cells.

returns: An integer representing the number of alive cells (including the main cell)
*/
int countNeighborhood(int rows, int columns, int x, int y, char* matrix)
{
	int count = 0;
	
	int pivot = ((y)*columns+x);
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			char c = (*(matrix+pivot+(i*columns)+j));
				
			if(c == '#') count++;
		}
	}
	return count;
}

/*
Step Simulation

name: stepSimulation

desc: This function will "step" the simulation according to the game's rules

params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) int boundary - A boolean variable representing whether or not a
		                   boundary should be applied.
		(4) char* prevMatrix - The previous matrix that will be used to calculate the new matrix.

returns: A pointer to the stepped matrix.
*/
char* stepSimulation(int rows, int columns, int boundary, char* prevMatrix)
{
  char* steppedMatrix = malloc(rows*columns);
  if(steppedMatrix == NULL)
	  return NULL;
  
  for(int y = 1; y < rows-1; y++)
  {
    for(int x = 1; x < columns-1; x++)
    {
	  int live = countNeighborhood(rows,columns,x,y,prevMatrix);
	  char cell = *(prevMatrix+(y*columns+x));
	  if(cell == '#') live--;  //avoid counting self
	  
	  *(steppedMatrix+(y*columns+x)) = cell; //just making sure
	  
      if(live < 2)
	  {
		  *(steppedMatrix+(y*columns+x)) = '.'; //manual indexing again
	  }
	  else if(live > 3)
	  {
		  *(steppedMatrix+(y*columns+x)) = '.';
	  }
	  else if((live == 2 || live == 3) && cell == '#')
	  {
		  *(steppedMatrix+(y*columns+x)) = '#';
	  }
	  else if(live == 3 && cell == '.')
	  {
		  *(steppedMatrix+(y*columns+x)) = '#';
	  }
	  else if(live == 2)
	  {
		  //do nothing
	  }
    }
  }
  free(prevMatrix); //free up previous matrix
  if(boundary) applyBorders(rows, columns, steppedMatrix); //apply border
  return steppedMatrix;
}