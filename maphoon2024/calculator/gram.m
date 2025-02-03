
// A sample Maphoon file. 

%startsymbol Session EOF 
   // A start symbol, together with its terminators. 

%reductionseq Quit Show Debug Nodebug H
 
// %nodefaults 

// Symbols and their attributes types: 

%symbol{       	     }             EOF  BAD 
%symbol{ std::string }             SCANERROR IDENT 
%symbol                            SEMICOLON ASSIGN COMMA 
%symbol{ double }                  DOUBLE
%symbol                            PLUS TIMES MINUS DIVIDES MODULO
%symbol                            FACTORIAL
%symbol                            LPAR RPAR 
%symbol{ double }                  E F G H 
%symbol{ std::vector<double> }     Arguments  
%symbol                            Session Command 
%symbol                            Quit Show Debug Nodebug 

   // No attribute type means void. 
%symbol{}                          COMMENT WHITESPACE EMPTY
   // These symbols are used internally in the tokenizer.
   // One still has to declare them. 

%parameter{ std::map< std::string, double > } memory 
%parameter{ std::vector< std::string > }      errorlog 
%parameter{ tokenizer }                       tok
   // Declares additional parameters to the parser.
   // They are reference parameters to the parser, 
   // and they can be used in action code. 
   // There is no point in allowing local variables
   // in the parser.
   // If you want to read input from a file, or from
   // somewhere, you have to include it here.

// This goes into the beginning of symbol.h :

%symbolcode_h{ #include <vector> } 
%parsercode_h{ #include <map> }

%parsercode_h{ #include "tokenizer.h" }
%parsercode_h{ #include <cmath> }

%source{ tok. read( ); }
   // Source from where the symbols come.
   // It must compile in a context of form s = readsymbol( );


%rules

Session => Session Command 
|
;

Command => E:e SEMICOLON
| IDENT:id ASSIGN E:e SEMICOLON
| Quit SEMICOLON
| Show SEMICOLON
| Debug SEMICOLON { debug = 1; }
| Nodebug SEMICOLON { debug = 0; }
|  _recover_ SEMICOLON
;

E  => E:e PLUS F:f   { return e + f; }
    | E:e MINUS F:f  { return e - f; }
    | F : f          { return f; }
    ;

F  => F:f TIMES G:g { return f * g; }
| F:f DIVIDES G:g 

| F:f MODULO G:g    // here you can put a comment 

| G : g   
;

G => MINUS G : g   
  | PLUS G : g   
  | H : h      
  ;

H  => H:h FACTORIAL
 
| LPAR E:e RPAR  
| IDENT : id 

| DOUBLE : d   { return d; } 

| IDENT:id LPAR Arguments:args RPAR 
;

Arguments => E:e          {  return { e };   }
| Arguments:a COMMA E:e   { a. push_back(e); return a; }
;

Quit => IDENT : id
%requires { return id == "quit"; }
;

Show => IDENT : id 
%requires { return id == "show"; }
;

Debug => IDENT : id 
%requires { return id == "debug"; }
;

Nodebug => IDENT : id
%requires { return id == "nodebug"; }
;

%errors
   IDENT LPAR => "a function argument";
   F ( TIMES | DIVIDES ) => "a factor"; 
    
