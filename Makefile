default:
	gcc -fopenmp -lglut -lGL -lGLEW -lGLU watorGL.c -o wator


clean:
	-rm -f wator
