

#ifndef MAPH_OUROWNPARSER_
#define MAPH_OUROWNPARSER_  1

#include <vector>

#include "tokenizing.h"
#include "errors.h"
#include "symbolcodebuilder.h"
#include "parsercodebuilder.h"
#include "cpp/codefragment.h" 


struct ourownparser
{
   lexing::filereader read;
   std::pair< token, size_t > lookahead;

   ourownparser( lexing::filereader&& read )
      : read( std::move( read )),
        lookahead( std::pair( tok_empty, 0 ))
   { } 

   void maph_forward( ); 
      // Forward for Maphoon input. 
      // It ignores comments and whitespace. 

   void cpp_forward( );
      // Forward inside C++ code.
      // Comments and whitespace are returned like separate
      // token.

   void throwsyntaxerror( const std::string& problem );
   void throwgeneralerror( const std::string& problem );
      // throw their way out of here. 
   
   std::string_view conv2view( ) const;
   size_t conv2unsigned( ) const;

   std::vector< std::string > readcpptype( );
      // Assuming that lookahead is tok_lbrace/1,
      // read the rest of the C++ type, including
      // the closing tok_rbrace/1. 

   void readheadercode( cpp::codefragment& header ); 
      // Assuming that lookahead is already tok_lbrace/1,
      // read until and including the closing tok_rbrace/1. 

   void readstandardcode( cpp::codefragment& code );
      // As above.

   cpp::nspace readspace( );

   void startsymbol( grammar& gr ); 
   void infotype( cpp::type& tp );
   void symbols( symbolcodebuilder& bld_sym );
   void parameter( parsercodebuilder& bld_parse );
   void code_h( cpp::codefragment& code_h );
   void code_cpp( cpp::codefragment& code_cpp ); 

   void rulegroup( parsercodebuilder& bld_parse ); 
      // Group of rules that share an lhs.

   symbol::map< size_t > reductionseq( );
      // Parse a line %reductionseq ...

   expectation one_expectation( );

   void print( std::ostream& ) const;
};


inline std::ostream& 
operator << ( std::ostream& out, const ourownparser& parse )
{
   parse. print( out );
   return out;
}

#endif

