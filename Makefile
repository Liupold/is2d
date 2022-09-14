is2d: main.o ising-2d.o
	gcc build/ising-2d.o build/main.o -o is2d -lm  -lGL -lGLU -lglut

main.o: dirs
	gcc -c scr/main.c -o build/main.o

ising-2d.o: dirs
	gcc -c scr/ising-2d.c -o build/ising-2d.o

dirs:
	mkdir -p build/

clean:
	rm -rf build/ising-2d.o build/main.o is2d
