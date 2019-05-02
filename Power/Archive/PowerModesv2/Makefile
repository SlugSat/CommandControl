OBJECTS = PowerModes.o transitions.o  PModeTest.o 
CHECKOFF = PowerModes.o transitions.o  PowerCheckoff.o 
THREADS = -lpthread


all: PModeTest PowerCheckoff

PowerCheckoff: $(CHECKOFF)
	gcc -o PowerCheckoff $(CHECKOFF) $(THREADS)
	
PModeTest: $(OBJECTS)
	gcc -o pmodes $(OBJECTS) $(THREADS)

PowerModes.o: PowerModes.c PowerModes.h
	gcc -c PowerModes.c  $(THREADS)
	
PModeTest.o: PModeTest.c
	gcc -c PModeTest.c  $(THREADS)

transitions.o: transitions.c 
	gcc -c transitions.c  $(THREADS)
	
clean:
	rm -f PowerCheckoff pmodes *.o
