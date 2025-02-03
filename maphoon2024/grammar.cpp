
// Written by Hans de Nivelle, Jan 2021.

#include "grammar.h"

void grammar::push_back( rule&& r )
{
   index[ r. lhs ]. insert( rules. size( )); 

   size_t nr = rules. size( ); 
   rules. push_back( std::move(r) ); 
}


void grammar::addstartsym( symbol start, const symbolset& lookahead )
{
   startsymbols[ start ]. insert( lookahead );
}


symbolset grammar::leftsymbols( ) const
{
   symbolset result;
   for( const auto& r : rules ) 
      result. insert( r. lhs );
   return result;
}


symbolset grammar::rightsymbols( ) const
{
   symbolset result; 
   for( const auto& r : rules ) 
   {
      for( auto p = r. rhs_begin( ); p != r. rhs_end( ); ++ p ) 
         result. insert( *p );
   }
   return result;
}


symbolset grammar::allsymbols( ) const
{
   symbolset result;

   // Collect the start symbols and their lookaheads:

   for( const auto& p : startsymbols )
   {
      result. insert( p. first );
      for( auto la : p. second )
         result. insert( la ); 
   }

   result. insert( leftsymbols( ));
   result. insert( rightsymbols( ));
   return result;
}


namespace
{
   // Used in nullablesymbols( ) below:

   void 
   print( const symbol::map< std::vector< std::pair< size_t, size_t >>> & mp,
          std::ostream& out )
   {
      for( const auto& p : mp )
      {
         out << p. first << " : ";
         out << "{ ";
         for( size_t i = 0; i != p. second. size( ); ++ i )
         {
            if(i) out << ", "; 
            out << p. second[i]. first << "/" << p. second[i]. second; 
         }
         out << " }\n";
      }
   }

   // We check that nullable is a fixed point, but we don't
   // check that it is a least fixed point. 

   void
   recheck( const grammar& gr, const symbolset& nullable )
   {
      symbolset next;

      for( size_t nr = 0; nr != gr. nrrules( ); ++ nr )
      {
         bool subs = true;
         for( size_t i = 0; i != gr. rules[nr]. rhs. size( ); ++ i )
         {
            if( !nullable. contains( gr. rules[nr]. rhs[i] ))
               subs = false;
         }

         if( subs )
            next. insert( gr.rules[nr]. lhs ); 
      }
 
      for( symbol s : nullable )
      {
         if( !next. remove(s))
         {
            std::cout << "symbol " << s << " should not have been there\n";
            std::abort( ); 
         }
      }
      if( next. size( ))
      {
         std::cout << next << "  were missed\n";
         std::abort( );
      }
   }

}

// I believe that this algorithm is optimal. It is essentially
// Horn clause logic.

symbolset grammar::nullablesymbols( ) const
{
   symbolset result;   
   std::vector< symbol > unchecked;

   symbol::map< std::vector< std::pair< size_t, size_t >>> watched;
      // watched[ sym ] is a list of pairs of form nr/i, such that 
      // rules[nr].rhs[i] equals sym.
  
   for( size_t nr = 0; nr != rules. size( ); ++ nr )
   {
      if( rules[nr]. rhs. size( ) == 0 )
      {
         if( result. insert( rules[nr]. lhs ))
            unchecked. push_back( rules[nr]. lhs );
      }
      else
         watched[ rules[nr]. rhs[0] ]. push_back( std::pair( nr, 0 )); 
   }

   // No rule watches a symbol in result \ unchecked.

   while( unchecked. size( )) 
   {
      symbol s = unchecked. back( );
      unchecked. pop_back( );

      auto w = watched. find(s); 
      if( w != watched. end( )) 
      {
         for( const auto& p : w -> second ) 
         {
            std::pair< size_t, size_t > next = p;
            while( rules[ next. first ]. rhs. size( ) != next. second &&
                result. contains( rules[ next. first ]. rhs[ next. second ] ))
            {
               ++ next. second; 
            }

            // If we can find no other symbol to watch in the rhs,
            // then the lhs is nullable.

            if( rules[ next. first ]. rhs. size( ) == next. second )
            {
               symbol found = rules[ next. first ]. lhs; 
               if( result. insert( found )) 
                  unchecked. push_back( found );
            }
            else
            {
               // We found another symbol to watch, it is n:

               symbol n = rules[ next. first ]. rhs[ next. second ];
               watched[n]. push_back( next ); 
            }
         }
         w -> second. clear( );
      }
   }

   recheck( *this, result ); 
   return result;
}


symbolset grammar::reachablesymbols( symbol s ) const
{
   // Invariant :
   // Every symbol in unchecked is also in res.
   // For the symbols in res \ unchecked, the immediately reachable symbols 
   // are in res.

   std::vector< symbol > unchecked;
   unchecked. push_back(s);
 
   symbolset res;
   res. insert(s);

   while( unchecked. size( ))
   {
      symbol lhs = unchecked. back( );
      unchecked. pop_back( );

      auto r = index. find( lhs );
      if( r != index. end( ))
      {
         // Indices of the rules that have lhs as left hand side:

         for( size_t i : r -> second ) 
         {
            for( auto p = rules[i]. rhs_begin( ); 
                      p != rules[i]. rhs_end( ); ++ p )  
            {
               if( res. insert( *p ))
                  unchecked. push_back(*p);  
            }
         }
      }
   }

   return res;
}


bool grammar::hasrule( symbol s ) const
{
   auto p = index. find(s);
   return p != index. end( ) && p -> second. size( ); 
}


symbol::map< symbolset > 
grammar::firstsets( const symbolset& allsymbols, 
                    const symbolset& nullable ) const
{
   symbol::map< symbolset > firsts; 
   std::vector< std::pair< symbol, symbol >> unchecked;
      // Pair { A, B } means that we discovered that B 
      // is among the firsts of A. 
 
   for( auto s : allsymbols )
   {
      firsts[s] = { s };
      unchecked. push_back( std::pair( s, s ));
   }

   while( unchecked. size( ))
   {
      auto f = unchecked. back( );
      unchecked. pop_back( );

      auto r = index. find( f. second );
      if( r != index. end( ))
      {
         // Indices of the rules that have f. second as right hand side.

         for( size_t i : r -> second )
         {
            // We always include the first symbol of the rhs, but
            // if it is nullable also the second, etc:
     
            bool nullablebefore = true; 
            for( auto p = rules[i]. rhs_begin( ); 
                      nullablebefore && p != rules[i]. rhs_end( ); ++ p )
            {
               if( firsts. at( f. first ). insert(*p) )
               {
                  unchecked. push_back( std::pair( f. first, *p ));
               }

               if( !nullable. contains( *p ))
                  nullablebefore = false;   
            }
         } 
      }
   }
   return firsts;
}


grammar grammar::example1( )
{
   grammar g;

   g. addstartsym( symbol( "S" ), { symbol( "EOF" ) } );

   g. push_back( rule( g. nrrules( ), 
                   symbol( "S" ), { symbol( "E" ), symbol( "dot" ) } ));
   g. push_back( rule( g. nrrules( ), 
                   symbol( "E" ), { symbol( "op" ), symbol( "E" ) } )); 
   g. push_back( rule( g. nrrules( ),
                   symbol( "E" ), { symbol( "E" ), symbol( "op" ), symbol( "E" ) } )); 

   g. push_back( rule( g. nrrules( ),
                   symbol( "E" ), { symbol( "E" ), symbol( "op" ) } )); 

   g. push_back( rule( g. nrrules( ),
                symbol( "E" ), { symbol( "lpar" ), symbol( "E" ), symbol( "rpar" ) } ));

   g. push_back( rule( g. nrrules( ), symbol( "E" ), { symbol( "id" ) } )); 
   g. push_back( rule( g. nrrules( ), symbol( "E" ), { symbol( "num" ) } )); 
   g. push_back( rule( g. nrrules( ), symbol( "op" ), { symbol( "id" ) } )); 

   return g;
}


grammar grammar::example3( )
{
   symbol sss = symbol( "S" );

   symbol aaa = symbol( "A" );
   symbol bbb = symbol( "B" );
   symbol ccc = symbol( "C" );
   symbol ddd = symbol( "D" );

   symbol aa = symbol( "a" );
   symbol bb = symbol( "b" );
   symbol cc = symbol( "c" );
   symbol dd = symbol( "d" );

   grammar g;
   g. addstartsym( sss, { symbol( "EOF" ) } );

   g. push_back( rule( g. nrrules( ), sss, { aaa, bbb, ccc } ));
   g. push_back( rule( g. nrrules( ), aaa, { } ));
   g. push_back( rule( g. nrrules( ), aaa, { cc } ));
   g. push_back( rule( g. nrrules( ), aaa, { } )); 
   g. push_back( rule( g. nrrules( ), bbb, { cc } ));
   g. push_back( rule( g. nrrules( ), bbb, { dd } ));
   g. push_back( rule( g. nrrules( ), bbb, { bbb } )); 

   g. push_back( rule( g. nrrules( ), ccc, { dd } )); 

   return g;
}


std::ostream& operator << ( std::ostream& stream, const grammar& g )
{
   stream << "Start Symbols with their Lookahead Sets:\n";
   stream << g. startsymbols << "\n\n";

   stream << "Rules:\n\n"; 
   for( auto& r : g. rules ) 
      stream << r << "\n";
   stream << "\n\n";

   return stream; 
}


