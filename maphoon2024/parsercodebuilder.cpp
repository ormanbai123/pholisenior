
// Code written by Hans de Nivelle, February 2021.

#include <algorithm>
#include <sstream>

#include "parsercodebuilder.h" 
#include "errors.h"
#include "tablecodebuilder.h"


void parsercodebuilder::reduction::print( std::ostream& out ) const 
{
   out << "reduction";
   for( auto p = parameters. begin( ); p != parameters. end( ); ++ p ) 
   {
      if( p != parameters. begin( )) 
         out << ", ";
      else
         out << "( ";
      out << ( p -> first ) << " : " << ( p -> second );
   }
   
   if( parameters. size( ))
      out << " )"; 
   out << "\n";
   if( !cond. isempty( )) 
      out << "? " << cond << "\n";
   if( !body. isempty( )) 
      out << body << "\n";
}


namespace
{
   // A simplified version of filereader.

   void readchar( std::istream& idee, long unsigned int line, std::string& s ) 
   {
      s. push_back( idee. get( )); 
      if( !idee.good( ))  
         throw ideeproblem( "could not read", line );
   }

   void setideeline( std::ostream& out, long unsigned int line )
   {
      out << "#line " << (line+1) << " \"idee.x\"\n";
   }

}


void 
parsercodebuilder::printcode( const symbolcodebuilder& bld_sym,
                              const parsetable& pt, 
                              std::vector< conflict > & conflicts, 
                              std::istream& idee,
                              std::ostream& cpp, std::ostream& hh ) const
{
   std::ostream* dest = nullptr;
   long unsigned int line = 0;      // Line in idee.x file. 

   while( true )
   {
      std::string inp; 
      readchar( idee, line, inp );  // Read and append one character. 

      if( inp. back( ) == '$' ) 
      {
         readchar( idee, line, inp );  
         if( inp. back( ) == '$' ) 
         {
            readchar( idee, line, inp );
            while( isdigit( inp. back( )) || isalpha( inp. back( )) || 
                   inp. back( ) == '_' )
            {
               readchar( idee, line, inp );
            }

            if( inp. size( ) < 4 || inp. back( ) != '$' ) 
               throw ideeproblem( "ill-formed $-insertion", line );

            readchar( idee, line, inp );
            if( inp. back( ) != '$' )
               throw ideeproblem( "second $ expected", line );
         
            // When inp has been recognized and processed, one
            // should jump to handled. 
     
            if( inp == "$$h$$" )
            {
               dest = &hh;
               goto handled; 
            } 

            if( inp == "$$cpp$$" )
            {
               dest = &cpp;
               goto handled;
            }
 
            if( inp == "$$nowhere$$" )
            {
               dest = nullptr;
               goto handled; 
            }
    
            if( inp == "$$parserincludeguard$$" )
            {
               if( dest )
                  *dest << space. includeguard( "parser" ); 
               goto handled; 
            } 

            if( inp == "$$parserspace$$" ) 
            {
               if( dest )
                  space. printasprefix( *dest ); 
               goto handled;
            }

            if( inp == "$$openparserspace$$" )
            {
               if( dest )
                  space. open( *dest );
               goto handled;
            }

            if( inp == "$$closeparserspace$$" )
            {
               if( dest )
                  space. close( *dest );
               goto handled;
            }

            if( inp == "$$parsercode_h$$" )
            {
               if( dest )
               {
                  code_h. printascode( *dest ); 
                  setideeline( *dest, line );
               }
               goto handled; 
            }

            if( inp == "$$parsercode_cpp$$" )
            {
               if( !code_cpp. isempty( ))
               {
                  if( dest )
                  {
                     space. open( *dest ); 
                     code_cpp. printascode( *dest );
                     space. close( *dest );
                     *dest << "\n";
                     setideeline( *dest, line ); 
                  }
               }
               goto handled; 
            }

            if( inp == "$$parameterfields$$" )
            {
               if( dest )
                  printparameterfields( *dest ); 
               goto handled; 
            }

            if( inp == "$$constructor$$" )
            {
               if( dest )
                  printconstructor( *dest );
               goto handled; 
            }

            if( inp == "$$symbolspace$$" )
            {
               if( dest )
                  bld_sym. space. printasprefix( *dest );
               goto handled;
            }

            if( inp == "$$source$$" )
            {
               if( dest )
                  source. printascode( *dest );
               goto handled;
            }

            if( inp == "$$actiondeclarations$$" )
            {
               if( dest )
                  printactiondeclarations( bld_sym, *dest );
               goto handled;
            }

            if( inp == "$$actiondefinitions$$" )
            {
               if( dest )
                  printactiondefinitions( bld_sym, *dest );
               goto handled;
            }

            if( inp == "$$symbolhasinfo$$" )
            {
               // print true if symbol has an infotype:

               if( dest )
               {
                  if( bld_sym. infotype. isvoid( ))
                     *dest << "false";
                  else
                     *dest << "true";
               }
               goto handled;
            }

            if( inp == "$$usererror$$" )
            {
               // print true if user defines error:

               if( dest )
               {
                  if( create_errorfunc )
                     *dest << "false";
                  else
                     *dest << "true";
               } 
               goto handled;
            }

            if( inp == "$$maintables$$" )
            {
               if( dest )
                  printmaintables( pt, bld_sym. space, conflicts, *dest );
               goto handled;
            }

            if( inp == "$$errormessages$$" )
            {
               if( dest )
                  printerrortable( bld_sym. space, *dest ); 
               goto handled; 
            }

            if( inp == "$$reductioncases$$" )
            {
               if( dest )
                  printreductioncases( bld_sym, *dest );
               goto handled;
            }
 
            if( inp == "$$end$$" )
               return;

            throw ideeproblem(  
                      std::string( "unknown insertion: " ) + inp, line ); 
         }
      }

      for( auto c : inp )
      {
         if( c == '\n' )
            ++ line; 

         if( dest )
            dest -> put(c);
      } 

   handled:
      ;
   }

}


std::unordered_map< std::string, std::string >
parsercodebuilder::createreplacement( size_t nr, 
                         const symbolcodebuilder& bld_sym ) const
{
   std::unordered_map< std::string, std::string > res;
   for( const auto& par : reductions[nr]. parameters )
   {
      std::ostringstream expr; 
         // Expression by which par. left will be replaced.

      expr << "(topsymbol(";
      expr << grrr. rules[ nr ]. rhs. size( ) - par. second; 
      expr << ").get<";
      symbol s = grrr. rules[nr]. rhs[ par. second ];
      bld_sym. attributes. at(s). printascode( expr, cpp::type::value ); 
      expr << ">())"; 

      res. insert( std::pair( par. first, expr. str( )) );
   }

   // If there is an infotype, we make it accessible as
   // a__info: 

   if( !bld_sym. infotype. isvoid( )) 
   {
      for( const auto& par : reductions[nr]. parameters )
      {
         std::ostringstream expr;
            // Expression by which par. left will be replaced.

         expr << "(topsymbol(";
         expr << grrr. rules[ nr ]. rhs. size( ) - par. second;
         expr << ").info)";
         res. insert( std::pair( par. first + "__info", expr. str( )) );
      }
   }
   return res; 
}


void parsercodebuilder::printparameterfields( std::ostream& out ) const
{
   for( const auto& par : parameters )
   {
      out << "   ";  
      par. second. printascode( out, cpp::type::ref );
      out << " ";
      par. first. print( out );
      out << ";\n";
   }
}

void parsercodebuilder::printconstructor( std::ostream& out ) const
{
   out << "   parser("; 
   bool first = true; 
   for( const auto& par : parameters ) 
   {
      if( first ) 
         out << " "; 
      else
         out << ",\n           ";

      par. second. printascode( out, cpp::type::ref );
      out << " ";
      par. first. print( out );
      first = false; 
   }
   out << " )";

   first = true;
   for( const auto& par : parameters )
   {
      if( first )
         out << "\n      : ";
      else
         out << ",\n        ";
      par. first. print( out ); 
      out << "( "; 
      par. first. print( out );
      out << " )";
      first = false; 
   }
  
   out << "\n   { }\n";
}

void parsercodebuilder::printactiondeclarations( 
         const symbolcodebuilder& bld_sym, std::ostream& out ) const
{
   if( grrr. nrrules( ) != reductions. size( ))
      throw std::logic_error( "sizes must be equal" );

   // We look for the first rule that has a condition or reduction code.

   size_t start = 0;
   while( start != grrr. nrrules( ) && reductions[ start ]. isempty( ) )
      ++ start;

   if( start == grrr. nrrules( ))
      return; 

   out << "   // Declarations of reduction functions and their conditions:\n";
   out << "   // They have no arguments because they use the stack.\n\n";

   for( size_t i = start; i != grrr. nrrules( ); ++ i )
   {
      if( !reductions[i]. cond. isempty( ))
      {
         out << "   short cond" << i << "( ) const;\n";
      }

      if( !reductions[i]. body. isempty( ))
      {
         cpp::type tp = bld_sym. attributes. at( grrr. rules[i]. lhs ); 
         out << "   ";
         tp. printascode( out, cpp::type::value );
         out << " body" << i << "( );\n";
      }
   }
}

void parsercodebuilder::printactiondefinitions(
         const symbolcodebuilder& bld_sym, std::ostream& out ) const
{
   if( grrr. nrrules( ) != reductions. size( ))
      throw std::logic_error( "sizes of rules and reductions must be equal" );

   // We look for the first rule that has a condition or reduction code.

   size_t start = 0;
   while( start != grrr. nrrules( ) && reductions[ start ]. isempty( ) )
      ++ start;

   if( start == grrr. nrrules( ))
      return;

   out << "\n";
   out << "// Definitions of the reduction functions:\n";
   out << "\n"; 

   for( size_t i = start; i != grrr. nrrules( ); ++ i )
   {
      if( !reductions[i]. isempty( ))
      {
         auto repl = createreplacement( i, bld_sym ); 
         if( !reductions[i]. cond. isempty( ))
         {
            out << "short ";
            space. printasprefix( out ); 
            out << "parser::cond" << i << "( ) const\n";
            out << "{\n";
            reductions[i]. cond. printascode( out, repl ); 
            out << "}\n\n";
         }

         if( !reductions[i]. body. isempty( ))
         {
            out << "auto ";
            space. printasprefix( out );
            out << "parser::body" << i << "( ) -> ";
            cpp::type tp = bld_sym. attributes. at( grrr. rules[i]. lhs );
            tp. printascode( out, cpp::type::value );
            out << "\n";
            out << "{\n";
            reductions[i]. body. printascode( out, repl ); 
            out << "}\n\n";
         }
      }
   }
}


void parsercodebuilder::rulesequence::print( std::ostream& out ) const
{
   out << "rulesequence(";
   for( auto p = seq. begin( ); p != seq. end( ); ++ p )
   {
      if( p != seq. begin( ))
         out << ',';
      out << ( (*p) -> nr ); 
   }
   out << ')';
}

bool parsercodebuilder::rulesequence::lhs_containedin( 
           const symbol::map< size_t > & set ) const 
{
   for( const rule* r : seq )
      if( set. find( r -> lhs ) == set. end( ))
         return false;
   return true; 
}

void parsercodebuilder::rulesequence::sort_with( 
           const symbol::map< size_t > & pref ) 
{
   std::sort( seq. begin( ), seq. end( ),
      [&pref]( const rule* r1, const rule* r2 ) -> bool 
      { return pref. at( r1 -> lhs ) < pref. at( r2 -> lhs ); } );

}

void 
parsercodebuilder::printmaintables( const parsetable& pt, 
                                    const cpp::nspace& symbolspace, 
                                    std::vector< conflict > & conflicts, 
                                    std::ostream& out ) const
{
   tablecodebuilder bld_code; 

   // Collect the start symbols:

   for( const auto& start : grrr. startsymbols )
   {
      symbol sym = start. first; 
      size_t state = pt. initialstates. at( sym ); 

      bld_code. startsymbols. insert( { sym, { state, start. second }} );
   }

   for( size_t ss = 0; ss != pt. nrstates( ); ++ ss )
   {
      const parsetable::state& st = pt. states[ss];

      symbolset shiftred; 
         // Symbols for which there are both a shift and a reduction:

      for( const auto& p : st. reductions )
      {
         if( st. shifts. count( p. first ))
            shiftred. insert( p. first );
      }

      if( st. reductions. size( ))
      {
         if( usedefaults && shiftred. size( ) == 0 && 
             st. itemset. nrreductions( ) == 1 )
         {
            const auto& s = st. reductions. begin( ) -> second; 
            size_t nr = (*s.begin( )) -> nr; 

            // If we have no shifts at all, then we are trivial (T): 

            if( st. shifts. size( ) == 0 )
               bld_code. reductioninfo. push_back( { 'T', nr } );  
            else
               bld_code. reductioninfo. push_back( { 'D', nr } ); 
         }
         else
         {
            bld_code. reductioninfo. push_back( { 'H', 0 } );
               // It will be a hard state. 

            conflicts. push_back( conflict(ss) );
               // If it turns out that there are no conflicts,
               // we will pop_back( ) in the end.

            for( const auto& red : st. reductions )
            {
               rulesequence redcandidates; 
               for( const auto& p : red. second )
                  redcandidates. append(p);

               bool doinggreat = true;
               if( redcandidates. size( ) > 1 )
               {
                  // redcandidates must be contained in one of the
                  // reduction sequences.

                  auto p = redcandidates. first_lhs_containedin( 
                              redsequences. begin( ),
                              redsequences. end( )); 
                  if( p == redsequences. end( ))
                  {
                     auto sym = red. first;  

                     for( auto r : redcandidates )
                        conflicts. back( ). 
                           badrules[ sym ]. push_back( r -> nr );
                  }
                  else
                  {
                     redcandidates. sort_with( *p ); 

                     // If two rules have identical lhs, we have a conflict:

                     auto c1 = redcandidates. begin( );
                     while( c1 != redcandidates. end( ))
                     {
                        auto c2 = c1; ++ c2;
                        bool first = true; 
 
                        while( c2 != redcandidates. end( ) &&
                               ((*c1) -> lhs ) == ((*c2) -> lhs ) )
                        {
                           if( first )
                           {
                              conflicts. back( ). badrules[ red. first ].
                                 push_back( (*c1) -> nr );
                           }

                           conflicts. back( ). badrules[ red. first ]. 
                              push_back( (*c2) -> nr );

                           ++ c2; first = false; 
                        }
                        
                        c1 = c2; 
                     }
                  }
               }

               for( auto p = redcandidates. begin( );
                         p != redcandidates. end( );
                         ++ p ) 
               {
                  if( reductions [ (*p) -> nr ]. cond. isempty( ))
                  {
                     auto p1 = p; ++ p1;
                     if( p1 == redcandidates. end( ))
                     {
                        // If we are last, and there is a shift for 
                        // red. first, we report a conflict:

                        if( st. hasshift( red. first ))
                        {
                           conflicts. back( ). badrules[ red. first ].
                              push_back( (*p) -> nr );
                        }
                     }
                     else
                     {
                        conflicts. back( ). badrules[ red. first ].
                           push_back( (*p) -> nr ); 
                     }
                  }
               }

               // We need the numbers of the rules:

               std::vector<size_t> numbers;
               for( auto c : redcandidates )
                  numbers. push_back( c -> nr ); 

               bld_code. reductions. insert( { { ss, red. first }, 
                                               std::move( numbers ) } );
            }
   
            if( conflicts. back( ). isempty( )) 
               conflicts. pop_back( ); 

         }
      }
      else
      {
         bld_code. reductioninfo. push_back( { 'N', 0 } );  
            // State has no reductions.
      }

      // We almost forgot about the shifts. Here they are:

      for( const auto& shift : st. shifts )
         bld_code. shifts. insert( {{ ss, shift. first }, shift. second } ); 
   }

   bld_code. printascode( out, space, symbolspace );    
}


void 
parsercodebuilder::printerrortable( const cpp::nspace& symbolspace,
                                    std::ostream& out ) const
{
   out << "const std::vector< ";
   space. printasprefix( out ); 
   out << "parser::expectation > ";
   space. printasprefix( out );
   out << "parser::errormessages =\n";

   out << "{\n";

   for( auto e = errormessages. begin( ); e != errormessages. end( ); ++ e )
   {
      if( e != errormessages. begin( ))
         out << ",\n";

      out << "   { ";
     
      out << "{ ";  
      for( auto t = e -> trigger. begin( ); t != e -> trigger. end( ); ++ t )
      {
         if( t != e -> trigger. begin( ))
            out << ", ";    

         out << "{{ " ;
         for( auto s = t -> set. begin( ); s != t -> set. end( ); ++ s )
         {
            if( s != t -> set. begin( ))
               out << ", ";

            symbolspace. printasprefix( out );
            out << "sym_" << *s; 
         }
         out << " }, " << t -> maxskip << " }"; 
       }
       out << " }, "; 
       out << e -> expected; 
       out << " }";
   }
   out << "\n};\n";
}


void parsercodebuilder::printreductioncases( 
         const symbolcodebuilder& bld_sym, 
         std::ostream& out ) const
{
   if( grrr. nrrules( ) != reductions. size( ))
      throw std::logic_error( "sizes must be equal" );

   for( size_t nr = 0; nr != reductions. size( ); ++ nr )
   {
      indentation ind(6);
      out << ind << "case " << nr << ":\n";
      ind += 3;
      if( !reductions[nr]. cond. isempty( ))
      {
         out << ind << "if( short s = cond" << nr << "(); s > 0 )\n"; 
         out << ind << "{\n";
         ind += 3;
      }

      if( !bld_sym. attributes. at( grrr. rules[nr]. lhs ). isvoid( ))
      {
         // This should have been checked before, so if it happens
         // here, it is a logic error.

         if( reductions[nr]. body. isempty( ))
         {
            std::cout << "rule is " << nr << "\n";
            throw std::logic_error( "no attribute function defined" ); 
         }
 
         out << ind << "reduce( ";
         bld_sym. space. printasprefix( out );  
         out << "sym_" << grrr.rules[nr]. lhs << ", ";
         out << "body" << nr << "( ), "; 
         out << grrr.rules[nr]. rhs.size( ) << " );\n";
      }
      else
      {
         if( !reductions[nr]. body. isempty( ))
         {
            out << ind << "body" << nr << "( );\n";
         }
         out << ind << "reduce( ";
         bld_sym. space. printasprefix( out ); 
         out << "sym_" << grrr.rules[nr].lhs << ", "; 
         out << "std::monostate( ), ";
         out << grrr.rules[nr]. rhs. size( ) << " );\n";
      }
      out << ind << "goto mainloop;\n";

      if( !reductions[nr]. cond. isempty( ))
      {
         ind -= 3;
         out << ind << "}\n";
         out << ind << "else\n";
         out << (ind+3) << "if( s < 0 ) shift = reductioninfo. size( );\n"; 
      }
      out << ind << "break;\n";
   }

}



std::ostream& 
operator << ( std::ostream& out, const parsercodebuilder& bld )
{
   out << "parsercodebuilder:\n\n";

   out << "namespace =      ";
   bld. space. print( out );
   out << "\n";

   out << "source =          ";
   bld. source. print( out ); 

   out << "parameters:\n";
   for( const auto& g : bld. parameters )
   {
      out << "   " << g. first << " :       " << g. second << "\n";
   } 
   out << "\n"; 

   if( !bld. code_h. isempty( ))
      out << "code for .h:\n" << bld. code_h << "\n";

   if( !bld. code_cpp. isempty( ))
      out << "code for .cpp:\n" << bld. code_cpp << "\n";
 
   out << bld. grrr << "\n";

   if( bld. redsequences. size( ))
   {
      out << "Reduction Sequences:\n";
      for( const auto& seq: bld. redsequences )
      {
         out << "   ";
         for( auto p = seq. begin( ); p != seq. end( ); ++ p ) 
         {
            if( p != seq. begin( ))
               out << ", ";
            out << ( p -> first ) << "/" << ( p -> second );
         }
         out << "\n";
      } 
      out << "\n";
   }

   for( size_t i = 0; i != bld. reductions. size( ); ++ i )
   {
      if( !bld. reductions[i]. isempty( ))
      {
         out << "#" << i << ": ";
         bld. reductions[i]. print( out );
      }
   }

   if( bld. errormessages. size( ))
   {
      out << "\n";
      out << "Possible error messages ( trigger => expectation ):\n";
      for( const auto& e : bld. errormessages )
         out << "   " << e << "\n";
      out << "\n";
   }

   return out;
}


