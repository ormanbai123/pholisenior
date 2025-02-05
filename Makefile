

# Automatically generated Makefile
# Makefile-generator created by Hans de Nivelle and Akhmetzhan Kussainov, 2023

# PHOLI

CPP = g++

Lexing = ./lexing2023
Maph = ./maphoon2024
Source = .
Util = .

Libs =

Options = -w -std=c++20

# Linking:

$(Source)/parser : $(Source)/parsing/main.o $(Source)/parsing/tokenizer.o $(Source)/parsing/parser.o $(Source)/parsing/symbol.o $(Source)/lexing2023/filereader.o $(Source)/lexing2023/partition.o $(Source)/lexing2023/stateset.o
	$(CPP) $(Options) -o $(Source)/parser $(Source)/parsing/main.o $(Source)/parsing/tokenizer.o $(Source)/parsing/parser.o $(Source)/parsing/symbol.o $(Source)/lexing2023/filereader.o $(Source)/lexing2023/partition.o $(Source)/lexing2023/stateset.o $(Libs)

# Preprocessors:

$(Source)/parsing/parser.h $(Source)/parsing/parser.cpp $(Source)/parsing/symbol.h $(Source)/parsing/symbol.cpp : $(Source)/parsing/grammar.m
	$(Maph)/maphoon $(Source)/parsing/grammar.m $(Source)/parsing $(Maph)

# Compiling the source files:

$(Source)/parsing/main.o : $(Source)/parsing/main.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/location.h $(Source)/parsing/parser.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h
	$(CPP) -c $(Options) $(Source)/parsing/main.cpp -o $(Source)/parsing/main.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/tokenizer.o : $(Source)/parsing/tokenizer.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/location.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h
	$(CPP) -c $(Options) $(Source)/parsing/tokenizer.cpp -o $(Source)/parsing/tokenizer.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/parser.o : $(Source)/parsing/parser.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/location.h $(Source)/parsing/parser.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h
	$(CPP) -c $(Options) $(Source)/parsing/parser.cpp -o $(Source)/parsing/parser.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/symbol.o : $(Source)/parsing/symbol.cpp $(Source)/parsing/location.h $(Source)/parsing/symbol.h
	$(CPP) -c $(Options) $(Source)/parsing/symbol.cpp -o $(Source)/parsing/symbol.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/filereader.o : $(Source)/lexing2023/filereader.cpp $(Source)/lexing2023/filereader.h
	$(CPP) -c $(Options) $(Source)/lexing2023/filereader.cpp -o $(Source)/lexing2023/filereader.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/partition.o : $(Source)/lexing2023/partition.cpp $(Source)/lexing2023/partition.h $(Source)/lexing2023/state.h $(Source)/lexing2023/stateset.h
	$(CPP) -c $(Options) $(Source)/lexing2023/partition.cpp -o $(Source)/lexing2023/partition.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/stateset.o : $(Source)/lexing2023/stateset.cpp $(Source)/lexing2023/state.h $(Source)/lexing2023/stateset.h
	$(CPP) -c $(Options) $(Source)/lexing2023/stateset.cpp -o $(Source)/lexing2023/stateset.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

