
// Written by Hans de Nivelle, Spring 2021.

#ifndef MAPH_TOKENIZING_
#define MAPH_TOKENIZING_  1

#include <iostream> 
#include <map>

#include "indentation.h"
#include "lexing2023/includes.h"


enum token { tok_rewrite,         // =>
             tok_bar,             // |
             tok_colon,           // :
             tok_semicolon,       // ;
 
             tok_symbol,          // %symbol 
             tok_startsymbol,     // %startsymbol 
             tok_infotype,        // %infotype
             tok_parameter,       // %parameter
             tok_symbolspace,     // %symbolspace
             tok_parserspace,     // %parserspace 
             tok_symbolcode_h,    // %symbolcode_h
             tok_symbolcode_cpp,  // %symbolcode_cpp
             tok_parsercode_h,    // %parsercode_h
             tok_parsercode_cpp,  // %parsercode_cpp
             tok_source,          // %source
             tok_nodefaults,      // %nodefaults
             tok_showclosures,    // %showclosures
             tok_selfcheck,       // %selfcheck 
             tok_reductionseq,    // %reductionseq
             tok_usererror,       // %usererror
             tok_rules,           // %rules
             tok_requires,        // %requires
             tok_reduces,         // %reduces
             tok_errors,          // %errors 
             tok_recover,         // _recover_ 

             tok_lpar,
             tok_rpar,
             tok_lbrace, 
             tok_rbrace,
             tok_lbracket,
             tok_rbracket,

             tok_lt,              //  < 
             tok_gt,              //  > 
             tok_separator,       //  :: 
             tok_star,            //  * 
             tok_slash,           //  /  
             tok_ampersand,
             tok_comma, 
             tok_dot, 

             tok_identifier,      // A C++ identifier. 
             tok_unsigned, 
             tok_double,     
             tok_wrongdouble,   // more informative, than just 'error'. 
             tok_string,
             tok_wrongstring,   // more informative, than just 'error'.

             tok_cpp_include,   // The C++ preprocessor #include 
             tok_cpp_define,    // The C++ preprocessor #define 
             tok_cpp_operator,  // All remaining operators that have no 
                                // special meaning for us. 

             tok_scanerror,
             tok_comment, 
             tok_wrongcomment,
             tok_skip,
             tok_wrongskip, 
             tok_whitespace,
             tok_empty,         // The empty token has zero length. 
             tok_bad,           // The input source is bad. 
             tok_eof };


const char* getcstring( token tok );

inline std::ostream& operator << ( std::ostream& out, token tok )
{
   out << getcstring( tok ); return out;
}

inline 
std::ostream& operator << ( std::ostream& out, 
                            std::pair< token, size_t > t )
{
   return out << t. first << "/" << t. second; 
}

namespace tokenize
{

   std::pair< token, size_t > maph( lexing::filereader& read );
   std::pair< token, size_t > cpp( lexing::filereader& read );
      // I think that inside C++ code, the tokens are always the 
      // same. It doesn't matter if it is a type definition, an include,
      // or action code. 
      // cpp does not ignore whitespace or comments, but returns them.
      // We tokenize C++ code, so that we can normalize types (somewhat),
      // and identify identifiers that are subject to replacement.

}

#endif 

