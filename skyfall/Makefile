GOAL = skyfall
CC = gcc
FLAGS = -lm -g -Wall
OGL = -lGL -lGLU -lglut

$(GOAL): main.o draw.o image.o
	$(CC) -o $@ $^ $(FLAGS) $(OGL)

main.o: main.c draw.h draw.c image.h image.c
	$(CC) -o $@ $< -c

draw.o: draw.c draw.h
	$(CC) -o $@ $< -c

func.o: image.c image.h
	$(CC) -o $@ $< -c


.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o