default: wator

wator.o: wator.c
	gcc -c wator.c -o wator.o

wator: wator.o
	gcc wator.o -o wator

clean:
	-rm -f wator.o
	-rm -f wator
