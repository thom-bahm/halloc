hallocTest: halloc.o hallocTest.o
	gcc halloc.o hallocTest.o -o hallocTest

hallocTest.o: hallocTest.c halloc.h
	gcc hallocTest.c -c

halloc.o: halloc.c halloc.h 
	gcc halloc.c -c

clean:
	rm -f *.o
	rm -f hallocTest