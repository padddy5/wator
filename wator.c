#include<stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAPSIZE 20

void createSharks(int pNum, int pSize, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], int pSharkMoveArray[][MAPSIZE]);
void createFish(int pNum, int pSize, int pFishArray[][MAPSIZE], int pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]);


main()
{
	srand(time(NULL));

	int mapSize = 22;
	int sharkNum = 10;
	int fishNum = 20;		

	//Array of ints to represent shark and fish ages. If -1, fish and shark not alive.
	//If not -1, they are alive.
	int sharks[mapSize][mapSize];
	int fish[mapSize][mapSize];
	short fishmove[mapSize][mapSize];
	short sharkmove[mapSize][mapSize];
	int starve[mapSize][mapSize];

	
	// fill all arrays with -1
	int i;
	for(i = 0; i < mapSize; i++) {
		int i2;
		for(i2 = 0; i2 < mapSize; i2++) {
			sharks[i][i2] = -1;
			fish[i][i2] = -1;
			fishmove[i][i2] = 0;
			sharkmove[i][i2] = 0;
			starve[i][i2] = -1;
			if ( i2 == mapSize ) {
				i++;
				i2 = 0;			
			}
		}
	}
	
	// Create sharks and add them to array
	int j;
	for(j = 0; j < sharkNum;) {
		int r = rand() % mapSize;
		int r2 = rand() % mapSize;

		if (sharks[r][r2] == -1)
	    {
		  	sharks[r][r2] = 0; 
			starve[r][r2] = 0;
		  	j++;
		}
	}

	// Create fish and add them to array
	int f;
	for(f = 0; f < fishNum; ) {
		int r = rand() % mapSize;
		int r2 = rand() % mapSize;
		
		// Create fish if generated index is empty
		if(sharks[r][r2] == -1 && fish[r][r2] == -1){
			fish[r][r2] = 0;
			f++;
		}
	}
	
	//Game loop. Check time against last time world simulated. If greater than 1 second, re-simulate and re-draw world
	short quit = 0;
	short timesSimulated = 0;
	time_t lastTime;
	lastTime = time (NULL);
	while (quit == 0)
	{
		time_t thisTime;
  		thisTime = time (NULL);
		

		if (timesSimulated == 1000)
		{
			quit = 1;
		}
		if (thisTime - lastTime > 1000)
		{
			int k;
			for(k = 0; k < mapSize;) {
				int k2;
				for(k2 = 0; k2 < mapSize; k2++) {

					// Print sharks
					if ( sharks[k][k2] != -1) {
					printf("%c", '*');
					}
					// Print fish
					else if ( fish[k][k2] != -1) {
						printf("%c", '.');
					}
					//Output space
					else
					{
						printf("%c", ' ');
					}
			
				}	
				//Go to next line
				if ( k2 == mapSize ) {
					printf("\n");
					k++;
					k2 = 0;
				}
			}
			lastTime = thisTime;
		}
	}

}
//Create sharks using passed in arrays
void createSharks(int pNum, int pSize, int pSharkArray[][MAPSIZE], int pStarveArray[][MAPSIZE], int pSharkMoveArray[][MAPSIZE]) {
	
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % pSize;
		int r2 = rand() % pSize;

		if (pSharkArray[r][r2] == -1) //If this position 
	    {
		  	pSharkArray[r][r2] = 0; 
			pStarveArray[r][r2] = 0;
			pSharkMoveArray[r][r2] = 0;
		  	j++;
		}
	}
}
void createFish(int pNum, int pSize, int pFishArray[][MAPSIZE], int pFishMoveArray[][MAPSIZE], int pSharkArray[][MAPSIZE]) {
	
	int j;
	for(j = 0; j < pNum;) {
		int r = rand() % pSize;
		int r2 = rand() % pSize;

		if (pFishArray[r][r2] == -1 && pSharkArray[r][r2] == -1)
	    {
		  	pFishArray[r][r2] = 0; 
			pFishMoveArray[r][r2] = 0;
		  	j++;
		}
	}
}

