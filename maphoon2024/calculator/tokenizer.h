
#ifndef TOKENIZER_
#define TOKENIZER_   

#include "symbol.h"
#include "lexing2023/includes.h"


class tokenizer
{
   lexing::filereader inp;

public:
   tokenizer( ) = delete;

   tokenizer( lexing::filereader&& inp ) 
      : inp( std::move( inp ))
   { }

   void printstate( std::ostream& ) const;

   symbol handmade( );
   symbol generated( );

   symbol read( ) { return generated( ); }

   // This is used by the hand implemented tokenizer only:

   std::pair< symbolval, size_t > try_whitespace( );
   std::pair< symbolval, size_t > try_comment( ); 
   std::pair< symbolval, size_t > try_double( );
   std::pair< symbolval, size_t > try_special( );
   std::pair< symbolval, size_t > try_identifier( );

}; 

#endif

