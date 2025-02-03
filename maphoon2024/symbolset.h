
// Code written by Hans de Nivelle, January 2021.

// One could wonder why this class exists.
// Life is full of mysteries.

#ifndef MAPH_SYMBOLSET_
#define MAPH_SYMBOLSET_  1

#include <unordered_set>
#include <iostream>

#include "symbol.h"

class symbolset
{
   std::unordered_set< symbol, symbol::hash, symbol::equal_to > repr; 

public: 
   symbolset( ) = default;
  
   symbolset( std::initializer_list< symbol > init )
   {   
      for( symbol s : init ) 
         insert(s);
   }
 
   bool contains( symbol s ) const 
      { return repr. find(s) != repr. end( ); }  

   // Returns true if s did not occur before the insertion:

   bool insert( symbol s ) { return repr. insert(s). second; } 

   // Returns the number of elements actually inserted. 
   // (those that did not occur already)

   size_t insert( const symbolset& set ) 
   {
      size_t count = 0; 
      for( symbol s : set )
         count += insert(s);
      return count;
   }

   // Returns true if s occurred before removal: 

   bool remove( symbol s ) { return repr. erase(s); } 

   // Returns the number of elements actually removed:

   size_t remove( const symbolset& set )
   {
      size_t count = 0;
      for( symbol s : set )
         count += remove(s);
      return count;
   }

   size_t size( ) const { return repr. size( ); } 
   void clear( ) { repr. clear( ); } 

   friend std::ostream& operator << ( std::ostream& , const symbolset& s );

   using iterator = 
   std::unordered_set< symbol, symbol::hash, symbol::equal_to > :: iterator;

   using const_iterator = 
   std::unordered_set< symbol, symbol::hash, 
                       symbol::equal_to > :: const_iterator;

   iterator begin( ) { return repr. begin( ); } 
   iterator end( ) { return repr. end( ); }

   const_iterator begin( ) const { return repr. cbegin( ); }
   const_iterator end( ) const { return repr. cend( ); }

   const_iterator cbegin( ) const { return repr. cbegin( ); }
   const_iterator cend( ) const { return repr. cend( ); }

};

inline std::ostream& operator << ( std::ostream& out, const symbolset& s )
{
   out << "{";
   for( auto p = s. begin( ); p != s. end( ); ++ p )
   {
      if( p != s. begin( ))
         out << ", ";
      else
         out << " ";
      out << *p;
   }
   out << " }"; 
   return out;
}

inline symbolset intersection( const symbolset& s1, const symbolset& s2 )
{
   symbolset inter;
   for( symbol s : s1 )
   {
      if( s2. contains(s) )
         inter. insert(s);
   }
   return inter;
}

#endif

