SRC := $(wildcard src/*.cpp)
FLAGS := -std=c++17 -o


main: $(SRC)
	g++ $^ $(FLAGS) $@

run: main
	./main