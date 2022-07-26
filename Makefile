CC = g++
COMPILATION_FLAGS = -lpanel -lncurses -std=c++17
MAIN_FILE = Snake_multiPlayer.cpp
EXECUTABLE = Snake_multiPlayer

main:
	$(CC) $(MAIN_FILE) $(COMPILATION_FLAGS) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE)
