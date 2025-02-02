

// Code written by Hans de Nivelle, November 2006.
// Simplified June 2008. Rewritten January 2021.


#include "parsetable.h"

void parsetable::state::print( std::ostream& out, bool showred ) const
{
   out << itemset;

   if( shifts. size( ))
   {
      out << "Shifts:\n";
      for( const auto& p : shifts )
         out << "   " << p. first << " :  " << p. second << "\n";
      out << "\n";
   }

   if( showred && reductions. size( ))
   {
      out << "Reductions:\n";
      for( const auto& p : reductions )
      {
         out << "   " << p. first << " :  ";
         for( auto q = p. second. begin( ); q != p. second. end( ); ++ q )
         {
            if( q != p. second. begin( ))
               out << ", ";
            out << (*q) -> nr; 
         }
         out << "\n";
      }
      out << "\n";
   } 

}


parsetable::parsetable( const grammar& g )
{
   // We create for each start symbol an initial state:

   for( const auto& s : g. startsymbols ) 
   {
      lr0itemset init0;
      lr1itemset init1;

      auto p = g. index. find( s. first ); 
        // Indices of rules that have s. first as lhs.

      if( p != g. index. end( ))
      {
         for( size_t r : p -> second ) 
         {
            init0. insert( item( &g.rules[r] ));
            init1. insert( item( &g.rules[r] ), s. second ); 
         }
      }

      size_t nr = states. size( );
      index. insert( std::pair( std::move( init0 ), nr ));

      states. push_back( state( std::move( init1 ) )); 
      initialstates. insert( std::pair( s. first, nr ));
   }
}

void parsetable::close( const grammar& g, bool showclosures ) 
{
   std::cout << "starting closure\n";

   // Time to collect the symbols from the grammar:

   symbolset nullable = g. nullablesymbols( );
   symbolset allsymbols = g. allsymbols( );
   symbol::map< symbolset > firsts = g. firstsets( allsymbols, nullable );

   // std::cout << "nullable symbols : " << nullable << "\n";
   // std::cout << "first sets :\n" << firsts << "\n";

   std::unordered_set< size_t > lookaheadchanged; 
      // These are the states for which one of the lookaheads 
      // was changed. 
      // If s is in this set, we need to check for every state
      // that is reachable from s, if its lookahead also changes.

   size_t generate = 0; 
      // In the first pass we compute the reachable lr0items. 
      // In the second and later passes we keep adjusting lookaheads
      // until we reach a fixed point.

   while( generate < states. size( ))
   {
      lr1itemset clos1 = 
         states[ generate ]. itemset. closure( g, nullable, firsts );

      lr0itemset clos0 = clos1. lr0base( );

      symbolset possibleshifts = clos0. shiftables( ); 

      for( symbol s : possibleshifts )
      {
         lr1itemset aftershift = clos1. shift(s);
         auto it = index. insert( 
            std::pair( aftershift. lr0base( ), states. size( ))); 

         // Create the shift in the parse table:

         states[ generate ]. shifts. insert( 
                      std::pair( s, it.first -> second ));

         if( !it. second )
         {
            // State already exists.

            size_t nr = it. first -> second;  
            bool changed = states[nr]. itemset. insert( aftershift );  
            if( changed ) 
               lookaheadchanged. insert( nr ); 
         }
         else
         {
            // State is new: 

            states. emplace_back( std::move( aftershift ));     
         }
      }
      ++ generate;
   }

   std::cout << "all states have been generated\n";

   while( lookaheadchanged. size( ))
   {
      auto p = lookaheadchanged. begin( );
      size_t s = *p;
      lookaheadchanged. erase(p); 

      // std::cout << "rechecking neighbours of state " << s << "\n";

      lr1itemset clos = states[s]. itemset. closure( g, nullable, firsts );
      // std::cout << clos << "\n";

      for( const auto& p : states[s]. shifts )
      {
         lr1itemset next = clos. shift( p. first ); 
         size_t oldsize = states[ p. second ]. itemset. size( ); 
         // std::cout << states[ p. second ]. itemset << "\n";
         // std::cout << next << "\n";
         bool b = states[ p. second ]. itemset. insert( next );
         if(b) 
         {
            // std::cout << "we found a change\n";
            if( oldsize != states[ p. second ]. itemset. size( ))
               throw std::runtime_error( 
                  "new itemsets were added: this should never happen" ); 
            lookaheadchanged. insert( p. second );  
         }
      }
   }


   // We have to add reductions with empty rhs, because they can be 
   // introduced only by closure, and all our states were created 
   // by shifts.

   for( auto& s : states ) 
   {
      lr1itemset red = 
         s. itemset. closure( g, nullable, firsts ). getreductions( );
      s. itemset. insert( red );
         // We just add them all. 
   }

   std::cout << "adding reductions\n";

   // We write the reductions in the reductions mappings:

   for( auto& s : states ) 
   {
      for( auto& it : s. itemset )
      {
         if( it. first. atend( ))
         {
            for( symbol sym : it. second )
               s. reductions[ sym ]. insert( it.first.r );
         }
      }
   } 

   // This plays no role for the parser. It is sometimes
   // useful to see the closures.

   if( showclosures )
   {
      std::cout << "replacing states by theire closures\n";
      for( auto& s : states )
         s. itemset = s. itemset. closure( g, nullable, firsts ); 
      std::cout << "replaced\n";
   }

   // There is one remaining problem: For the correct functioning of the
   // parser, it is necessary that from the initial states of the parser,
   // a shift under the start symbol is possible. For each start
   // symbol, we check if such a transition exists. If not, then we 
   // create one. 

   for( const auto& start : initialstates ) 
   {
      if( states[ start. second ]. shifts. find( start. first ) ==
          states[ start. second ]. shifts. end( )) 
      {
         // std::cout << "unfortunately something needs to be done\n";
         // std:: cout << start. first << "\n";

         states[ start. second ]. shifts. insert( 
                   std::pair( start. first, states. size( )));

         states. push_back( state( lr1itemset( ) )); 
            // The state stays empty. 
      }
   }
   std::cout << "ending closure\n";
}


void parsetable::selfcheck( ) const
{
   std::cout << "starting self check\n";

   lr0itemset::equal_to eq;
   lr0itemset::hash h;

   for( auto p1 = index. begin( ); p1 != index. end( ); ++ p1 )
   {
      for( auto p2 = index. begin( ); p2 != p1; ++ p2 )
      {
         if( p1 -> first. size( ) == p2 -> first. size( ))
         {
            if( p1 -> first. nr_buckets( ) != p2 -> first. nr_buckets( )) 
               throw std::runtime_error( "this is a problem for hashing" );
         }

         if( eq( p1 -> first, p2 -> first ))
         {
            std::cout << p1 -> first << "\n\n";
            throw std::runtime_error( "hashing failed" ); 
         }

         if( h( p1 -> first ) == h( p2 -> first ))
         {
            std::cout << p1 -> first << "\n";
            std::cout << p2 -> first << "\n";
            std::cout << "these states have the same hash value " << 
                         h( p1 -> first ) << "\n";
         }
      }
   }
   for( size_t i = 0; i != index. bucket_count( ); ++ i )
   {
      std::cout << i << " -> " << index. bucket_size(i) << "\n";
   }
   std::cout << "ended self check\n\n";
}


void parsetable::print( std::ostream& out ) const 
{
   out << "Parsetable:\n";
   out << "====================================\n\n"; 

   out << "Initial States:\n"; 
   for( const auto& init : initialstates ) 
      out << "   " << init. first << " : state " << init. second << "\n";

   out << "\n\n";

   for( size_t ss = 0; ss != states. size( ); ++ ss )
   {
      out << "state " << ss << ":" << "\n";
      states[ss]. print( out, true );
      out << "\n";
   }
   out << "\n";
   
   if( false )
   {
      out << "lr0index:\n";
      for( const auto& p : index )
      {
         out << "state " << p.second << ":\n";
         out << p. first;
      }
   }
}


