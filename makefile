CC = gcc
OPTION = -lm -g -lpthread
CFLAGS = -Wall
OPTIMISATIONLEVEL = -O3

PARAM = $(CFLAGS) $(OPTIMISATIONLEVEL) $(OPTION)

ENTITY = entity/guard.o entity/player.o entity/position.o
SCENE = scene/scene.o scene/simulation.o
TOOLS = tools/draw.o tools/graph.o tools/out.o tools/priority.o tools/stack.o

%.o : %.c
	$(CC) -o $@ -c $< $(PARAM)

tests : tests/tests.o $(ENTITY) $(SCENE) $(TOOLS)
	$(CC) $^ -o tests.out $(PARAM)
	
main : main.o $(ENTITY) $(SCENE) $(TOOLS)
	$(CC) $^ -o main.out $(PARAM)

cleano :
	rm -f */*.o
	rm -f *.o

clean : cleano
	rm -f out/*.txt
	rm -f *.out

all_t : tests
	./tests.out
	python3 ./out/main.py

all_m : main
	./main.out 20 1 200 5 20
	python3 ./out/main.py