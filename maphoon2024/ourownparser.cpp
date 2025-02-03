
// Written by Hans de Nivelle, spring 2021.

#include "ourownparser.h" 
#include "errors.h"

void ourownparser::maph_forward( )
{
   read. commit( lookahead. second );
   lookahead = tokenize::maph( read );
}

void ourownparser::cpp_forward( )
{
   read. commit( lookahead. second );
   lookahead = tokenize::cpp( read );
}

void ourownparser::throwsyntaxerror( const std::string& problem )
{
   throw syntaxerror( problem, read.filename, read.line, read.column );
}

void ourownparser::throwgeneralerror( const std::string& problem )
{
   throw generalerror( problem, read.filename, read.line, read.column );
}

std::string_view ourownparser::conv2view( ) const
{
   return read. view( lookahead. second );
}

size_t ourownparser::conv2unsigned( ) const 
{
   return std::stoul( std::string( read. view( lookahead. second )));
}

std::vector< std::string > ourownparser::readcpptype( )
{
   std::vector< std::string > tp; 

   if( lookahead. first != tok_lbrace ) 
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );
 
   size_t depth = 1;
      // Nesting depth of { and }. We already have read one { .

   while( true )
   {
      switch( lookahead. first ) 
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( ); 
            return tp; 
         }
         cpp_forward( ); 
         tp. push_back( "}" );       
         break;

      case tok_lbrace:
         ++ depth;
         tp. push_back( "{" );
         cpp_forward( );
         break; 

      case tok_whitespace:
      case tok_comment: 
         cpp_forward( ); 
         break;

      case tok_identifier:
      case tok_separator:
      case tok_lt:
      case tok_gt:
      case tok_star:
      case tok_ampersand: 
      case tok_comma: 
         tp. push_back( std::string( read. view( lookahead. second )));
         cpp_forward( ); 
         break; 
       
      default:
         throwsyntaxerror( std::string( getcstring( lookahead.first )) +
                           " " + 
			   std::string( conv2view( )) + 
                           " not allowed in C++ type" );

      }
   }
}


void ourownparser::readheadercode( cpp::codefragment& header ) 
{
   if( lookahead. first != tok_lbrace )
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );

   size_t depth = 1;
      // Nesting depth of { and }. We already have read one { .

   while( true )
   {
      switch( lookahead. first )
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( );
            return;
         }
         cpp_forward( );
         header. push_back( "}" );
         break;

      case tok_lbrace:
         ++ depth;
         header. push_back( "{" );
         cpp_forward( );
         break;

      case tok_lpar:
      case tok_rpar:

      case tok_whitespace:
      case tok_comment:
         header. push_back( conv2view( )); 
         cpp_forward( );
         break;

      case tok_lt:
      case tok_gt:
      case tok_cpp_include:
      case tok_string: 
      case tok_identifier: 
      case tok_comma:
      case tok_dot: 
      case tok_separator:
      case tok_colon:
      case tok_semicolon:
      case tok_star:
      case tok_slash: 
      case tok_ampersand:
         header. push_back( conv2view( ));
         cpp_forward( );
         break;

      default:
         throwsyntaxerror( std::string( "forbidden use of " ) + 
                           std::string( read. view( lookahead. second ))
                            + " in header code" );
      }
   }
}


void ourownparser::readstandardcode( cpp::codefragment& code ) 
{
   unsigned int startline = read.line;
   unsigned int startcolumn = read.column;

   if( lookahead. first != tok_lbrace ) 
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );
 
   size_t depth = 1;
      // Nesting depth of { and }. We have already read one { .

   while( true )
   {
      switch( lookahead. first ) 
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( ); 
            return; 
         }
         cpp_forward( ); 
         code. push_back( "}" );       
         break;

      case tok_lbrace:
         ++ depth;
         code. push_back( "{" );
         cpp_forward( );
         break; 

      case tok_whitespace:
      case tok_comment: 
         code. push_back( std::string( conv2view( ))); 
         cpp_forward( ); 
         break;

      case tok_identifier:
         code. push_back( std::string( conv2view( )), true );
         cpp_forward( );
         break;

      case tok_lpar:
      case tok_rpar:
      case tok_lbracket:
      case tok_rbracket:
         code. push_back( std::string( conv2view( )));
         cpp_forward( );
         break;

      case tok_string:
      case tok_double: 
         code. push_back( std::string( conv2view( )));
         cpp_forward( );
         break;

      case tok_separator:
      case tok_lt:
      case tok_gt:
      case tok_star:
      case tok_slash: 
      case tok_ampersand: 
      case tok_comma: 
      case tok_dot: 
      case tok_colon:
      case tok_semicolon:
      case tok_cpp_operator: 
         code. push_back( std::string( conv2view( )));
         cpp_forward( ); 
         break; 
       
      default:
         throwsyntaxerror( std::string( getcstring( lookahead.first )) + 
                           " " + 
                           std::string( conv2view( )) + 
                           " not allowed in code block (starting at " + 
                           std::to_string(startline+1) + "/" + 
                           std::to_string(startcolumn+1) + ")" );

      }
   }
}

cpp::nspace ourownparser::readspace( )
{
   cpp::nspace res;

   if( lookahead. first != tok_identifier ) 
      return res;

   res += std::string( conv2view( ));
   maph_forward( );

   while( lookahead. first == tok_separator )
   {
      maph_forward( );
      if( lookahead. first != tok_identifier )
         throwsyntaxerror( "expecting identifier for namespace" );
      
      res += std::string( conv2view( ));
      maph_forward( ); 
   }

   return res; 
}

void ourownparser::startsymbol( grammar& gr )
{ 
   if( lookahead. first != tok_startsymbol )
      throw std::runtime_error( "lookahead must be startsym" );

   maph_forward( );

   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "identifier expected" ); 

   symbol startsym = std::string( conv2view( ));
   maph_forward( );
 
   symbolset terminators;
   while( lookahead. first == tok_identifier )
   {
      terminators. insert( symbol( std::string( conv2view( )))); 
      maph_forward( );
   }

   gr. addstartsym( startsym, terminators );
}

void ourownparser::infotype( cpp::type& tp )
{
   if( lookahead. first != tok_infotype )
      throw std::runtime_error( "should be infotype" );

   maph_forward( );
   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   std::vector< std::string > str = readcpptype( );
   if( str. size( ) == 0 )
      str. push_back( "void" );
 
   tp = std::move( str );
}

void ourownparser::symbols( symbolcodebuilder& bld_sym )
{
   if( lookahead. first != tok_symbol )
      throw std::runtime_error( "should have been symbol" );

   maph_forward( ); 

   std::vector< std::string > str;

   if( lookahead. first == tok_lbrace )
      str = readcpptype( );

   if( str. size( ) == 0 )
      str. push_back( "void" );
   
   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "there should be at least one declared symbol" );

   cpp::type thetype = str;

   while( lookahead. first == tok_identifier )
   {
      if( !bld_sym. addsymbol( std::string( conv2view( )), thetype ))
      {
         throwgeneralerror( std::string( "redeclaration of symbol " ) +
                            std::string( conv2view( )) );
      }

      maph_forward( ); 
   }
}

void ourownparser::parameter( parsercodebuilder& bld_parse )
{
   if( lookahead. first != tok_parameter )
      throw std::runtime_error( "should have been parameter" );

   maph_forward( );

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "parameters cannot be declared without type" );

   std::vector< std::string > str = readcpptype( ); 
   if( str. size( ) == 0 )
      str. push_back( "void" );

   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "parameter name expected" );

   bld_parse. parameters. push_back( 
      std::pair( std::string( conv2view( )), cpp::type( str ))); 

   maph_forward( );
}


void ourownparser::code_h( cpp::codefragment& code_h ) 
{
   if( code_h. size( ) == 0 )
   {
      code_h. filename = read. filename;
      code_h. line = read. line;
      code_h. column = 0;
   }

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   readheadercode( code_h );
   code_h. push_back( "\n" );
}


void ourownparser::code_cpp( cpp::codefragment& code_cpp )
{
   if( code_cpp. size( ) == 0 )
   {
      code_cpp. filename = read. filename;
      code_cpp. line = read. line;
      code_cpp. column = 0;
   }

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   readstandardcode( code_cpp );
   code_cpp. push_back( "\n" );
}


void ourownparser::rulegroup( parsercodebuilder& bld_parse )
{
   if( lookahead. first != tok_identifier )
      throw std::runtime_error( "grammar rule must start with lhs" );

   symbol lhs = std::string( conv2view( ));
   size_t nrrules = 0; 
      // nr of rules that share this lhs.

   maph_forward( );
   while( lookahead. first != tok_semicolon )
   {
      if( nrrules == 0 && lookahead. first != tok_rewrite )
      {
         throwsyntaxerror( "=> expected" );
      }

      if( nrrules != 0 &&
          lookahead. first != tok_rewrite &&
          lookahead. first != tok_bar )
      { 
         throwsyntaxerror( "| or => expected" );
      }

      maph_forward( );

      std::vector< symbol > rhs;
      std::unordered_map< std::string, size_t > parameters;

      while( lookahead. first == tok_identifier ||
             lookahead. first == tok_recover ) 
      {
         symbol right = std::string( conv2view( ));
         maph_forward( );

         if( lookahead. first == tok_colon )
         {
            maph_forward( );
            if( lookahead. first != tok_identifier )
            {
               throwsyntaxerror( "attribute identifier expected" );
            }

            auto p = parameters. insert( 
                        std::pair( std::string( conv2view( )), rhs. size( )));     

            if( !p. second )
            {
               throwgeneralerror( std::string( "attribute name " ) + 
                                  std::string( conv2view( )) + 
                                  std::string( " is reused" ));
            }
            maph_forward( );
         }

         rhs. push_back( right );
      }

      cpp::codefragment cond;
      
      if( lookahead. first == tok_requires )
      {
         maph_forward( );
         if( lookahead. first != tok_lbrace )
            throwsyntaxerror( "expecting { after %requires" );
 
         cond. filename = read. filename;
         cond. line = read. line;
         cond. column = read. column;

         readstandardcode( cond ); 
      }
 
      cpp::codefragment body;

      if( lookahead. first == tok_reduces ) 
         maph_forward( );

      if( lookahead. first == tok_lbrace ) 
      {
         body. filename = read. filename;
         body. line = read. line;
         body. column = read. column;

         readstandardcode( body ); 
      }

      if( lookahead. first != tok_bar &&
          lookahead. first != tok_rewrite &&
          lookahead. first != tok_semicolon )
      {
         throwsyntaxerror( 
            std::string( "unexpected symbol " ) + 
            std::string( conv2view( )) +
            " in rhs of grammar rule" );
      }
 
      bld_parse. grrr. push_back( 
            rule( bld_parse. grrr. nrrules( ), lhs, std::move( rhs )));

      bld_parse. reductions. emplace_back( std::move( parameters ),
                                           std::move( cond ),
                                           std::move( body ));
      ++ nrrules;
   }

   maph_forward( );   // We know it's a semicolon.

}


symbol::map< size_t > ourownparser::reductionseq( )
{
   if( lookahead. first != tok_reductionseq )
      throw std::runtime_error( "first should have been reductionseq" );

   maph_forward( );

   symbol::map< size_t > result;

   while( lookahead. first == tok_identifier )
   {
      symbol s = std::string( conv2view( ));

      if( !result. insert( { s, result. size( ) } ). second ) 
      {
         throwgeneralerror( std::string( "symbol " ) +
                            std::string( conv2view( )) + 
                            " occurs twice in the same reduction sequence" );    
      }

      maph_forward( );  
   }

   return result;
}

expectation ourownparser::one_expectation( )
{
   std::vector< expectation::seenonstack > trigger; 

   while( lookahead. first == tok_lpar ||
          lookahead. first == tok_identifier )
   {
      symbolset set;
      if( lookahead. first == tok_identifier )
      {
         set. insert( symbol( std::string( conv2view( ))));
         maph_forward( );
      }
      else
      {
         maph_forward( ); 

         if( lookahead. first != tok_identifier )
         {
            throwsyntaxerror( "expecting a symbol in error description" );
         }

         set. insert( symbol( std::string( conv2view( ))));
         maph_forward( );
         
         while( lookahead. first == tok_bar )
         {
            maph_forward( );
            if( lookahead. first != tok_identifier )
            {
               throwsyntaxerror( "expecting a symbol in error description" );
            }

            set. insert( symbol( std::string( conv2view( )))); 
            maph_forward( ); 
         }

         if( lookahead. first != tok_rpar )
         {
            throwsyntaxerror( "expecting | or ) in error description" );
         }

         maph_forward( );
      } 

      size_t maxskip = 0;
      while( lookahead. first == tok_star ||
             lookahead. first == tok_unsigned )
      {
         if( lookahead. first == tok_star )
         {
            ++ maxskip;
            maph_forward( ); 
         }
         else
         {
            maxskip += conv2unsigned( );
            maph_forward( );  
         }
      }

      trigger. push_back( expectation::seenonstack( std::move( set ), maxskip ));
   }

   if( lookahead. first != tok_rewrite )
   {
      throwsyntaxerror( "expecting => in error description" );
   }

   maph_forward( );
   
   if( lookahead. first != tok_string )
   {
      throwsyntaxerror( "expecting message (string) in error description" );
   }

   std::string explanation = std::string( conv2view( ));

   maph_forward( );

   if( lookahead. first != tok_semicolon )
   {
      throwsyntaxerror( "expecting ; after error description" );
   }

   maph_forward( ); 

   return expectation( std::move( trigger ), std::move( explanation ));    
}


void ourownparser::print( std::ostream& out ) const
{
   out << read;
   out << "lookahead: " << lookahead << "\n";
}

