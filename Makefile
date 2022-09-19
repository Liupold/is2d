CC = gcc
CFLAGS = -O2 -Wall -lm
DIR_SRC = src
DIR_BUILD = build
DIR_BINs = bins


is2d: main.o ising-2d.o
	$(CC) $(CFLAGS) $(DIR_BUILD)/ising-2d.o $(DIR_BUILD)/main.o -o $(DIR_BINs)/is2d -lGL -lGLU -lglut

main.o: dirs
	$(CC) $(CFLAGS) -c $(DIR_SRC)/main.c -o $(DIR_BUILD)/main.o

ising-2d.o: dirs
	$(CC) $(CFLAGS) -c $(DIR_SRC)/ising-2d.c -o $(DIR_BUILD)/ising-2d.o

graphs: ising-2d.o
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/ising-2d.o graphs-analysis/chi-graph.c -o $(DIR_BINs)/chi-graph
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/ising-2d.o graphs-analysis/beta-param-val.c -o $(DIR_BINs)/beta-param-val.c

all: is2d graphs

dirs:
	mkdir -p $(DIR_BUILD) $(DIR_BINs)

clean:
	rm -rf $(DIR_BUILD) $(DIR_BINs)
