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

$(Source)/parser : $(Source)/identifier.o $(Source)/errorstack.o $(Source)/parsing/main.o $(Source)/parsing/tokenizer.o $(Source)/parsing/parser.o $(Source)/parsing/symbol.o $(Source)/parsing/evaluator.o $(Source)/logic/type.o $(Source)/logic/selector.o $(Source)/logic/printing.o $(Source)/logic/structdef.o $(Source)/logic/term.o $(Source)/logic/uniquenamestack.o $(Source)/logic/pretty.o $(Source)/logic/counting.o $(Source)/logic/context.o $(Source)/logic/belief.o $(Source)/logic/beliefstate.o $(Source)/logic/kbo.o $(Source)/logic/position.o $(Source)/logic/replacements.o $(Source)/logic/structural.o $(Source)/lexing2023/filereader.o $(Source)/lexing2023/partition.o $(Source)/lexing2023/stateset.o
	$(CPP) $(Options) -o $(Source)/parser $(Source)/identifier.o $(Source)/errorstack.o $(Source)/parsing/main.o $(Source)/parsing/tokenizer.o $(Source)/parsing/parser.o $(Source)/parsing/symbol.o $(Source)/parsing/evaluator.o $(Source)/logic/type.o $(Source)/logic/selector.o $(Source)/logic/printing.o $(Source)/logic/structdef.o $(Source)/logic/term.o $(Source)/logic/uniquenamestack.o $(Source)/logic/pretty.o $(Source)/logic/counting.o $(Source)/logic/context.o $(Source)/logic/belief.o $(Source)/logic/beliefstate.o $(Source)/logic/kbo.o $(Source)/logic/position.o $(Source)/logic/replacements.o $(Source)/logic/structural.o $(Source)/lexing2023/filereader.o $(Source)/lexing2023/partition.o $(Source)/lexing2023/stateset.o $(Libs)

# Preprocessors:

$(Source)/parsing/parser.h $(Source)/parsing/parser.cpp $(Source)/parsing/symbol.h $(Source)/parsing/symbol.cpp : $(Source)/parsing/grammar.m
	$(Maph)/maphoon $(Source)/parsing/grammar.m $(Source)/parsing $(Maph)

# Compiling the source files:

$(Source)/identifier.o : $(Source)/identifier.cpp $(Source)/identifier.h $(Source)/util/hashbuilder.h
	$(CPP) -c $(Options) $(Source)/identifier.cpp -o $(Source)/identifier.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/errorstack.o : $(Source)/errorstack.cpp $(Source)/errorstack.h $(Source)/util/indentation.h
	$(CPP) -c $(Options) $(Source)/errorstack.cpp -o $(Source)/errorstack.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/main.o : $(Source)/parsing/main.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/evaluator.h $(Source)/parsing/location.h $(Source)/parsing/parser.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h $(Util)/./identifier.h $(Util)/./logic/belief.h $(Util)/./logic/beliefstate.h $(Util)/./logic/exact.h $(Util)/./logic/proof.h $(Util)/./logic/selector.h $(Util)/./logic/structdef.h $(Util)/./logic/term.h $(Util)/./logic/type.h $(Util)/./parsing/evaluator.h $(Util)/./util/hashbuilder.h $(Util)/identifier.h $(Util)/logic/belief.h $(Util)/logic/beliefstate.h $(Util)/logic/exact.h $(Util)/logic/proof.h $(Util)/logic/selector.h $(Util)/logic/structdef.h $(Util)/logic/term.h $(Util)/logic/type.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/parsing/main.cpp -o $(Source)/parsing/main.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/tokenizer.o : $(Source)/parsing/tokenizer.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/location.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h $(Util)/./identifier.h $(Util)/./logic/belief.h $(Util)/./logic/beliefstate.h $(Util)/./logic/exact.h $(Util)/./logic/proof.h $(Util)/./logic/selector.h $(Util)/./logic/structdef.h $(Util)/./logic/term.h $(Util)/./logic/type.h $(Util)/./parsing/evaluator.h $(Util)/./util/hashbuilder.h $(Util)/identifier.h $(Util)/logic/belief.h $(Util)/logic/beliefstate.h $(Util)/logic/exact.h $(Util)/logic/proof.h $(Util)/logic/selector.h $(Util)/logic/structdef.h $(Util)/logic/term.h $(Util)/logic/type.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/parsing/tokenizer.cpp -o $(Source)/parsing/tokenizer.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/parser.o : $(Source)/parsing/parser.cpp $(Source)/parsing/../lexing2023/acceptor.h $(Source)/parsing/../lexing2023/algorithms.h $(Source)/parsing/../lexing2023/classifier.h $(Source)/parsing/../lexing2023/comp.h $(Source)/parsing/../lexing2023/concepts.h $(Source)/parsing/../lexing2023/deterministic.h $(Source)/parsing/../lexing2023/filereader.h $(Source)/parsing/../lexing2023/flatmap.h $(Source)/parsing/../lexing2023/generators.h $(Source)/parsing/../lexing2023/includes.h $(Source)/parsing/../lexing2023/indentation.h $(Source)/parsing/../lexing2023/minimization.h $(Source)/parsing/../lexing2023/minmax.h $(Source)/parsing/../lexing2023/partition.h $(Source)/parsing/../lexing2023/reachfunc.h $(Source)/parsing/../lexing2023/state.h $(Source)/parsing/../lexing2023/stateset.h $(Source)/parsing/../lexing2023/viewreader.h $(Source)/parsing/evaluator.h $(Source)/parsing/location.h $(Source)/parsing/parser.h $(Source)/parsing/symbol.h $(Source)/parsing/tokenizer.h $(Util)/./identifier.h $(Util)/./logic/belief.h $(Util)/./logic/beliefstate.h $(Util)/./logic/exact.h $(Util)/./logic/proof.h $(Util)/./logic/selector.h $(Util)/./logic/structdef.h $(Util)/./logic/term.h $(Util)/./logic/type.h $(Util)/./parsing/evaluator.h $(Util)/./util/hashbuilder.h $(Util)/identifier.h $(Util)/logic/belief.h $(Util)/logic/beliefstate.h $(Util)/logic/exact.h $(Util)/logic/proof.h $(Util)/logic/selector.h $(Util)/logic/structdef.h $(Util)/logic/term.h $(Util)/logic/type.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/parsing/parser.cpp -o $(Source)/parsing/parser.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/symbol.o : $(Source)/parsing/symbol.cpp $(Source)/parsing/location.h $(Source)/parsing/symbol.h $(Util)/./identifier.h $(Util)/./logic/belief.h $(Util)/./logic/beliefstate.h $(Util)/./logic/exact.h $(Util)/./logic/proof.h $(Util)/./logic/selector.h $(Util)/./logic/structdef.h $(Util)/./logic/term.h $(Util)/./logic/type.h $(Util)/./parsing/evaluator.h $(Util)/./util/hashbuilder.h $(Util)/identifier.h $(Util)/logic/belief.h $(Util)/logic/beliefstate.h $(Util)/logic/exact.h $(Util)/logic/proof.h $(Util)/logic/selector.h $(Util)/logic/structdef.h $(Util)/logic/term.h $(Util)/logic/type.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/parsing/symbol.cpp -o $(Source)/parsing/symbol.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/parsing/evaluator.o : $(Source)/parsing/evaluator.cpp $(Source)/parsing/evaluator.h $(Util)/identifier.h $(Util)/logic/belief.h $(Util)/logic/beliefstate.h $(Util)/logic/exact.h $(Util)/logic/proof.h $(Util)/logic/selector.h $(Util)/logic/structdef.h $(Util)/logic/term.h $(Util)/logic/type.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/parsing/evaluator.cpp -o $(Source)/parsing/evaluator.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/type.o : $(Source)/logic/type.cpp $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/type.cpp -o $(Source)/logic/type.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/selector.o : $(Source)/logic/selector.cpp $(Source)/logic/selector.h
	$(CPP) -c $(Options) $(Source)/logic/selector.cpp -o $(Source)/logic/selector.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/printing.o : $(Source)/logic/printing.cpp $(Source)/logic/belief.h $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/term.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/printing.cpp -o $(Source)/logic/printing.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/structdef.o : $(Source)/logic/structdef.cpp $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/structdef.cpp -o $(Source)/logic/structdef.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/term.o : $(Source)/logic/term.cpp $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/term.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/term.cpp -o $(Source)/logic/term.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/uniquenamestack.o : $(Source)/logic/uniquenamestack.cpp $(Source)/logic/uniquenamestack.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/uniquenamestack.cpp -o $(Source)/logic/uniquenamestack.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/pretty.o : $(Source)/logic/pretty.cpp $(Source)/logic/belief.h $(Source)/logic/beliefstate.h $(Source)/logic/context.h $(Source)/logic/exact.h $(Source)/logic/kbo.h $(Source)/logic/pretty.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/term.h $(Source)/logic/type.h $(Source)/logic/uniquenamestack.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/pretty.cpp -o $(Source)/logic/pretty.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/counting.o : $(Source)/logic/counting.cpp $(Source)/logic/counting.h $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/term.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/counting.cpp -o $(Source)/logic/counting.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/context.o : $(Source)/logic/context.cpp $(Source)/logic/context.h $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/context.cpp -o $(Source)/logic/context.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/belief.o : $(Source)/logic/belief.cpp $(Source)/logic/belief.h $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/term.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/belief.cpp -o $(Source)/logic/belief.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/beliefstate.o : $(Source)/logic/beliefstate.cpp $(Source)/logic/belief.h $(Source)/logic/beliefstate.h $(Source)/logic/exact.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/term.h $(Source)/logic/termoperators.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/beliefstate.cpp -o $(Source)/logic/beliefstate.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/kbo.o : $(Source)/logic/kbo.cpp $(Source)/logic/exact.h $(Source)/logic/kbo.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/term.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/kbo.cpp -o $(Source)/logic/kbo.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/position.o : $(Source)/logic/position.cpp $(Source)/logic/position.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/position.cpp -o $(Source)/logic/position.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/replacements.o : $(Source)/logic/replacements.cpp $(Source)/logic/exact.h $(Source)/logic/kbo.h $(Source)/logic/proof.h $(Source)/logic/replacements.h $(Source)/logic/selector.h $(Source)/logic/term.h $(Source)/logic/topdown.h $(Source)/logic/type.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/replacements.cpp -o $(Source)/logic/replacements.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/logic/structural.o : $(Source)/logic/structural.cpp $(Source)/logic/belief.h $(Source)/logic/beliefstate.h $(Source)/logic/context.h $(Source)/logic/exact.h $(Source)/logic/kbo.h $(Source)/logic/pretty.h $(Source)/logic/proof.h $(Source)/logic/selector.h $(Source)/logic/structdef.h $(Source)/logic/structural.h $(Source)/logic/term.h $(Source)/logic/type.h $(Source)/logic/uniquenamestack.h $(Util)/errorstack.h $(Util)/identifier.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/hashbuilder.h $(Util)/util/indentation.h $(Util)/util/print.h
	$(CPP) -c $(Options) $(Source)/logic/structural.cpp -o $(Source)/logic/structural.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/filereader.o : $(Source)/lexing2023/filereader.cpp $(Source)/lexing2023/filereader.h
	$(CPP) -c $(Options) $(Source)/lexing2023/filereader.cpp -o $(Source)/lexing2023/filereader.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/partition.o : $(Source)/lexing2023/partition.cpp $(Source)/lexing2023/partition.h $(Source)/lexing2023/state.h $(Source)/lexing2023/stateset.h
	$(CPP) -c $(Options) $(Source)/lexing2023/partition.cpp -o $(Source)/lexing2023/partition.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

$(Source)/lexing2023/stateset.o : $(Source)/lexing2023/stateset.cpp $(Source)/lexing2023/state.h $(Source)/lexing2023/stateset.h
	$(CPP) -c $(Options) $(Source)/lexing2023/stateset.cpp -o $(Source)/lexing2023/stateset.o -I $(Lexing) -I $(Util) -I $(Maph) -I $(Source)

