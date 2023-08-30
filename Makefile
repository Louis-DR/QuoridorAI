CC      := g++
C_FLAGS := -std=c++17 -O3 -fpermissive

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= release.exe
else
EXECUTABLE	:= release
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(wildcard src/*.cpp)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)
