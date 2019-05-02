OBJECTS = PowerModes.o PModeTest.o

all: PModeTest
	
PModeTest: $(OBJECTS)
	gcc -o pmodes $(OBJECTS)

PowerModes.o: PowerModes.c
	gcc -c PowerModes.c
	
PModeTest.o: PModeTest.c
	gcc -c PModeTest.c
	
clean:
	rm -f pmodes *.o
