
// Maphoon 2021, written by Hans de Nivelle, Jan 2021/May 2022/Feb 2023.
// Read the licence before using it. 

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "tokenizing.h" 
#include "ourownparser.h" 
#include "symbol.h"
#include "symbolset.h"

#include "parsetable.h" 
#include "grammar.h"
#include "itemsets.h"
#include "symbolcodebuilder.h"
#include "parsercodebuilder.h" 

int main( int argc, char* argv[] ) 
{
#if 0
   expectation e1 = { { { { symbol("aa"), symbol("xx") }, 4 },
                      { { symbol("bb") }, 3 }}, "something" };

   std::cout << e1 << "\n";
#endif

#if 0
   // Tests the tokenizer:

   filereader test( &std::cin, "STDIN" );
   while( true )
   {
      std::pair< token, size_t > p = tokenize::maph( test );
      std::cout << "read " << p.first << "/" << p.second << "\n";
      if( p. second == 0 )
         return 0; 
      else
      {
         test. commit( p. second );
      } 
   }
   return 0;
#endif

   // This is the real start: 

   if( argc < 2 || argc > 4 ) 
   {
      std::cout << "Hello, I need one, two or three parameters:\n";
      std::cout << "1. exact name of the .m file\n";
      std::cout << "2. directory where symbol and parser code will be placed\n";
      std::cout << "3. exact name of the idee.x file\n";
      std::cout << "   (if 2 and 3 are absent, no code will be generated)\n"; 
      std::cout << "\n";
      exit(-1);
   }

   std::string inputfilename = argv[1];
   if( inputfilename. size( ) < 3 ||
       ( inputfilename[ inputfilename. size( ) - 1 ] != 'm' &&
         inputfilename[ inputfilename. size( ) - 1 ] != 'M' ) ||
       inputfilename[ inputfilename. size( ) - 2 ] != '.' )
   {
      std::cout << "inputfile " << inputfilename << " must end in .m or .M\n";
      exit(-1);
   }

   std::ifstream inputfile( inputfilename );
   if( !inputfile )
   {
      std::cout << "could not not open " << inputfilename << " for reading\n";
      exit(-1);
   }

   symbolcodebuilder bld_sym( cpp::nspace( ), cpp::type( { "void" } )); 
   
   parsercodebuilder bld_parse = { cpp::nspace( ) };

   bool selfcheck = false;
   bool showclosures = false;

      // These options can be changed by the input.


   // Now we are ready to read the input:

   ourownparser parse( lexing::filereader( &inputfile, inputfilename ));
   parse. maph_forward( ); 

   try
   {
      while( parse.lookahead. first != tok_eof &&
             parse.lookahead. first != tok_rules ) 
      {
         switch( parse. lookahead. first )
         {
         case tok_startsymbol: 
            parse. startsymbol( bld_parse. grrr );
            break;

         case tok_infotype:
            parse. infotype( bld_sym. infotype );
            break; 

         case tok_symbol:
            parse. symbols( bld_sym );
            break;

         case tok_parameter:
            parse. parameter( bld_parse );
            break;
 
         case tok_symbolspace:
            parse. maph_forward( ); 
            bld_sym. space = parse. readspace( ); 
            break;

         case tok_parserspace:
            parse. maph_forward( );
            bld_parse. space = parse. readspace( );
            break;

         case tok_symbolcode_h:
            parse. maph_forward( ); 
            parse. code_h( bld_sym. code_h );
            break;

         case tok_symbolcode_cpp:
            parse. maph_forward( ); 
            parse. code_cpp( bld_sym. code_cpp );
            break;

         case tok_parsercode_h:
            parse. maph_forward( );
            parse. code_h( bld_parse. code_h );
            break;

         case tok_parsercode_cpp:
            parse. maph_forward( );
            parse. code_cpp( bld_parse. code_cpp );
            break;

         case tok_source:
            parse. maph_forward( );
            parse. code_cpp( bld_parse. source ); 
            bld_parse. source. filename. clear( );
            break;

         case tok_showclosures:    
            parse. maph_forward( );
            showclosures = true;
            break;

         case tok_selfcheck:
            selfcheck = true;
            parse. maph_forward( );
            break;
           
         case tok_reductionseq:
            bld_parse. redsequences. push_back( parse. reductionseq( ));
            break;

         case tok_nodefaults:
            bld_parse. usedefaults = false;
            parse. maph_forward( ); 
            break;

         case tok_usererror:
            bld_parse. create_errorfunc = false;
            parse. maph_forward( );
            break;

         default:
            std::string err = "cannot process ";
            err += parse. read. view( parse. lookahead. second );
            std::cout << parse << "\n";
            parse. throwsyntaxerror( err );  
         }
      }

      while( parse. lookahead. first == tok_rules ||
             parse. lookahead. first == tok_errors )
      {
         switch( parse. lookahead. first )
         {
         case tok_rules: 
            parse. maph_forward( );

            while( parse.lookahead. first == tok_identifier ) 
               parse. rulegroup( bld_parse ); 
            break; 

         case tok_errors:  
            {
               parse. maph_forward( );
               while( parse. lookahead. first == tok_identifier ||
                      parse. lookahead. first == tok_lpar ||
                      parse. lookahead. first == tok_rewrite )
               {
                  bld_parse. errormessages. push_back( 
                                     parse. one_expectation( ));
               }
            } 
            break; 

         default:
            throw std::runtime_error( "unreachable" ); 
         }
      } 

      if( parse. lookahead. first != tok_eof )
         parse. throwsyntaxerror( 
            "expecting end-of-file, grammar rules, or errors" );

      std::cout << parse << "\n";
   }
   catch( const syntaxerror& synt ) 
   {
      std::cout << synt << "\n\n"; 
      exit(-1);
   }
   catch( const generalerror& gen )
   {
      std::cout << gen << "\n";
      exit(-1); 
   }

   std::cout << bld_sym << "\n";
   std::cout << bld_parse << "\n";

   std::cout << "input was read succesfully\n\n";

   std::vector< std::ostringstream > errors; 
   size_t warnings = 0; 

   bld_sym. attributes. insert( 
       std::pair( symbol( "_recover_" ), cpp::type( { "void" } )));

   // There will be some big variables. We create a local scope, so that 
   // they will be cleaned up when they are no longer needed.

   {
      symbolset allsym = bld_parse. grrr. allsymbols( );

      {
         symbolset undeclared; 
         for( symbol s : allsym )
            if( !bld_sym. attributes. count(s) )
               undeclared. insert(s);  
      
         if( undeclared.size( ))
         {
            errors. push_back( std::ostringstream( )); 
            errors. back( ) << "symbols used without declaration: " << 
                         undeclared << "\n";
         }
      }

      symbolset unused;
      for( const auto& s : bld_sym. attributes )
      {
         if( !allsym. contains( s. first ))
            unused. insert( s. first );
      }

      if( unused. size( ))
      {
         std::cout << "warning: the following symbols have been declared, ";
         std::cout << "but do not occur in the grammar:\n";
         std::cout << "   " << unused << "\n";

         ++ warnings; 
      }
 
      // We will report two problems: left hand side symbols 
      // that are not reachable from a start symbol, and lookaheads 
      // that are reachable from their start symbol. Both are errors. 

      symbolset left = bld_parse. grrr. leftsymbols( );
 
      for( const auto& st : bld_parse. grrr. startsymbols )
      {
         symbolset reachable = bld_parse.grrr. reachablesymbols( st. first );

         if( bld_parse. grrr. index. find( st. first ) ==
             bld_parse. grrr. index. end( )) 
         {
            std::cout << "start symbol " << st. first << " is not used\n";
            ++ warnings;
         }

         symbolset rr = intersection( st. second, reachable );
         if( rr. size( ))
         {
            errors. push_back( std::ostringstream( )); 
            errors. back( ) << 
                      "there are reachable symbols in the terminator set ";
            errors. back( ) << "of start symbol " ;
            errors. back( ) << st. first << " :  ";
            errors. back( ) << rr << "\n";
         }

         left. remove( reachable );
            // Becausey the are reachable from st. first.             
      }

      if( left. size( ))
      {
         std::cout << "warning: there are unreachable left hand side symbols: ";
         std::cout << left << "\n";
         ++ warnings; 
      }
   }

   // We check for attributes that are declared with a reference type.
   // We warn for it.

   for( const auto& s : bld_sym. attributes )
   {
      if( s. second. isref( ))
      {
         std::cout << "warning: symbol " << s. first;
         std::cout << " is declared with a reference type:  ";
         std::cout << s. second << "\n";

         ++ warnings; 
      } 

      if( s. second. isconst( ))
      {
         std::cout << "warning: symbol " << s. first;
         std::cout << " is declared with a const type:  ";
         std::cout << s. second << "\n";

         ++ warnings;
      }
   } 

   // We check for parameters that are declared with a reference type,
   // and warn if we find some. 

   for( const auto& par : bld_parse. parameters )
   {
      if( par. second. isref( ))
      {
         std::cout << "warning: parameter " << par. first;
         std::cout << " is declared with a reference type:  ";
         std::cout << par. second << "\n"; 

         ++ warnings;
      }
   }

   // We check that _recover_ is never the last symbol in a rule:

   for( const auto& r : bld_parse. grrr. rules )
   {
      if( r. rhs. size( ))
      {
         if( r. rhs. back( ) == symbol( "_recover_" ))
         {
            errors. push_back( std::ostringstream( ));
            errors. back( ) <<
                      "rule contains _recover_ as last symbol in rhs:\n";
            errors. back( ) << "   " << r << "\n";
         }
      }
   }

   // We check that all symbols occurring in the reduction sequences
   // have a declaration. We warn if some of them are undeclared: 

   {
      symbolset undeclared; 

      for( const auto& m : bld_parse. redsequences )
      {
         for( const auto& p : m )
         {
            if( !bld_sym. attributes. count( p. first ))
               undeclared. insert( p. first );  
         }
      }

      if( undeclared. size( ))
      {
         std::cout << "\n";
         std::cout << 
            "warning: there are undeclared symbols used in a %reductionseq\n";
         std::cout << "   { ";
         for( auto p = undeclared. begin( ); p != undeclared. end( ); ++ p )
         {
            if( p != undeclared. begin( ))
               std::cout << ", ";
            std::cout << *p;
         }
         std::cout << " }\n\n";

         ++ warnings; 
      } 
   }

   // We check that all symbols occurring in error rules, 
   // have a declaration: 

   {
      symbolset undeclared;

      for( const auto& e : bld_parse. errormessages )
      {
         for( const auto& t : e. trigger )
         {
            for( const auto& s : t. set )
            {
               if( !bld_sym. attributes. count(s))
                  undeclared. insert(s);  
            }
         }
      }

      if( undeclared. size( ))
      {
         std::cout << "\n";
         errors. push_back( std::ostringstream( ));
         errors. back( ) << "error rules contain undeclared symbols: "; 
         errors. back( ) << undeclared << "\n";
      } 
   }

   parsetable table( bld_parse. grrr );

   table. close( bld_parse. grrr, showclosures ); 

   std::cout << "\n";
   std::cout << table << "\n";

   if( selfcheck )
      table. selfcheck( );

   if( warnings > 0 )
   {
      std::cout << "note that there are warnings\n\n";
      warnings = 0; 
   }

   if( errors. size( ))
   {
      std::cout << "quitting because of errors:\n";
      for( const auto& err: errors )
         std::cout << "   " << err. str( ) << "\n";
      std::cout << "\n";
      return 1;
   }

   if( argc >= 3 ) 
   {
      std::string symbolfilename = argv[2];
      if( symbolfilename. back( ) != '/' )
         symbolfilename. push_back( '/' );

      symbolfilename += "symbol";
      std::cout << "symbol definition will be put in ";
      std::cout << symbolfilename << ".h" << " and ";
      std::cout << symbolfilename << ".cpp\n\n";
    
      {
         std::ofstream hh( symbolfilename + ".h" );
         if( hh ) 
            bld_sym. print_h_file( hh ); 
         else
            std::cout << "could not open " << symbolfilename << ".h\n";
      }

      {
         std::ofstream cpp( symbolfilename + ".cpp" );
         if( cpp )
            bld_sym. print_cpp_file( cpp );
         else
            std::cout << "could not open " << symbolfilename << ".cpp\n";
      }
   }

   if( argc == 4 )
   {
      // We check for non-trivial attributes that have a rule without
      // reduction code. This is an error, because we will not know
      // how to generate the attribute during a reduction. 

      for( const auto& s : bld_sym. attributes )
      {
         if( !s. second. isvoid( ))
         {
            const grammar& gr = bld_parse. grrr;
            auto p = gr. index. find( s. first );
            if( p != gr. index. end( ))
            {
               size_t count = 0;
               for( size_t r : p -> second )
               {
                  if( bld_parse. reductions[r]. body. isempty( ))
                     ++ count;
               }

               if( count )
               {
                  errors. push_back( std::ostringstream( ));
                  errors. back( ) << "symbol " << s. first;
                  errors. back( ) << " is declared with nontrivial attribute ";
                  errors. back( ) << s. second << ", ";
                  errors. back( ) << "but " << count << " of its reductions ";
                  errors. back( ) << "have no action code";
               }
            }
         }
      }  

      // We check for a source definition. If there is no, 
      // we produce an error.

      if( bld_parse. source. isempty( ))
      {
         errors. push_back( std::ostringstream( ));
         errors. back( ) << "no input source specified (use %source)\n";
      }

      std::string ideefilename = argv[3];
      if( ideefilename. back( ) != '/' )
         ideefilename. push_back( '/' );
   
      ideefilename += "idee.x";
      std::cout << "opening file " << ideefilename << "\n";
      std::ifstream idee( ideefilename );
      if( !idee )
      {
         errors. push_back( std::ostringstream( )); 
         errors. back( ) << 
                    "could not open " << ideefilename << " for reading\n";
      }

      std::string parserfilename = argv[2];
      if( parserfilename. back( ) != '/' )
         parserfilename. push_back( '/' );

      parserfilename += "parser";
      std::cout << "parser definition will be put in ";
      std::cout << parserfilename << ".h" << " and ";
      std::cout << parserfilename << ".cpp\n\n";

      std::ofstream hh( parserfilename + ".h" );
      if( !hh )
      {
         errors. push_back( std::ostringstream( )); 
         errors. back( ) << "could not open " << parserfilename << ".h\n";
      }

      std::ofstream cpp( parserfilename + ".cpp" );
      if( !cpp )
      {
         errors. push_back( std::ostringstream( )); 
         errors. back( ) << "could not open " << parserfilename << ".cpp\n";
      }

      if( errors. size( ))
      {
         std::cout << "quitting because of errors:\n";
         for( const auto& err: errors )
            std::cout << "   " << err. str( ) << "\n";
         std::cout << "\n";
         return -1;
      }

      std::vector< conflict > conflicts;
         // Our opinion about conflicts is that they should not
         // happen, but since it is still possible to create
         // a parser, we should.

      try 
      {   
         bld_parse. printcode( bld_sym, table, conflicts, idee, cpp, hh );  
         if( conflicts. size( ))
         {
            std::cout << "problems during construction of the parse table:\n\n";

            for( const auto& c : conflicts )
               std::cout << c << "\n";
         }   
      }
      catch( const ideeproblem& pr )
      {
         std::cout << pr << "\n";
         exit(-1);
      }
   } 

   return 0;
      // This is the normal end. 
}


