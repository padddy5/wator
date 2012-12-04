#include<stdio.h>
#include <time.h>
#include <stdlib.h>

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <omp.h> 
#include <time.h>

typedef short bool;
#define true 1
#define false 0

#define MAPSIZE 500
#define WINDOWSIZE 1000

#define fishBreedAge 80
#define sharkBreedAge 300
#define turnsToStarve 250

void renderFunction();
void update();
void move(short x, short y, short dirX, short dirY);
bool isFish(short x, short y);
void updateCreature(short, short);
void renderCreatures(short, short);
void drawBitmapText(char *string,float x,float y,float z) ;

struct creature {

	short type; // 0=fish, 1=shark, -1=empty
	bool moved;
	short age;
	short starve;
};

struct creature map[MAPSIZE][MAPSIZE];

short sharkNum = 1000;
short fishNum = 1000;

FILE *fp;
time_t now;
struct tm* tm;
float prevTime;
float currentTime;
short frames;
double fps;
float tslu;

int startTime;
int endTime;

float totalFrames;
float frameLimit;

float timeTaken;

short main(int argc, char** argv)
{
	startTime = omp_get_wtime();

	srand(time(NULL));
	prevTime = 0;
	currentTime = 0;
	frames = 0;
	fps =0;
	tslu = 0;
	endTime = 0;


	frameLimit = 50;
	totalFrames = 0;
	timeTaken = 0;

	short i;
	#pragma omp parallel for private(i)
	for(i = 0; i < MAPSIZE; i++) {
		short j;
		#pragma omp parallel for private(j)
		for(j = 0; j < MAPSIZE; j++) {
			map[i][j].type = -1;
			map[i][j].moved = 0;
			map[i][j].age = -1;
			map[i][j].starve = -1;
		}
	}

	// Create fish
	short k;
	#pragma omp parallel for private(k)
	for(k = 0; k < fishNum; k++) {
		short posX = rand() % MAPSIZE;
		short posY = rand() % MAPSIZE;

		map[posY][posX].type = 0;
		map[posY][posX].moved = false;
		map[posY][posX].age = 0;
		map[posY][posX].starve = 0;
	}

	// Create sharks
	short s;
	
	for(s = 0; s < sharkNum; ) {
		short posX = rand() % MAPSIZE;
		short posY = rand() % MAPSIZE;

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
void drawBitmapText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) 
	{
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}
void update() {

	//printf( "%d", "test");
	//printf("%c", '*');

	currentTime =  glutGet(GLUT_ELAPSED_TIME);
	
	tslu = currentTime - prevTime;

	tslu = tslu /1000;
	fps = frames /tslu ;//tslu = time since last update
	frames = 0;

	prevTime =  glutGet(GLUT_ELAPSED_TIME);

	if(totalFrames == frameLimit && timeTaken == 0) {
		
		endTime = omp_get_wtime();
		timeTaken = endTime - startTime;
	}

	short i;
	short j;

	#pragma omp parallel private(i,j) num_threads(8)
	{
		int th_id = omp_get_thread_num();

		//if(th_id == 1){
		#pragma omp for private(i)
			for(i = 0; i < (MAPSIZE-1); i++) {
			
				#pragma omp private(j) for
				for(j = 0; j < (MAPSIZE-1); j++) {
						
					// Update fish
					if(map[i][j].type == 0) {
						//#pragma omp critical
						{
							updateCreature(i, j);
						}	
					}
					// Update sharks
					else if(map[i][j].type == 1) {
						//#pragma omp critical
						{
							updateCreature(i, j);
						}
					}
				}
			}
		//}
	}

	glColor3f(1.0, 0.0, 1.0);
	char bufferB[16] = "<some characters";
	char b;
	b = sprintf(bufferB, "%lf", timeTaken);
	drawBitmapText( &b, WINDOWSIZE -90,0,0);

	frames++;
	totalFrames++;
}

void move(short x, short y, short dirX, short dirY) {
		
	short destX = x+dirX;
	short destY = y+dirY;

	bool fishTarget = false;

	if(map[destX][destY].type == -1 || (map[destX][destY].type == 0 && map[x][y].type == 1)) {
		
		if(x+dirX < 0) {
			destX = MAPSIZE-2;	
		}		
		else if(x+dirX >= MAPSIZE-2) {
			destX = 0;	
		}

		if(y+dirY < 0) {
			destY = MAPSIZE-2;	
		}		
		else if(y+dirY >= MAPSIZE-2) {
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

bool isFish(short x, short y) {

	short targetX = x;
	short targetY = y;	

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

void updateCreature(short i, short j) {

	if(map[i][j].moved == false) {
		if(map[i][j].type == 0) {
			// Move randomly
			short dir = rand() % 4;
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
			short checks = 0;
			//#pragma omp parallel 

			while(checks < 5) {
				
				// Look for nearby fish and eat it if one is found
				short dir = rand() % 4;

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
				short dir = rand() % 4;
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


void renderCreatures(short i, short j)
{
	short size = 2;
	if (map[i][j].type == 0) {

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex2f(i*size, j*size);
			glVertex2f((i*size)+size, j*size);
			glVertex2f((i*size)+size, (j*size)+size);
			glVertex2f(i*size, (j*size)+size);
		glEnd();
	}
	else if (map[i][j].type == 1) {

		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2f(i*size, j*size);
			glVertex2f((i*size)+size, j*size);
			glVertex2f((i*size)+size, (j*size)+size);
			glVertex2f(i*size, (j*size)+size);
		glEnd();
	}
}

void renderFunction()
{
	short x;
	for(x = 0; x < MAPSIZE; x++) {
		short y;
		for(y = 0; y < MAPSIZE; y++) {

			map[x][y].moved = false;
		}
	}

	update();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	short i;
	short j;

	for(i = 0; i < (MAPSIZE); i++) {

		for(j = 0; j < (MAPSIZE); j++) {

			renderCreatures(i, j);
		}
	}

	glColor3f(1.0, 0.0, 1.0);
	char buffer[16] = "<some characters";
	char c;
	c = sprintf(buffer, "%lf", fps);
	drawBitmapText( &c, WINDOWSIZE -30,0,0);
		
    glFlush();

	//redraw
	glutPostRedisplay();
}
