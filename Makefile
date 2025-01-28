CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++20

SRC     := parsing/*.cpp lexing2023/*.cpp
INCLUDE := lexing2023
EXECUTABLE  := parser

all: $(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -Iparsing  $^ -o $@ 

clean:
	@echo "🧹 Clearing..."
	-rm ./*.o


