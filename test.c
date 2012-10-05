/* Hello World program */

#include<stdio.h>
#include <time.h>
#include <stdlib.h>

main()
{
	srand(time(NULL));
	char sharks[20][20];
	
	int i;
	for(i = 0; i < 20; i++) {
		int i2;
		for(i2 = 0; i2 < 20; i2++) {
			sharks[i][i2] = ' ';

			if ( i2 == 20 ) {
				i++;
				i2 = 0;			
			}
		}
	}
	
	
	int j;
	for(j = 0; j < 5; j++) {
		int j2;
		for(j2 = 0; j2 < 5; j2++) {
			int r = rand() % 20;
			int r2 = rand() % 20;
			sharks[r][r2] = '*';	

			if ( j2 == 20 ) {
				j++;
				j2 = 0;			
			}		
		}
	}

	int k;
	for(k = 0; k < 20; k++) {
		int k2;
		for(k2 = 0; k2 < 20; k2++) {
			printf("%c", sharks[k][k2]);

			if ( k2 == 20 ) {
				printf("\n");
				k++;
				k2 = 0;
			}
		}	
	}
    
}

