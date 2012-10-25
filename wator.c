#include<stdio.h>
#include <time.h>
#include <stdlib.h>

typedef short bool;
#define true 1
#define false 0

#define MAPSIZE 20
#define fBreed 5
#define sBreed 10
#define starveNum 5
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
main()
{
	srand(time(NULL));
	int sharkNum = 10;
	int fishNum = 50;		
	sharkcounter = sharkNum;
	fishcounter = fishNum;
	//Array of ints to represent shark and fish ages. If -1, no fish or shark at this point.
	//If not -1, they are alive.
	int sharks[MAPSIZE][MAPSIZE];
	int fish[MAPSIZE][MAPSIZE];
	short fishmove[MAPSIZE][MAPSIZE];
	short sharkmove[MAPSIZE][MAPSIZE];
	int starve[MAPSIZE][MAPSIZE];

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
	short updateNum = 20;
	short updateTime = 1;

	//Game loop (update method)
	while (quit == false)
	{
		//The current time
		time_t thisTime;
  		thisTime = time (NULL);
		

		if (timesSimulated == updateNum)
		{
			//Exit from loop if simulation run RUNNUMBER times
			quit = true;
		}
		if (thisTime - lastTime > updateTime) //If time since last simulation is greater than UPDATETIME, do the simulation
		{
		    updateShark(sharks, fish,starve,sharkmove);
		    updateFish(fish,sharks,fishmove);
			//Prints creatures to console
			print(sharks, fish);
			
			//Increment counter and reset time counter
			timesSimulated++;
			lastTime = thisTime;
		}
	}

}

//Initialises shark properties and creates sharks
void createSharks(int pNum, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]) {
	

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

//Creates fish
void createFish(int pNum, int pFishArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]) {
	
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

//Prints the positions of creatures to the console
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
	printf("%d",sharkcounter);
	printf("\n");
	printf("%d",fishcounter);
}
int checkFish(int pFishArray[][MAPSIZE],int x, int y)
{
	if (x >= MAPSIZE)
	{
		x = 0;
	}
	else if (x < 0)
	{
		x = MAPSIZE;
	}
	if (y >= MAPSIZE)
	{
		y = 0;
	}
	else if (y<0)
	{
		y = MAPSIZE;
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
void updateFish(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE]) {
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			pFishMoveArray[i][i2] = 0;
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

void updateShark(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]) {
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			//pSharkMoveArray[i][i2] = 0;
			//If there is a fish to the left of this shark OR (if the left of the shark is the edge, check far right of map)
			//Move the shark to that position and kill the fish
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
			/*if (i-1 >= 0)
			{
				if (pFishArray[i-1][i2] > -1)// ||(i-1 < 0 && pFishArray[MAPSIZE-1][i2] > -1))
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i-1,i2);
					pFishArray[i-1][i2] = -1;
					fishcounter--;
					printf("Fish eaten");
					pStarveArray[i-1][i2] = 0;
				}
			}
			else if (i-1 < 0)
			{
				if (pFishArray[MAPSIZE-1][i2] > -1)// ||(i-1 < 0 && pFishArray[MAPSIZE-1][i2] > -1))
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,MAPSIZE-1,i2);
					pFishArray[MAPSIZE-1][i2] = -1;
					fishcounter--;
					pStarveArray[MAPSIZE-1][i2] = 0;
				}
			}
			
			else if (pFishArray[i+1][i2] > -1 ||(i+1 == MAPSIZE && pFishArray[0][i2] > -1))
			{
				if (i+1 == MAPSIZE)
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,0,i2);
					pFishArray[0][i2] = -1;
					pStarveArray[0][i2] = 0;
				}
				else
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i+1,i2);
					pFishArray[i+1][i2] = -1;
					pStarveArray[i+1][i2] = 0;
				}
			}
			else if (pFishArray[i][i2+1] > -1 ||(i2+1 == MAPSIZE && pFishArray[i][0] > -1))
			{
				if (i2+1 == MAPSIZE)
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i,0);
					pFishArray[0][i2] = -1;
					pStarveArray[0][i2] = 0;
				}
				else
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i,i2+1);
					pFishArray[i][i2+1] = -1;
					pStarveArray[i][i2+1] = 0;
				}
			}
			else if (pFishArray[i][i2-1] > -1 ||(i2-1 < 0 && pFishArray[i][MAPSIZE-1] > -1))
			{
				if (i2-1 < 0)
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i,MAPSIZE-1);
					pFishArray[i][MAPSIZE-1] = -1;
					pStarveArray[i][MAPSIZE-1] = 0;
				}
				else
				{
					checkSharkArraysAndMove(pSharkArray, pSharkMoveArray, pStarveArray,i,i2,i,i2-1);
					pFishArray[i][i2-1] = -1;
					pStarveArray[i][i2-1] = 0;
				}
			}
			*/
			//Else if there is no fish in the adjacent squares, move randomly
			else if (pSharkArray[i][i2] > -1)
			{ 
				pStarveArray[i][i2]++;
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
			//If his starve value is greater than starve, kill shark
			if (pStarveArray[i][i2] >= starveNum)
			{
				pStarveArray[i][i2] = 0;
				pSharkArray[i][i2] = -1;
				sharkcounter--;
				printf("Shark starved");
			}
		}
	}
}
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
void checkSharkArraysAndMove(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE], int pStarveArray[][MAPSIZE],int xPos, int yPos, int xOffset, int yOffset) {
	if (xOffset < 0)
	{
		xOffset = MAPSIZE;
	}
	else if (xOffset = MAPSIZE)
	{
		xOffset = 0;
	}
	if (yOffset < 0)
	{
		yOffset = MAPSIZE;
	}
	else if (yOffset = MAPSIZE)
	{
		yOffset = 0;
	}
	if (pFishArray[xOffset][yOffset] > -1)
	{
		pFishArray[xOffset][yOffset] = -1;
		pStarveArray = 0;
	}
	if (pSharkMoveArray[xOffset][yOffset] == 0 && pSharkArray[xOffset][yOffset] == -1)
	{
		pSharkArray[xOffset][yOffset] = pSharkArray[xPos][yPos];
		pStarveArray[xOffset][yOffset] = pStarveArray[xPos][yPos];
		pSharkArray[xPos][yPos]= -1;
		pStarveArray[xPos][yPos] = 0;
		
		pSharkArray[xOffset][yOffset] ++;
		if (pSharkArray[xOffset][yOffset] > sBreed)
		{
			printf("Shark fae breed");
			sharkcounter++;
			pSharkArray[xOffset][yOffset] = 0;
			pSharkArray[xPos][yPos] = 0;
			pStarveArray[xPos][yPos] = 0;
		}
	}
}
