OBJECTS = PowerModes.o transitions.o  PModeTest.o 

all: PModeTest
	
PModeTest: $(OBJECTS)
	gcc -o pmodes $(OBJECTS)

PowerModes.o: PowerModes.c PowerModes.h
	gcc -c PowerModes.c
	
PModeTest.o: PModeTest.c
	gcc -c PModeTest.c

transitions.o: transitions.c 
	gcc -c transitions.c
	
clean:
	rm -f pmodes *.o
