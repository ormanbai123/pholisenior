
// A sample Maphoon file. 

%startsymbol Session EOF 
   // A start symbol, together with its terminators. 

%reductionseq Quit Show Debug Nodebug H
 
// %nodefaults 

// Symbols and their attributes types: 

%symbol{       	     }             EOF BAD 
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


%symbolcode_cpp{ 

   void print_attr( const std::vector< double > & vect, std::ostream& out )
   {
      print_range( vect. begin( ), vect. end( ), '{', '}', out );
   }     
}


%parsercode_cpp {

   double fact( unsigned int _n )
   {
      double res = 1.0;
      while( _n )
      {
         res *= _n;
         -- _n;
      }
      return res;
   }

   void printerrors( const std::vector< std::string > & errors, 
                     std::ostream& out )
   {
      std::cout << "Errors:\n";
      for( const auto& err : errors )
         out << "   " << err << "\n";
   }

}
   

%source{ tok. read( ); } 
   // Source from where the symbols come. 
   // It must compile in a context of form s = readsymbol( ); 

%rules

Session => Session Command 
|
;

Command => E:e SEMICOLON
{
   if( errorlog. empty( ))
      std::cout << "   --> " << e << "\n";
   else
   {
      printerrors( errorlog, std::cout ); 
      errorlog. clear( ); 
   }
}

| IDENT:id ASSIGN E:e SEMICOLON

{
   if( errorlog. empty( )) 
   {
      std::cout << "   --> assigning " << id << " := " << e << "\n";
      memory[id] = e; 
   }
   else
   {
      printerrors( errorlog, std::cout ); 
      errorlog. clear( ); 
   }
}

| Quit SEMICOLON
{
   std::cout << "\n";
   std::cout << "quitting\n"; timetosaygoodbye = true;
}
| Show SEMICOLON
{
   std::cout << "Memory:\n";
   for( const auto& p : memory )
      std::cout << "   " << p. first << " : " << p. second << "\n";
   std::cout << "\n\n";
}
| Debug SEMICOLON { debug = 1; }
| Nodebug SEMICOLON { debug = 0; }
|  _recover_ SEMICOLON
{
   if( debug ) 
      std::cout << "recovered from syntax error\n\n";
}
;

E  => E:e PLUS F:f   { return e + f; }
    | E:e MINUS F:f  { return e - f; }
    | F : f          { return f; }
    ;

F  => F:f TIMES G:g { return f * g; }
| F:f DIVIDES G:g 

{
   if( g == 0.0 ) 
   {
      errorlog. push_back( "division by zero" ); 
      g = 1.0;   // invent a value. 
   }
   return f / g;  
}

| F:f MODULO G:g    // here you can put a comment 

{
   if( g == 0.0 )
   {
      errorlog. push_back( "modulo by zero" );
      g = 1.0;
   }
    
   return f - g * floor( f / g ); 
}

| G : g   /* here can also be comment */ { return g; }
;

G => MINUS G : g    { return -g; }
  | PLUS G : g      { return g; } 
  | H : h           { return h; }
  ;

H  => H:h FACTORIAL
{
   unsigned int f = static_cast< unsigned int >
         ( floor( h + 0.0001 ));
   return fact(f);  
}
 
   | LPAR E:e RPAR  { return e; }
   | IDENT : id 
   {
      if( memory. contains(id))
         return memory. at( id );
      else
      {
         errorlog. push_back( std::string( "variable " ) + id + 
                              " is undefined " );
         return 0.0;  
      }
   }

   | DOUBLE : d   { return d; } 

   | IDENT:id LPAR Arguments:args RPAR 
   { 
      if( id == "sin" && args. size( ) == 1 ) 
         return sin( args[0] ); 

      if( id == "cos" && args. size( ) == 1 )
         return cos( args[0] );

      if( id == "sqrt" && args. size( ) == 1 )
         return sqrt( args[0] ); 

      if( id == "pow" && args. size( ) == 2 )
      {
         return pow( args[0], args[1] );
      }
  
      if( id == "exp" && args. size( ) == 1 )
      {
         return exp( args[0] );
      }

      if( id == "log" && args. size( ) == 1 )
      {
         return log( args[0] );
      }

      if( id == "sinh" && args. size( ) == 1 )
      {
         return sinh( args[0] );
      }

 
      errorlog. push_back( std::string( "calling unknown function " ) + id );
      return 0.0;
   }
  
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
    
