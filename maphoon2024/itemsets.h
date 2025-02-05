
// Code written by Hans de Nivelle, January 2021. 

#ifndef MAPH_ITEMSETS_
#define MAPH_ITEMSETS_   1

#include "symbolset.h"
#include "rule.h"
#include "grammar.h"

// Items do not maintain copies of their rules. 
// They only contain pointers to their rules. 
// As a consequence, it is probably not a good idea to move or delete
// a rule at a moment when there exist itemsets on this rule. 

struct item 
{
   const rule* r;
   rule::rhs_iterator p;

   item( const rule* r )
      : r(r), p( r -> rhs_begin( )) 
   { }

   bool atend( ) const { return p == r -> rhs_end( ); } 
   symbol operator * ( ) const { return *p; }
   item& operator ++ ( ) { ++p; return *this; }
   item operator ++ ( int ) { auto itm = *this; ++p; return itm; }

   struct hash
   {
      std::hash< const rule* > rh;   
         // hasher for rule pointers. 

      // Actually, this implementation is full crazy, because
      // the p-s determine the rule.

      size_t operator( ) ( item itm ) const
      {
         size_t h1 = rh( itm. r );
         size_t h2 = itm. p - itm. r -> rhs_begin( );

         return h1 + 13 * h2; 
      }
   };

   struct equal_to
   {
      bool operator( ) ( item itm1, item itm2 ) const
      {
         return itm1.r == itm2.r && itm1.p == itm2.p;  
      }
   };

   bool wellformed( ) const
   {   
      return r -> rhs_begin( ) <= p && p <= r -> rhs_end( );
   }
};

std::ostream& operator << ( std::ostream& , item itm );

template< typename D >
using itemmap = std::unordered_map< item, D, item::hash, item::equal_to > ;

// I tried, but there is no way around having classes for both types
// of itemsets. 

struct lr0itemset
{
   itemmap< bool > repr;

   lr0itemset( ) = default;

   bool contains( item itm ) const;
      // true if we contain itm.

   bool insert( item itm )
      { return repr.insert( std::pair( itm, true )). second; } 
         // Add s to the lookahead set of itm, possibly adding itm.
         // Return true if the insertion took place.

   bool insert( const lr0itemset& set ); 
      // Merge set into this one. Returns true if insertions happened.

   symbolset shiftables( ) const;
      // The set of symbols under which a shift is possible in 
      // this itemset.

   void clear( ) { repr. clear( ); }
   size_t size( ) const { return repr. size( ); }
   size_t nr_buckets( ) const { return repr. bucket_count( ); }

   using iterator = itemmap< bool > :: iterator;
   using const_iterator = itemmap< bool > :: const_iterator;

   iterator begin( ) { return repr. begin( ); }
   iterator end( ) { return repr. end( ); }
   const_iterator begin( ) const { return repr. begin( ); }
   const_iterator end( ) const { return repr. end( ); }
   const_iterator cbegin( ) const { return repr. cbegin( ); }
   const_iterator cend( ) const { return repr. cend( ); }

   struct hash
   {
      size_t operator( ) ( const lr0itemset& set ) const;
   };

   struct equal_to
   {
      bool
      subset( const lr0itemset& set1, const lr0itemset& set2 ) const;

      bool
      operator( ) ( const lr0itemset& set1, const lr0itemset& set2 ) const;
   };
};

std::ostream& operator << ( std::ostream& stream, const lr0itemset& itset );


// We implement lr1itemsets as a mapping from
// lr0 items to their lookahead sets. 
// We always assume that lookahead sets are closed under taking
// first. That means if A is in the lookahead set, then all firsts
// of A are also in the lookahead set. 

struct lr1itemset
{
   itemmap< symbolset > repr;  

   lr1itemset( ) = default;
 
   bool contains( item itm, symbol s ) const;
      // true if we contain itm with s in its lookahead set. 
   bool contains( item itm, const symbolset& set ) const;
      // true if we contain itm with all of set in its lookahead set. 

   bool insert( item itm, symbol s )
      { return repr[ itm ]. insert(s); } 
         // Add s to the lookahead set of itm, possibly adding itm. 

   bool insert( item itm, const symbolset& lookahead ); 
      // If an item with the same base exists, we merge the lookaheads,
      // otherwise we create itm. 
      // We return true if the itemset is changed, which means that
      // either the item was added, or the lookahead set was extended.

   bool insert( const lr1itemset& other );
      // Returns true if the itemset is changed. 

   lr1itemset 
   closure( const grammar& g, 
            const symbolset& nullable, 
	    const symbol::map< symbolset > & firsts ) const; 
      // Close the itemset. 
      // First define FIRST( A1, ..., An ), where Ai is a symbol or 
      // a set of symbols as FIRST(A1) cup FIRST(A2) ... FIRST(Ai),
      // where Ai is the first symbol that is not nullable. 
      // Then the closure is the smallest fixed point of the following
      // closure rule:  
      // If S contains an item of form
      //   A -> B1 ... B[i-1] _ B[i] B[i+1] ... Bk / La, and there 
      //   exists a rule B[i] -> X1 ... Xm, then S contains 
      //      B[i] -> _ X1 ... Xm / First( B[i+1], ..., La ).

   lr1itemset shift( symbol s ) const; 
      // Constructs the itemset that one obtains when s is shifted.
      // In order to obtain the complete result,
      // the itemset must be closed. 

   lr0itemset lr0base( ) const;
      // Return the items in this set as lr0 items. 
 
   lr1itemset getreductions( ) const;
      // Collects the set of reductions in this itemset. These are the
      // lr1 items that are positioned at the end of their rules. 

   size_t nrreductions( ) const;
      // Equal to reductions. size( ), but without constructing the set.

   void clear( ) { repr. clear( ); } 
   size_t size( ) const { return repr. size( ); }

   using iterator = itemmap< symbolset > :: iterator;
   using const_iterator = itemmap< symbolset > :: const_iterator;

   iterator begin( ) { return repr. begin( ); }
   iterator end( ) { return repr. end( ); }
   const_iterator begin( ) const { return repr. begin( ); }
   const_iterator end( ) const { return repr. end( ); }
   const_iterator cbegin( ) const { return repr. cbegin( ); }
   const_iterator cend( ) const { return repr. cend( ); }

};

std::ostream& operator << ( std::ostream& stream, const lr1itemset& itset );

#endif


