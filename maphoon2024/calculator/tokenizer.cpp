
#include "tokenizer.h"

namespace
{
   lexing::classifier< char, symbolval > buildclassifier( )
   {
      using namespace lexing; 

      classifier< char, symbolval > cls( sym_SCANERROR );

      auto letter = range( 'a', 'z' ) | range( 'A', 'Z' );
      auto digit = range( '0', '9' );

      auto whitespace = just( ' ' ) | just( '\t' ) | just( '\n' ) |
                        just( '\v' ) | just( '\f' ) | just( '\r' ); 

      cls. insert( whitespace. plus( ), sym_WHITESPACE );

      cls. insert( word( "/*" ) *
         ( every<char> ( ). without( '*' ) |
            ( just('*'). plus( ) *
               every<char> ( ). without( '/' ). without( '*' )). star( )
            ). star( ) *
            just( '*' ). plus( ) * just( '/' ), sym_COMMENT );

      cls. insert( word( "//" ) *
              ( every<char>( ). without( '\n' )). star( ) * 
              just( '\n' ), sym_COMMENT );
      
      auto exp = ( just( 'e' ) | just( 'E' ) ) *
                 ( just( '-' ) | just( '+' )). optional( ) *
                 digit * digit. optional( ) * digit. optional( );  

      cls. insert( digit. plus( ) * 
                   ( just('.') * digit. star( )). optional( ) * 
                   exp. optional( ), sym_DOUBLE ); 
      cls. insert( just( '.') * digit. plus( ) *
                   exp. optional( ), sym_DOUBLE );

      cls. insert( letter * 
                   ( letter | digit | just( '_') ). star( ), sym_IDENT );

      cls. insert( just( '#' ), sym_EOF );
      cls. insert( just( ';' ), sym_SEMICOLON );
      cls. insert( word( ":=" ), sym_ASSIGN );
      cls. insert( just( ',' ), sym_COMMA );
      cls. insert( just( '+' ), sym_PLUS );
      cls. insert( just( '-' ), sym_MINUS );
      cls. insert( just( '*' ), sym_TIMES ); 
      cls. insert( just( '/' ), sym_DIVIDES );
      cls. insert( just( '%' ), sym_MODULO );
      cls. insert( just( '!' ), sym_FACTORIAL );
      cls. insert( just( '(' ), sym_LPAR );
      cls. insert( just( ')' ), sym_RPAR );

      // Put 'true' if you want to see all these automata:

      // std::cout << "non-deterministic:\n";
      // std::cout << cls << "\n";

      cls = make_deterministic(cls);
      // std::cout << "deterministic:\n";
      // std::cout << cls << "\n";

      cls = minimize( cls ); 
      // std::cout << "minimization:\n";
      // std::cout << cls << "\n";
 
#if 1
      std::ofstream file( "DFA.h" );
      if( !file ) throw std::runtime_error( "could not open file" );

      lexing::printcode<char, symbolval > (
        "char", "symbolval", {},
         cls, file,
         []( std::ostream& out, char c ) { out << (int) c; },
         []( std::ostream& out, const symbolval v ) { out << "sym_" << v; } );
#endif
      return cls;
   }

}


// #include "DFA.h"

symbol tokenizer::generated( )
{
   static auto cls = buildclassifier( );
   // unsigned int cls = 71;

restart:
   if( !inp. has(1))
      return symbol( sym_EOF );

   if( !inp. good() )
      return symbol( sym_BAD );

   std::pair< symbolval, size_t > p = readandclassify( cls, inp );
   // std::cout << p. first << "/" << p. second << "\n";

   if( p. second == 0 )
   {
      std::string attr; attr. push_back( inp. peek(0) );
      inp. commit(1);
      return symbol( sym_SCANERROR, attr );
   }

   // Stuff that needs to be ignored:

   if( p. first == sym_WHITESPACE || p. first == sym_COMMENT )
   {
      inp. commit( p. second );
      goto restart;
   }

   // String attribute:

   if( p. first == sym_SCANERROR || p. first == sym_IDENT )
   {
      std::string attr = std::string( inp. view( p. second ));
      inp. commit( p. second );
      return symbol( p. first, std::move( attr ));
   }

   if( p. first == sym_DOUBLE )
   {
      try
      {
         double val = std::stod( std::string( inp. view( p. second )));
         inp. commit( p. second );
         return symbol( p. first, val );
      }
      catch( const std::out_of_range& err )
      {
         std::string attr = std::string( inp. view( p. second ));
         inp. commit( p. second );
         return symbol( sym_SCANERROR, attr ); 
      } 
   }
 
   // Remaining symbols are without attribute:

   inp. commit( p. second );
   return symbol( p. first );
}


namespace
{

   bool starts_ident( char c )
   {
      return isalpha(c);
   }

   bool continues_ident( char c )
   {
      return isalpha(c) || isdigit(c) || c == '_';
   }

}


std::pair< symbolval, size_t > tokenizer::try_whitespace( )
{
   if( inp. has(1) && isspace( inp. peek(0)) )
   {
      size_t i = 1;
      while( inp. has(i+1) && isspace( inp. peek(i)) )
         ++ i;
      return std::pair( sym_WHITESPACE, i );
   }

   return std::pair( sym_EMPTY, 0 );
}


std::pair< symbolval, size_t > tokenizer::try_double( )
{
   if( inp. has(1) &&
       ( isdigit( inp. peek(0)) ||
         ( inp. peek(0) == '.' && inp. has(2) && isdigit( inp. peek(1)) )) )
   {
      size_t i = 0;

      while( inp. has(i+1) && isdigit( inp. peek(i)) )
         ++ i;

      // fraction?

      if( inp. has(i+1) && inp. peek(i) == '.' )
      {
         ++ i;
         while( inp. has(i+1) && isdigit( inp. peek(i)) )
            ++ i;
      }

      // Exponent, no more than 3 digits.

      if( inp. has(i+1) &&
            ( inp. peek(i) == 'e' || inp. peek(i) == 'E' ))
      {
         ++ i;
         if( inp. has(i+1) &&
                ( inp. peek(i) == '+' || inp. peek(i) == '-' ))
         {
            ++ i;
         }

         if( !inp. has(i+1) || !isdigit( inp. peek(i)) )
            return std::pair( sym_SCANERROR, i );
         ++ i;
         if( inp. has(i+1) && isdigit( inp. peek(i)) )
            ++ i;
         if( inp. has(i+1) && isdigit( inp. peek(i)) )
            ++ i;
         if( inp. has(i+1) && isdigit( inp. peek(i)) )
            return std::pair( sym_SCANERROR, i + 1 );

      }
      return std::pair( sym_DOUBLE, i );
   }
   return std::pair( sym_EMPTY, 0 );
}


std::pair< symbolval, size_t > tokenizer::try_identifier( )
{
   if( inp. has(1) && starts_ident( inp. peek(0)) )
   {
      size_t i = 1;
      while( inp. has(i+1) && continues_ident( inp. peek(i) ))
         ++ i;
      return std::pair( sym_IDENT, i );
   }
   else
      return std::pair( sym_IDENT, 0 );
}


std::pair< symbolval, size_t > tokenizer::try_comment( )
{
   if( inp.has(1) && inp. peek(0) == '/' )
   {
      if( inp. has(2))
      {
         // Starting with '//' :

         if( inp. peek(1) == '/' )
         {
            size_t i = 2;
            while( inp. has(i+1) && inp. peek(i) != '\n' )
               ++ i;
            if( inp. has(i+1))
               ++ i;             // Skip the '\n' too if it is there.

            return std::pair( sym_COMMENT, i );
         }

         // Starting with '/*' :

         if( inp. peek(1) == '*' )
         {
            size_t i = 2;
            while( inp. has(i+2) &&
                   ( inp. peek(i) != '*' || inp. peek(i+1) != '/' ))
            {
               ++ i;
            }

            // This means that we encountered eof in the comment:

            if( !inp. has( i + 1 ))
               return std::pair( sym_SCANERROR, i );
            if( !inp. has( i + 2 ))
               return std::pair( sym_SCANERROR, i + 1 );

            return std::pair( sym_COMMENT, i + 2 );
         }
      }
   }
   return std::pair( sym_EMPTY, 0 );
}


symbol tokenizer::handmade( ) 
{

restart:
   if( !inp. has(1))
      return symbol( sym_EOF );

   auto p = try_whitespace( );
   if( p. second ) 
   {
      inp. commit( p. second );
      goto restart;
   }

   p = try_comment( );
   if( p. first == sym_COMMENT && p. second ) 
   {
      inp. commit( p. second );
      goto restart;
   }

   p = try_double( );
   if( p. second )  
   {
      double val = std::stod( std::string( inp. view( p. second ))); 
      inp. commit( p. second ); 
      return symbol( sym_DOUBLE, val );
   }
  
   p = try_identifier( );
   if( p. second )
   { 
      std::string id = std::string( inp. view( p. second ));
      inp. commit( p. second );
      return symbol( sym_IDENT, std::move( id )); 
   }

   char c = inp. peek(0);

   // # can be used as end-of-file:

   if( c == '#' )
   {
      // We don't commit. 

      return symbol( sym_EOF );
   }

   if( c == ';' )
   {
      inp. commit(1);
      return symbol( sym_SEMICOLON );
   }

   if( c == ':' && inp. has(2) && inp. peek(1) == '=' ) 
   {
      inp. commit(2);
      return symbol( sym_ASSIGN );
   }

   if( c == ',' ) 
   {
      inp. commit(1); 
      return symbol( sym_COMMA );
   }

   if( c == '+' )
   {
      inp. commit(1);
      return symbol( sym_PLUS );
   }

   if( c == '-' ) 
   {
      inp. commit(1);
      return symbol( sym_MINUS );
   }

   if( c == '*' )
   {
      inp. commit(1);
      return symbol( sym_TIMES );
   }

   if( c == '/' ) 
   {
      inp. commit(1);
      return symbol( sym_DIVIDES ); 
   }

   if( c == '%' )
   {
      inp. commit(1);
      return symbol( sym_MODULO );
   }

   if( c == '!' )
   {
      inp. commit(1);
      return symbol( sym_FACTORIAL );
   }

   if( c == '(' )
   {
      inp. commit(1);
      return symbol( sym_LPAR );
   }

   if( c == ')' )
   {
      inp. commit(1);
      return symbol( sym_RPAR );
   }

   // If we could not recognize anything, then we produce
   // a scan error.

   std::string err; err. push_back(c);

   inp. commit(1);
   return symbol( sym_SCANERROR, err );
}


