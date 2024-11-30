all:clean link
	@echo "Build complete\nRun bin/main to start the simulation"

compile: src/Settlement.cpp src/main.cpp src/Facility.cpp src/SelectionPolicy.cpp src/Plan.cpp src/Action.cpp src/Simulation1.cpp src/Auxiliary.cpp
	@echo "Compiling source code"
	g++ -g -c -o bin/Settlement.o src/Settlement.cpp
	g++ -g -c -o bin/Facility.o src/Facility.cpp
	g++ -g -c -o bin/main.o src/main.cpp
	g++ -g -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -c -o bin/Plan.o src/Plan.cpp
	g++ -g -c -o bin/Action.o src/Action.cpp
	g++ -g -c -o bin/Simulation.o src/Simulation1.cpp
	g++ -g -c -o bin/Auxiliary.o src/Auxiliary.cpp


clean:
	@echo "Cleaning binary directory"
	rm -f bin/.o*

link: compile
	@echo "Linking object files"
	g++ -g -o bin/main bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Action.o bin/Simulation.o bin/Auxiliary.o

run: bin/main
	@echo "Running simulation"
	@echo ".\n.\n.\n"
	@./bin/main path


valgrind: bin/main
	valgrind --leak-check=full --show-reachable=yes bin/main config.txt