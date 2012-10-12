#include<stdio.h>
#include <time.h>
#include <stdlib.h>

typedef short bool
#define true 1
#define false 0

#define MAPSIZE 20
#define RUNNUMBER 20
#define UPDATETIME 1

void createSharks(int pNum, int pSize, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]);
void createFish(int pNum, int pSize, int pFishArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]);
void updateFish(int pFishArray[][MAPSIZE], int pSharkArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE]);
void updateShark(int pSharkArray[][MAPSIZE], int pFishArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]);

main()
{
	srand(time(NULL));
	int sharkNum = 10;
	int fishNum = 20;		

	//Array of ints to represent shark and fish ages. If -1, no fish or shark at this point.
	//If not -1, they are alive.
	int sharks[MAPSIZE][MAPSIZE];
	int fish[MAPSIZE][MAPSIZE];
	short fishmove[MAPSIZE][MAPSIZE];
	short sharkmove[MAPSIZE][MAPSIZE];
	int starve[MAPSIZE][MAPSIZE];

	
	// fill all arrays with -1
	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int i2;
		for(i2 = 0; i2 < MAPSIZE; i2++) {
			sharks[i][i2] = -1;
			fish[i][i2] = -1;
			fishmove[i][i2] = 0;
			sharkmove[i][i2] = 0;
			starve[i][i2] = -1;
			if ( i2 == MAPSIZE ) {
				i++;
				i2 = 0;			
			}
		}
	}
	createSharks(sharkNum, MAPSIZE, sharks, starve, sharkmove);
	createFish(fishNum, MAPSIZE, fish, fishmove,sharks);
	
	//Game loop. Check time against last time world simulated. If greater than 1 second, re-simulate and re-draw world
	bool quit = false;
	//The number of times the scenario has been simulated and displayed.
	short timesSimulated = 0;
	//The last time at which the simulation was run and drawn to screen
	time_t lastTime;
	lastTime = time (NULL);
	

	//Game loop (update method)
	while (quit == false)
	{
		//The current time
		time_t thisTime;
  		thisTime = time (NULL);
		

		if (timesSimulated == RUNNUMBER)
		{
			//Exit from loop if simulation run RUNNUMBER times
			quit = true;
		}
		if (thisTime - lastTime > UPDATETIME) //If time since last simulation is greater than UPDATETIME, do the simulation
		{
			int k;
			for(k = 0; k < MAPSIZE;) {
				int k2;
				for(k2 = 0; k2 < MAPSIZE; k2++) {
					
					// Print sharks
					if ( sharks[k][k2] != -1) {
					printf("%c", '*');
					}
					// Print fish
					else if ( fish[k][k2] != -1) {
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
			//Increment counter and reset time counter
			timesSimulated++;
			lastTime = thisTime;
		}
	}

}
//Create sharks using passed in arrays
void createSharks(int pNum, int pSize, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], short pSharkMoveArray[][MAPSIZE]) {
	
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % pSize;
		int r2 = rand() % pSize;

		if (pSharkArray[r][r2] == -1) //If this position does not already contain a shark
	    {
		  	pSharkArray[r][r2] = 0; 
			pStarveArray[r][r2] = 0;
			pSharkMoveArray[r][r2] = 0;
		  	j++;
		}
	}
}
void createFish(int pNum, int pSize, int pFishArray[][MAPSIZE], short pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]) {
	
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % pSize;
		int r2 = rand() % pSize;

		if (pFishArray[r][r2] == -1 && pSharkArray[r][r2] == -1) //If this position does not already contain a fish or a shark
	    {
		  	pFishArray[r][r2] = 0; 
			pFishMoveArray[r][r2] = 0;
		  	j++;
		}
	}
}

