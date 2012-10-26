/** 
 *  @file	wator.c
 *  @brief	Wator simulation
 */

#include<stdio.h>
#include <time.h>
#include <stdlib.h>

typedef short bool;
#define true 1
#define false 0

#define MAPSIZE 20
#define fBreed 5
#define sBreed 15
#define starveNum 9 

// Function definitions
void createSharks(int pNum, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]);
void createFish(int pNum, int pFishArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]);
void updateFish(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE]);
void updateShark(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]);
void checkFishArraysAndMove(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE],short pFishMoveArray[][MAPSIZE],int xPos, int yPos, int xOffset, int yOffset);
void checkSharkArraysAndMove(int pSharkArray[][MAPSIZE],int pFishArray[][MAPSIZE],short pSharkMoveArray[][MAPSIZE], int pStarveArray[][MAPSIZE],int xPos, int yPos, int xOffset, int yOffset);
int checkFish(int pFishArray[][MAPSIZE],int x, int y);
void print(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE]);

int fishcounter;
int sharkcounter;

/** @brief Initialisation and game loop.
 *
 * Initialises fish and sharks, then enters the game loop.
 * 
 * @param nothing
 * @return nothing
 */
int main()
{
	srand(time(NULL));
	int sharkNum = 10;
	int fishNum = 30;		
	sharkcounter = sharkNum;
	fishcounter = fishNum;
	//Array of ints to represent shark and fish ages. If -1, no fish or shark at this point.
	//If not -1, they are alive.
	int sharks[MAPSIZE][MAPSIZE];
	int fish[MAPSIZE][MAPSIZE];
	short fishmove[MAPSIZE][MAPSIZE];
	short sharkmove[MAPSIZE][MAPSIZE];
	int starve[MAPSIZE][MAPSIZE];
	int framesPerSecond;
	//Create fish and sharks
	createSharks(sharkNum, sharks, starve, sharkmove);
	createFish(fishNum, fish, fishmove,sharks);
	
	//Game loop. Check time against last time world simulated. If greater than 1 second, re-simulate and re-draw world
	bool quit = false;
	//The number of times the scenario has been simulated and displayed.
	short timesSimulated = 0;
	//The last time at which the simulation was run and drawn to screen
	time_t lastTime;
	lastTime = time (NULL);
	short updateNum = 20000;
	float updateTime = 0.1;
	int updateCounter = 0;
	//Game loop (update method)
	//The current time
	time_t thisTime;
	thisTime = time (NULL);
	int fps = 0;
	while (quit == false)
	{
		thisTime = time (NULL);
		if (timesSimulated >= updateNum)
		{
			//Exit from loop if simulation run RUNNUMBER times
			quit = true;
			printf("%d",fps);
		}
		//Update creatures
   		updateFish(fish,sharks,fishmove);
		updateShark(sharks, fish,starve,sharkmove);
		 
		//Prints creatures to console
		print(sharks, fish);
		//Increment counter and reset time counter
		timesSimulated++;
		updateCounter++;
		if (thisTime - lastTime > 1) //If time since last simulation is greater than UPDATETIME, do the simulation
		{	
			fps = (1000/updateCounter);
			lastTime = thisTime;
			updateCounter = 0;
			printf("%d",fps);
		}
	}
	
	return 0;
}

/** @brief Shark Initialisation.
 *
 * Initialises shark properties and creates sharks.
 * 
 * @param pNum Number of sharks to create.
 * @param pSharkArray The container for the sharks.
 * @param pStarveArray Array containing starve values for the sharks.
 * @param pSharkMoveArray Used to store whether or not a shark has moved.
 * @return nothing
 */
void createSharks(int pNum, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]) {
	
	// initialise shark properties
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			pSharkArray[i][i2] = -1;
			pSharkMoveArray[i][i2] = 0;
			pStarveArray[i][i2] = 0;
			if ( i2 == MAPSIZE ) {
				i++;
				i2 = 0;			
			}
		}
	}

	// Create sharks
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % MAPSIZE;
		int r2 = rand() % MAPSIZE;

		if (pSharkArray[r][r2] == -1) //If this position does not already contain a shark
	    {
		  	pSharkArray[r][r2] = 0; 
		  	j++;
		}
	}
}

/** @brief Fish Initialisation.
 *
 * Initialises fish properties and creates fish.
 * 
 * @param pNum Number of fish to create.
 * @param pFishArray The container for the fish.
 * @param pFishMoveArray Used to store whether or not a fish has moved.
 * @param pSharkArray Used to avoid placing a fish on top of a shark.
 * @return nothing
 */
void createFish(int pNum, int pFishArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]) {
	
	// initialise fish properties
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			pFishArray[i][i2] = -1;
			pFishMoveArray[i][i2] = -1;

			if ( i2 == MAPSIZE ) {
				i++;
				i2 = 0;			
			}
		}
	}

	// Create fish
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % MAPSIZE;
		int r2 = rand() % MAPSIZE;

		if (pFishArray[r][r2] == -1 && pSharkArray[r][r2] == -1) //If this position does not already contain a fish or a shark
	    {
		  	pFishArray[r][r2] = rand()%fBreed+1; 
			pFishMoveArray[r][r2] = 0;
		  	j++;
		}
	}
}

/** @brief Print function.
 *
 * Prints the sharks and fish to the console
 *
 * @param pFishArray The container for the fish.
 * @param pSharkArray The container for the sharks.
 * @return nothing
 */
void print(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE]) {

	int k;
	for(k = 0; k < MAPSIZE;) {
		int k2;
		for(k2 = 0; k2 < MAPSIZE; k2++) {
			
			// Print sharks
			if ( pSharkArray[k][k2] != -1) {
			printf("%c", '*');
			}
			// Print fish
			else if ( pFishArray[k][k2] != -1) {
				printf("%c", '.');
			}
			//Output space if no creature at position
			else
			{
				printf("%c", ' ');
			}
		}	
		//Go to next line (row in arrays)
		if ( k2 == MAPSIZE ) {
			printf("\n");
			k++;
			k2 = 0;
		}
	}
	//printf("%d",sharkcounter);
	//printf("\n");
	//printf("%d",fishcounter);
}

/** @brief Print function.
 *
 * Prints the sharks and fish to the console
 *
 * @param pFishArray The container for the fish.
 * @param pSharkArray The container for the sharks.
 * @return nothing
 */
int checkFish(int pFishArray[][MAPSIZE],int x, int y)
{
	if (x >= MAPSIZE)
	{
		x = 0;
	}
	else if (x < 0)
	{
		x = MAPSIZE-1;
	}
	if (y >= MAPSIZE)
	{
		y = 0;
	}
	else if (y<0)
	{
		y = MAPSIZE-1;
	}
	if (pFishArray[x][y] > -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/** @brief Fish update
 *
 * Updates the fishes positions and properties
 *
 * @param pFishArray The container for the fish.
 * @param pSharkArray The container for the sharks.
 * @param pFishMoveArray Used to determine if a fish has been moved to avoid moving it twice in one update.
 * @return nothing
 */
void updateFish(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE]) {
	int g;
	int h;
	for (g = 0; g < MAPSIZE; g++)
	{
		for (h=0; h < MAPSIZE; h++)
		{
			pFishMoveArray[g][h] = 0;
		}
	}
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			if (pFishArray[i][i2] > -1)
			{ 
				//If there is a fish at this position, move
				int r = rand() % 4;
				if (r == 0)
				{
					if (i+1 < MAPSIZE)
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray, i,i2,i+1,i2);
					}
					else
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,0,i2);
					}
				}
				else if (r == 1)
				{
					if (i-1 >= 0)
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,i-1,i2);
					}
					else
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,MAPSIZE-1,i2);
					}
				}
				else if (r == 2)
				{
					if (i2+1 < MAPSIZE)
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,i,i2+1);
					}
					else
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,i,0);
					}	
				}
				else
				{
					if (i2 - 1 >= 0)
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,i,i2-1);
					}
					else
					{
						checkFishArraysAndMove(pFishArray,pSharkArray, pFishMoveArray,i,i2,i,MAPSIZE-1);
					}
				}
			}
			
		}
	}
}

/** @brief Shark update
 *
 * Updates the sharks positions and properties
 *
 * @param pSharkArray The container for the sharks.
 * @param pFishArray The container for the fish.
 * @param pStarveArray Used to determine if a shark has starved to death.
 * @param pSharkMoveArray Used to determine if a fish has been moved to avoid moving it twice in one update.
 * @return nothing
 */
void updateShark(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]) {
	int g;
	int h;
	for (g = 0; g < MAPSIZE; g++)
	{
		for (h=0; h < MAPSIZE; h++)
		{
			pSharkMoveArray[g][h] = 0;
		}
	}
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			if (pSharkArray[i][i2] > -1)
			{
				//If there is a fish to the left of this shark OR (if the left of the shark is the edge, check far right of map)
				//Move the shark to that position and kill the fish
				pSharkArray[i][i2]++;
				if (checkFish(pFishArray,i-1,i2) == 1)
				{
					checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i-1,i2);
				}
				else if (checkFish(pFishArray,i+1,i2) == 1)
				{
					checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i+1,i2);
				}
				else if (checkFish(pFishArray,i,i2-1) == 1)
				{
					checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i,i2-1);
				}
				else if (checkFish(pFishArray,i,i2+1) == 1)
				{
					checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i,i2+1);
				}
				//Else if there is no fish in the adjacent squares, move randomly
				else if (pSharkArray[i][i2] > -1)
				{ 
					int r = rand() % 4;
					if (r == 0)
					{
						if (i+1 < MAPSIZE)
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray,pSharkMoveArray, pStarveArray, i,i2,i+1,i2);
						}
						else
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,0,i2);
						}
					}
					else if (r == 1)
					{
						if (i-1 >= 0)
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i-1,i2);
						}
						else
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray,pSharkMoveArray, pStarveArray,i,i2,MAPSIZE-1,i2);
						}
					}
					else if (r == 2)
					{
						if (i2+1 < MAPSIZE)
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray,pSharkMoveArray, pStarveArray,i,i2,i,i2+1);
						}
						else
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i,0);
						}	
					}
					else
					{
						if (i2 - 1 >= 0)
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i,i2-1);
						}
						else
						{
							checkSharkArraysAndMove(pSharkArray, pFishArray, pSharkMoveArray, pStarveArray,i,i2,i,MAPSIZE-1);
						}
					}
				}
			}
		}
	}
}

/** @brief Fish movement
 *
 * Checks if a grid square is empty and moves the fish into it if it is.
 *
 * @param pFishArray The container for the fish.
 * @param pSharkArray The container for the sharks.
 * @param pFishMoveArray Used to mark a fish that has moved.
 * @param xPos The x coordinate of the fish
 * @param yPos The y coordinate of the fish
 * @param xOffset The x coordinate of the grid square being checked.
 * @param yOffset The y coordinate of the grid square being checked.
 * @return nothing
 */
void checkFishArraysAndMove(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int xPos, int yPos, int xOffset, int yOffset) {
	
	if (pFishArray[xOffset][yOffset] == -1 && pFishMoveArray[xOffset][yOffset] == 0 && pSharkArray[xOffset][yOffset] == -1)
	{
		pFishArray[xOffset][yOffset] = pFishArray[xPos][yPos];
		pFishArray[xPos][yPos]= -1;
		pFishMoveArray[xOffset][yOffset] = 1;
		pFishArray[xOffset][yOffset]++;
		if (pFishArray[xOffset][yOffset] > fBreed)
		{
			pFishArray[xOffset][yOffset] = 0;
			pFishArray[xPos][yPos] = 0;
			fishcounter++;
		}
	}
}

/** @brief Shark movement
 *
 * Moves a shark into the grid square being checked if it is not occupied by another shark
 *
 * @param pSharkArray The container for the sharks.
 * @param pFishArray The container for the fish.
 * @param pSharkMoveArray Used to mark a shark that has moved.
 * @param pStarveArray Used to check if a shark has starved to death.
 * @param xPos The x coordinate of the shark
 * @param yPos The y coordinate of the shark
 * @param xOffset The x coordinate of the grid square being checked.
 * @param yOffset The y coordinate of the grid square being checked.
 * @return nothing
 */
void checkSharkArraysAndMove(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE], int pStarveArray[][MAPSIZE],int xPos, int yPos, int xOffset, int yOffset) {
	
	if (xOffset < 0)
	{
		xOffset = MAPSIZE;
	}
	else if (xOffset == MAPSIZE)
	{
		xOffset = 0;
	}
	if (yOffset < 0)
	{
		yOffset = MAPSIZE;
	}
	else if (yOffset == MAPSIZE)
	{
		yOffset = 0;
	}
	if (pFishArray[xOffset][yOffset] > -1)
	{
		pFishArray[xOffset][yOffset] = -1;
		pStarveArray[xPos][yPos] = 0;
	}
	else
	{
		pStarveArray[xPos][yPos]++;
		//If his starve value is greater than starve, kill shark
		if (pStarveArray[xPos][yPos] >= starveNum)
		{
			pStarveArray[xPos][yPos] = 0;
			pSharkArray[xPos][yPos] = -1;
			sharkcounter--;
			//printf("Shark starved");
		}
	}
	if (pSharkMoveArray[xOffset][yOffset] == 0 && pSharkArray[xOffset][yOffset] == -1)
	{
		pSharkArray[xOffset][yOffset] = pSharkArray[xPos][yPos];
		pStarveArray[xOffset][yOffset] = pStarveArray[xPos][yPos];
		pSharkArray[xPos][yPos]= -1;
		pStarveArray[xPos][yPos] = 0;
		

		if (pSharkArray[xOffset][yOffset] > sBreed)
		{
			//printf("Shark breed");
			sharkcounter++;
			pSharkArray[xOffset][yOffset] = 0;
			pSharkArray[xPos][yPos] = 0;
			//pStarveArray[xPos][yPos] = 0;
		}
	}
}
