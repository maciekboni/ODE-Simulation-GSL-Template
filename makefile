# driver: driver.o rkf.o
# 	lcc  driver.o rkf.o -o driver -lm
# makefile
# Tabs *must* be used for the indentations below;
# spaces cause make syntax errors.

CC=g++
#CFLAGS=-fast -xO4 -xdepend -xarch=v8plusa -xprefetch -xvector -xunroll=8 -fsimple=2 -xsafe=mem
LIBS=-lm
GSLLIBS=-lgsl -lgslcblas 

ode:		
		$(CC) -O3 -c -o predict.o predict.cpp $(LIBS) $(GSLLIBS)
		$(CC) -O3 -c -o func.o func.cpp $(LIBS) $(GSLLIBS)
		$(CC) -O3 -c -o main.o main.cpp $(LIBS) $(GSLLIBS)
		$(CC) $(CFLAGS) -o run predict.o main.o func.o $(LIBS) $(GSLLIBS) 


clean:
		rm -f *.o core run *~




