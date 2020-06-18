all: main

main: main.o
	@echo "Linking all files into an executable"
	g++ -std=c++17 -o main main.o

main.o: Main.cpp
	@echo "Building main.o fille"
	g++ -std=c++17 -c -o main.o Main.cpp


clean:
	rm -rf main.o
