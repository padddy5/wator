#include<stdio.h>
#include <time.h>
#include <stdlib.h>

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <omp.h> 

typedef short bool;
#define true 1
#define false 0

#define MAPSIZE 1000
#define WINDOWSIZE 1000

#define fishBreedAge 80
#define sharkBreedAge 300
#define turnsToStarve 250

void renderFunction();
void update();
void move(int x, int y, int dirX, int dirY);
bool isFish(int x, int y);

struct creature {

	short type; // 0=fish, 1=shark
	bool moved;
	short age;
	short starve;	
};

struct creature map[MAPSIZE][MAPSIZE];

int sharkNum = 1000;
int fishNum = 1000;

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	int i;
#pragma omp parallel for private(i)
	for(i = 0; i < MAPSIZE; i++) {
		int j;
		#pragma omp parallel for private(j)
		for(j = 0; j < MAPSIZE; j++) {
			map[i][j].type = -1;
			map[i][j].moved = 0;
			map[i][j].age = -1;
			map[i][j].starve = -1;
		}
	}

	// Create fish
	int k;
	#pragma omp parallel for private(k)
	for(k = 0; k < fishNum; k++) {
		int posX = rand() % MAPSIZE;
		int posY = rand() % MAPSIZE;

		map[posY][posX].type = 0;
		map[posY][posX].moved = false;
		map[posY][posX].age = 0;
		map[posY][posX].starve = 0;
	}

	// Create sharks
	int s;
	#pragma omp parallel for private(s)
	for(s = 0; s < sharkNum; ) {
		int posX = rand() % MAPSIZE;
		int posY = rand() % MAPSIZE;

		if(map[posY][posX].type <= -1) {
			map[posY][posX].type = 1;
			map[posY][posX].moved = false;
			map[posY][posX].age = 0;
			map[posY][posX].starve = 0;
			s++;
		}
	}

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOWSIZE,WINDOWSIZE);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Wator");
    glutDisplayFunc(renderFunction);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WINDOWSIZE, 0.0, WINDOWSIZE, -1, 1);

    glutMainLoop(); 
	
 
    return 0;
}

void update() {

	int i;
	for(i = 0; i < MAPSIZE-1; i++) {
		int j;
		for(j = 0; j < MAPSIZE-1; j++) {
			if(map[i][j].moved == false) {
				if(map[i][j].type == 0) {
					// Move randomly
					int dir = rand() % 4;
					if (dir == 0) {
						move(i, j, 0, 1);
					}
					else if(dir == 1) {
						move(i, j, 0, -1);
					}
					else if(dir == 2) {
						move(i, j, 1, 0);
					}
					else if(dir == 3) {
						move(i, j, -1, 0);
					}
				}
				else {
					bool fishFound = false;
					int checks = 0;

					while(checks < 5) {
						
						// Look for nearby fish and eat it if one is found
						int dir = rand() % 4;
						if(dir == 0){
							if(isFish(i-1, 0)) {
								move(i, j, -1, 0);
								fishFound = true;
								checks = 5;
							} 
							else{checks++;}
						}
						else if(dir == 1) {
							if(isFish(i+1, 0)) {
								move(i, j, 1, 0);
								fishFound = true;
								checks = 5;
							}
							else{checks++;}
						}
						else if(dir == 2) {
							if(isFish(0, 1)) {
								move(i, j, 0, 1);
								fishFound = true;
								checks = 5;
							}
							else{checks++;}
						}
						else if(dir == 3) {
							if(isFish(0, -1)) {
								move(i, j, 0, -1);
								fishFound = true;
								checks = 5;
							}
							else{checks++;}
						}
					}		

					if(fishFound == false) {
						// Move randomly
						int dir = rand() % 4;
						if (dir == 0) {
							move(i, j, 0, 1);
						}
						else if(dir == 1) {
							move(i, j, 0, -1);
						}
						else if(dir == 2) {
							move(i, j, 1, 0);
						}
						else if(dir == 3) {
							move(i, j, -1, 0);
						}
					}
				}
			}
		}
	}
}

void move(int x, int y, int dirX, int dirY) {
		
	int destX = x+dirX;
	int destY = y+dirY;

	bool fishTarget = false;

	if(map[destX][destY].type == -1 || (map[destX][destY].type == 0 && map[x][y].type == 1)) {
		
		if(x+dirX < 0) {
			destX = MAPSIZE-1;	
		}		
		else if(x+dirX >= MAPSIZE) {
			destX = 0;	
		}

		if(y+dirY < 0) {
			destY = MAPSIZE-1;	
		}		
		else if(y+dirY >= MAPSIZE) {
			destY = 0;
		}
		
		// Set fishTarget to true if there is a fish at destination (used to reset shark starve)	
		if(map[destX][destY].type == 0) { 
			fishTarget = true;
		}

		map[destX][destY] = map[x][y];
		
		map[destX][destY].age++;
		map[destX][destY].moved = true;

		//fish
		if(map[destX][destY].type == 0) {
			if(map[destX][destY].age >= fishBreedAge) {

				map[x][y].type = 0;
				map[x][y].age = 0;
				map[x][y].moved = true;
				map[x][y].starve = -1;

				map[destX][destY].age = 0;
			}
			else {
				map[x][y].type = -1;
				map[x][y].age = -1;
				map[x][y].moved = false;
				map[x][y].starve = -1;
			}
		}	
		//shark
		else if(map[destX][destY].type == 1) {
			if(fishTarget == true) {
				map[destX][destY].starve = 0;			
			}
			else {
				map[destX][destY].starve++;
			}

			if(map[destX][destY].age >= sharkBreedAge) {

				map[x][y].type = 1;
				map[x][y].age = 0;
				map[x][y].moved = true;
				map[x][y].starve = 0;

				map[destX][destY].age = 0;
			}
			else {
				map[x][y].type = -1;
				map[x][y].age = -1;
				map[x][y].moved = false;
				map[x][y].starve = -1;
			}
			
			if(map[destX][destY].starve >= turnsToStarve) {
				map[destX][destY].type = -1;
				map[destX][destY].age = -1;
				map[destX][destY].moved = false;
				map[destX][destY].starve = -1;
			}
		}	
	}
}

bool isFish(int x, int y) {

	int targetX = x;
	int targetY = y;	

	if(targetX < 0) {
		targetX = MAPSIZE-1;	
	}
	else if(targetX >= MAPSIZE) {
		targetX = 0;
	}

	if(targetY < 0) {
		targetY = MAPSIZE-1;	
	}
	else if(targetY >= MAPSIZE) {
		targetY = 0;
	}

	if(map[targetX][targetY].type == 0) {
		return true;
	}

	return false;
}

void renderFunction()
{
	int x;
	for(x = 0; x < MAPSIZE; x++) {
		int y;
		for(y = 0; y < MAPSIZE; y++) {

			map[x][y].moved = false;
		}
	}

	update();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);


	int i;
	for(i = 0; i < MAPSIZE; i++) {
		int j;
		for(j = 0; j < MAPSIZE; j++) {
			if (map[i][j].type == 0) {

				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
					glVertex2f(i*2, j*2);
					glVertex2f((i*2)+2, j*2);
					glVertex2f((i*2)+2, (j*2)+2);
					glVertex2f(i*2, (j*2)+2);
				glEnd();
			}
			else if (map[i][j].type == 1) {

				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
					glVertex2f(i*2, j*2);
					glVertex2f((i*2)+2, j*2);
					glVertex2f((i*2)+2, (j*2)+2);
					glVertex2f(i*2, (j*2)+2);
				glEnd();
			}
		}
	}

    glFlush();

	//redraw
	glutPostRedisplay();
}
