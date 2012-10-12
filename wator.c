/* Hello World program */

#include<stdio.h>
#include <time.h>
#include <stdlib.h>

main()
{
	srand(time(NULL));

	int mapSize = 22;
	int sharkNum = 10;
	int fishNum = 20;		

	char sharks[mapSize][mapSize];
	char fish[mapSize][mapSize];
	
	// fill all arrays with ' '
	int i;
	for(i = 0; i < mapSize; i++) {
		int i2;
		for(i2 = 0; i2 < mapSize; i2++) {
			sharks[i][i2] = ' ';
			fish[i][i2] = ' ';

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
		sharks[r][r2] = '*';
		
		if(sharks[r][r2] == ' '){
			sharks[r][r2] = '*';
			j++;
		}
	}

	//createSharks(sharkNum, mapSize, sharks);

	// Create fish and add them to array
	int f;
	for(f = 0; f < fishNum; ) {
		int r = rand() % mapSize;
		int r2 = rand() % mapSize;
		
		// Create fish if generated index is empty
		if(sharks[r][r2] == ' ' && fish[r][r2] == ' '){
			fish[r][r2] = '.';
			f++;
		}
	}
	
	int k;
	for(k = 0; k < mapSize;) {
		int k2;
		for(k2 = 0; k2 < mapSize; k2++) {

			// Print sharks
			printf("%c", sharks[k][k2]);
			
			// Print fish
			if ( fish[k][k2] != ' ') {
				printf("%c", fish[k][k2]);
			}

			
		}	

		if ( k2 == mapSize ) {
			printf("\n");
			k++;
			k2 = 0;
		}
	}

}

/*void createSharks(int pNum, int pSize, char pArray) {
	
	int i;
	for(i = 0; i < pNum; i++) {
		int r = rand() % pSize;
		int r2 = rand() % pSize;
		pArray[r][r2] = '*'; 
	}
}*/


