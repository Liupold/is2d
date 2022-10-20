CC = gcc
CFLAGS = -O2 -Wall -lm -Wextra -Wpedantic -Werror
DIR_SRC = src
DIR_BUILD = build
DIR_BINs = bins

all: is2d graphs

is2d: ising-2d.o mtwister.o
	$(CC) $(CFLAGS) $(DIR_BUILD)/*.o $(DIR_SRC)/main.c -o $(DIR_BINs)/is2d -lGL -lGLU -lglut

ising-2d.o: dirs
	$(CC) $(CFLAGS) -c $(DIR_SRC)/ising-2d.c -o $(DIR_BUILD)/ising-2d.o

mtwister.o: dirs
	$(CC) $(CFLAGS) -c $(DIR_SRC)/mtwister.c -o $(DIR_BUILD)/mtwister.o


graphs: ising-2d.o
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/*.o graphs-analysis/chi-vs-T-graph.c -o $(DIR_BINs)/chi-vs-T -lpthread
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/*.o graphs-analysis/m-vs-T-graph.c -o $(DIR_BINs)/m-vs-T
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/*.o graphs-analysis/binder-cumulant.c -o $(DIR_BINs)/binder-cumulant
	$(CC) $(CFLAGS) -I $(DIR_SRC) $(DIR_BUILD)/*.o graphs-analysis/binder-cumulant-2.c -o $(DIR_BINs)/binder-cumulant-2


dirs:
	mkdir -p $(DIR_BUILD) $(DIR_BINs)

clean:
	rm -rf $(DIR_BUILD) $(DIR_BINs)
