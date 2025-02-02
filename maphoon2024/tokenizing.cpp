
// Written by Hans de Nivelle, February 2021.
// Rewritten, using automata, in May 2022.

#include "tokenizing.h"

const char* getcstring( token tok )
{
   switch( tok )
   {
   case tok_rewrite:         return "rewrite";
   case tok_bar:             return "bar";
   case tok_colon:           return "colon";
   case tok_semicolon:       return "semicolon";

   case tok_symbol:          return "%symbol";
   case tok_startsymbol:     return "%startsymbol";
   case tok_infotype:        return "%infotype";
   case tok_parameter:       return "%parameter";
   case tok_symbolspace:     return "%symbolspace";
   case tok_parserspace:     return "%parserspace";
   case tok_symbolcode_h:    return "%symbolcode_h"; 
   case tok_symbolcode_cpp:  return "%symbolcode_cpp"; 
   case tok_parsercode_h:    return "%parsercode_h";
   case tok_parsercode_cpp:  return "%parsercode_cpp";
   case tok_source:          return "%source"; 
   case tok_nodefaults:      return "%nodefaults"; 
   case tok_showclosures:    return "%showclosures"; 
   case tok_selfcheck:       return "%selfcheck"; 
   case tok_reductionseq:    return "%reductionseq"; 
   case tok_usererror:       return "%usererror";
   case tok_rules:           return "%rules";
   case tok_requires:        return "%requires";
   case tok_reduces:         return "%reduces";
   case tok_errors:          return "%errors";
   case tok_recover:         return "recover";

   case tok_lpar:            return "lpar";
   case tok_rpar:            return "rpar";
   case tok_lbrace:          return "lbrace";
   case tok_rbrace:          return "rbrace";
   case tok_lbracket:        return "lbracket";
   case tok_rbracket:        return "rbracket";

   case tok_lt:              return "lt";
   case tok_gt:              return "gt";
   case tok_separator:       return "separator";
   case tok_star:            return "star";
   case tok_slash:           return "slash"; 
   case tok_ampersand:       return "ampersand";
   case tok_comma:           return "comma";
   case tok_dot:             return "dot"; 

   case tok_identifier:      return "identifier"; 
   case tok_unsigned:        return "unsigned";
   case tok_double:          return "double";
   case tok_wrongdouble:     return "wrongdouble"; 
   case tok_string:          return "string";
   case tok_wrongstring:     return "wrongstring";

   case tok_cpp_include:     return "cpp_include";
   case tok_cpp_define:      return "cpp_define";
   case tok_cpp_operator:    return "cpp_operator";

   case tok_scanerror:       return "scanerror";
   case tok_comment:         return "comment";
   case tok_wrongcomment:    return "wrongcomment"; 
   case tok_skip:            return "skip";
   case tok_wrongskip:       return "wrongskip"; 
   case tok_whitespace:      return "whitespace";
   case tok_empty:           return "empty"; 
   case tok_bad:             return "bad"; 
   case tok_eof:             return "eof";
   }

   return "???";
}


namespace
{

   void comment( lexing::classifier< char, token > & acc )
   {
      using namespace lexing;

      acc. insert( word( "//" ) * ( every<char>( ). without( '\n' )). star( ) 
                   * just( '\n' ), tok_comment );

      acc. insert(  word( "/*" ) * 
            ( every<char>( ). without( '*' ) |
              just( '*' ). plus() * every<char> (). without( '/' ). without('*')). star() *
            just( '*' ). star( ) * word( "*/" ), tok_comment ); 

      acc. insert(  word( "/*" ) *
            ( every<char>( ). without( '*' ) |
              just( '*' ). plus() * every<char> (). without( '/' ). without( '*')). star(),
         tok_wrongcomment ); 
   }
 
   lexing::acceptor< char > skip( ) 
   {
      using namespace lexing;
 
      auto endskip = std::string_view( "%endskip" );

      auto inside = empty<char> ( );  
      for( unsigned int i = 0; i < endskip. size( ); ++ i )
      {
         acceptor<char> opt; 

         for( unsigned int j = 0; j < i; ++ j )
         {
            opt *= just( endskip[j] );
         }
         opt *= every<char> ( ). without( endskip[i] );

         inside |= opt;  
      }

      return word( "%skip" ) * inside. star( ) * 
             word( endskip. begin( ), endskip. end( )); 
   }
 
   // String or character constant:

   lexing::acceptor< char > quoted( )
   {
      using namespace lexing;

      auto hexdigit =
      range( '0', '9' ) | range( 'a', 'f' ) | range( 'A', 'F' );

      auto escape = word( "\\" ) * range( '0', '7' ) | 
                    word( "\\" ) * range( '0', '7' ) * range( '0', '7' ) |
                    word( "\\" ) * range( '0', '3' ) * range( '0', '7' ) * range( '0', '7' ) | 
                    word( "\\n" ) |
                    word( "\\t" ) | word( "\\v" ) |
                    word( "\\r" ) | word( "\\f" ) |
                    word( "\\\'" ) | word( "\\\"" ) |
                    word( "\\\\" ) | ( word( "\\x" ) * hexdigit * hexdigit );

      auto onechar = range( 'a', 'z' ) | range( 'A', 'Z' ) | range( '0', '9' );

      std::string allowedinstring = "`,.+-*/#$%:;=_!?|&()[]{}<>^@~ \t";

      for( char c : allowedinstring )
         onechar |= just(c);

      return 
         just( '\'' ) * ( onechar | escape | just( '"' )) * just( '\'' ) 
            |
         just( '"' ) * ( onechar | escape | just( '\'' ) ). star( ) * just( '"' );
   }


   lexing::classifier< char, token > maph_classifier( )
   {
      using namespace lexing; 

      classifier< char, token > cls( tok_scanerror ); 

      auto letter = range( 'a', 'z' ) | range( 'A', 'Z' );
      auto digit = range( '0', '9' );

      cls. insert( just( '{' ), tok_lbrace );
      cls. insert( just( '}' ), tok_rbrace );

      cls. insert( word( "::" ), tok_separator );
     
      cls. insert( word( "=>" ), tok_rewrite ); 
      cls. insert( just( '|' ),  tok_bar );
      cls. insert( just( ':' ),  tok_colon );
      cls. insert( just( ';' ),  tok_semicolon );

      cls. insert( just( '*' ),  tok_star );
      cls. insert( just( '(' ),  tok_lpar );
      cls. insert( just( ')' ),  tok_rpar );

      cls. insert( ( letter | just( '_' )) * 
                   ( letter | digit | just( '_' )). star( ), tok_identifier );

      cls. insert( digit * digit. optional( ) * 
                   digit. optional( ) * digit. optional( ) *
                   digit. optional( ) * digit. optional( ) *
                   digit. optional( ) * digit. optional( ) * 
                   digit. optional( ), tok_unsigned ); 

      cls. insert( word( "%symbol" ), tok_symbol );
      cls. insert( word( "%startsymbol" ), tok_startsymbol );
      cls. insert( word( "%infotype" ), tok_infotype ); 
      cls. insert( word( "%parameter" ), tok_parameter );
      cls. insert( word( "%symbolspace" ), tok_symbolspace );
      cls. insert( word( "%parserspace" ), tok_parserspace );
      cls. insert( word( "%symbolcode_h" ), tok_symbolcode_h );
      cls. insert( word( "%symbolcode_cpp" ), tok_symbolcode_cpp );
      cls. insert( word( "%parsercode_h" ), tok_parsercode_h );
      cls. insert( word( "%parsercode_cpp" ), tok_parsercode_cpp );
      cls. insert( word( "%source" ), tok_source );
      cls. insert( word( "%nodefaults" ), tok_nodefaults );
      cls. insert( word( "%showclosures" ), tok_showclosures );
      cls. insert( word( "%selfcheck" ), tok_selfcheck ); 
      cls. insert( word( "%reductionseq" ), tok_reductionseq );
      cls. insert( word( "%usererror" ), tok_usererror );
      cls. insert( word( "%rules" ), tok_rules );
      cls. insert( word( "%requires" ), tok_requires ); 
      cls. insert( word( "%reduces" ), tok_reduces ); 
      cls. insert( word( "%errors" ), tok_errors ); 
      cls. insert( word( "%end" ), tok_eof ); 

      cls. insert( word( "_recover_" ), tok_recover ); 

      cls. insert( quoted( ), tok_string );

      cls. insert( ( just( ' ' ) | just( '\t') | just( '\n' ) | 
                     just( '\v' ) | just( '\f' ) | just( '\r' )). plus( ), 
                   tok_whitespace );

      comment( cls );
      cls. insert( skip( ), tok_skip );

      return minimize( make_deterministic( cls ));
   }


   lexing::classifier< char, token > cpp_classifier( ) 
   {
      using namespace lexing;

      classifier< char, token > cls( tok_scanerror );

      auto letter = range( 'a', 'z' ) | range( 'A', 'Z' );
      auto digit = range( '0', '9' );

      cls. insert( ( letter | just( '_' )) *
                   ( letter | digit | just( '_' )). star( ), tok_identifier );

      cls. insert( just( '(' ), tok_lpar );
      cls. insert( just( ')' ), tok_rpar );
      cls. insert( just( '{' ), tok_lbrace );
      cls. insert( just( '}' ), tok_rbrace );
      cls. insert( just( '[' ), tok_lbracket );
      cls. insert( just( ']' ), tok_rbracket ); 

      cls. insert( word( "::" ), tok_separator );
      cls. insert( just( '<' ), tok_lt );
      cls. insert( just( '>' ), tok_gt );
      cls. insert( just( '*' ), tok_star );
      cls. insert( just( '/' ), tok_slash );
      cls. insert( just( '&' ), tok_ampersand );

      cls. insert( just( ':' ), tok_colon );
      cls. insert( just( ';' ), tok_semicolon );

      cls. insert( just( ',' ), tok_comma );
      cls. insert( just( '.' ), tok_dot );

      // All the other operators are just 'tok_cpp_operator':

      cls. insert( just( '+' ), tok_cpp_operator );
      cls. insert( just( '-' ), tok_cpp_operator );
      cls. insert( word( "++" ), tok_cpp_operator );
      cls. insert( word( "--" ), tok_cpp_operator );
      cls. insert( just( '%' ), tok_cpp_operator );
      cls. insert( just( '?' ), tok_cpp_operator ); 
      cls. insert( word( "==" ), tok_cpp_operator ); 
      cls. insert( word( "!=" ), tok_cpp_operator );
      cls. insert( just( '|' ), tok_cpp_operator );
      cls. insert( just( '!' ), tok_cpp_operator );
 
      cls. insert( just( '=' ), tok_cpp_operator );

      auto optsign = ( just( '-' ) | just( '+' )). optional( ); 

      auto exp = ( just( 'e' ) | just( 'E' )) *
                   ( just( '-' ) | just( '+' )). optional( ) *
                   digit * digit. optional( ) * digit. optional( ); 

      cls. insert( optsign * digit. plus( ) * exp. optional( ), tok_double );
      cls. insert( optsign * digit. plus( ) * just( '.' ) * 
                   digit. star( ) * exp. optional( ), tok_double );
      cls. insert( optsign * digit. star( ) * just( '.' ) *  
                   digit. plus( ) * exp. optional( ), tok_double ); 

      cls. insert( word( "#include" ), tok_cpp_include ); 
      cls. insert( word( "#define" ), tok_cpp_define ); 

      cls. insert( quoted( ), tok_string );

      cls. insert( ( just( ' ' ) | just( '\t' ) | just( '\n' ) |
                     just( '\v' ) | just( '\f' ) | just( '\r' )). plus( ), 
                   tok_whitespace );

      comment( cls );

      return minimize( make_deterministic( cls )); 
   }

}


std::pair< token, size_t > tokenize::maph( lexing::filereader& read )
{
   static auto cls = maph_classifier( );

restart: 
   if( !read. has(1))
      return std::pair( tok_eof, 0 );

   if( !read. good( ))
      return std::pair( tok_bad, 0 );

   auto p = readandclassify( cls, read );

   if( p. first == tok_whitespace || p. first == tok_comment ||
       p. first == tok_skip ) 
   {
      read. commit( p. second );
      goto restart;
   }

   return p; 
}


std::pair< token, size_t > tokenize::cpp( lexing::filereader& read )
{
   static auto cls = cpp_classifier( );

   if( !read. has(1))
      return std::pair( tok_eof, 0 );

   if( !read. good( ))
      return std::pair( tok_bad, 0 );

   auto p = readandclassify( cls, read );

   if( p. second ) 
      return p; 
   
   return std::pair( tok_empty, 0 );
}


