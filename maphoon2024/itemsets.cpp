
// Code written by Hans de Nivelle, January 2021. 

#include "itemsets.h"

std::ostream& operator << ( std::ostream& out, item itm )
{
   out << (( itm. r ) -> lhs ) << " => ";
   for( rule::rhs_iterator 
           p = itm. r -> rhs_begin( ); p != itm. r -> rhs_end( ); ++ p )
   {
      if( p == itm. p )
         out << " # ";
      else
         out << " ";

      out << *p; 
   }

   if( itm. p == itm. r -> rhs. end( ))
      out << " #";

   return out;
}

bool lr0itemset::insert( const lr0itemset& set ) 
{
   bool b = false; 
   for( const auto& p : set. repr )
      b |= insert( p. first );
   return b;
}

size_t lr0itemset::hash::operator( ) ( const lr0itemset& set ) const
{
   item::hash itemhash;

   // It is essential that we use only commutative operations:
   // This probably can be solved by changing the container.

   size_t hv = set. size( ) * 3019;
   for( const auto& p : set ) 
   {
      size_t h = itemhash( p. first ); 
      hv += h;
   }

   return hv; 
}

bool lr0itemset::equal_to::subset( const lr0itemset& set1, 
                                   const lr0itemset& set2 ) const
{
   if( set1. size( ) <= set2. size( ))
   {
      for( const auto& itm : set1 ) 
      {
         if( set2. repr. find( itm. first ) == set2. end( ))
            return false;
      }
      return true; 
   }
   else
      return false; 
}

bool 
lr0itemset::equal_to::operator( ) ( const lr0itemset& set1, 
                                    const lr0itemset& set2 ) const
{
   return set1. size( ) == set2. size( ) &&
          subset( set1, set2 ) && subset( set2, set1 );
}

std::ostream& operator << ( std::ostream& out, const lr0itemset& itset )
{
   for( const auto& itm : itset. repr )
      out << "   " << itm. first << "\n";
   out << "\n";
   return out;
}



bool lr1itemset::contains( item itm, symbol s ) const
{
   auto p = repr. find( itm );
   return p != repr. end( ) && p -> second. contains(s); 
}

bool lr1itemset::insert( item itm, const symbolset& lookahead ) 
{
   auto p = repr. find( itm );
   if( p == repr. end( ))
   {
      repr. insert( std::pair( itm, lookahead ));
      return true;
   }
   else
      return p -> second. insert( lookahead ); 
}

bool lr1itemset::insert( const lr1itemset& set )
{
   bool b = false; 
   for( const auto& p : set )
   {
      b |= insert( p.first, p.second );
   }
   return b;
}

symbolset lr0itemset::shiftables( ) const
{
   symbolset set; 
   for( const auto& p : repr )
   {
      if( !p. first. atend( ))
         set. insert( *p. first ); 
   }
   return set; 
}


lr1itemset 
lr1itemset::closure( const grammar& g,
                     const symbolset& nullable, 
                     const symbol::map< symbolset > & firsts ) const 
{ 
   lr1itemset result = *this;

   std::vector< std::pair< item, symbol >> unchecked;
      // unchecked means that we have tried to derive further item/lookahead
      // pairs from the item and its lookahead. 
      // If unchecked is empty, then checked is the result that 
      // can be returned.
  
   for( const auto& itm : *this )
      for( symbol s : itm. second )
      {
         unchecked. push_back( std::pair( itm. first, s ));  
            // template constructor automatic instantiation. First time. 
      }

   // Invariant is : If unchecked contains (itm, lookahead), then
   // result[itm] contains lookahead. 
   // If la in result[itm] and (itm,la) not in unchecked, then
   // its one step derivable items are in result.

   while( unchecked. size( )) 
   {
      item itm = unchecked. back( ). first;
      symbol lookahead = unchecked. back( ). second;
      unchecked. pop_back( ); 

      // std::cout << "   checking: " << itm << " / " << lookahead << "\n";
    
      if( !itm. wellformed( ))
         throw std::logic_error( "closure: item is not well-formed" ); 
       
      if( !itm.atend( ) && g.hasrule( *itm ) ) 
      {
         symbol lhs = *itm ++ ; 
   
         symbolset after; 
            // The lookahead set of the items that
            // we are going to create. 

         while( !itm. atend( ) && nullable. contains(*itm) )
         {
            after. insert( firsts. at( *itm ));  
            ++ itm;
         } 
           
         if( itm. atend( ))
            after. insert( lookahead );  
         else
            after. insert( firsts. at( *itm ));  

         const auto& s = g. index. at( lhs );
 
         // Indices of the rules that have form lhs ->  ...

         for( size_t i : s )
         {
            item next( &g.rules[i] ); 
            for( symbol a : after )
            {
               if( result. insert( next, a ))
                  unchecked. push_back( std::pair( next, a )); 
            }
         }
      }
   } 
   return result;
}

lr1itemset lr1itemset::shift( const symbol s ) const
{
   lr1itemset result;

   for( const auto& p : repr ) 
   {
      if( !p. first. atend( ) && *p.first == s ) 
      {
         item itm = p. first;
         ++ itm;  
	 result. insert( itm, p. second ); 
      }
   }
   return result;
}

lr0itemset lr1itemset::lr0base( ) const
{
   lr0itemset set;
   for( const auto& p : *this )
      set. insert( p. first );
   return set;
}

lr1itemset lr1itemset::getreductions( ) const
{
   lr1itemset result;
   for( const auto& p : *this ) 
   {
      if( p.first.atend( ))
         result. repr. insert( p ); 
   }
   return result;
}

size_t lr1itemset::nrreductions( ) const
{
   size_t nr = 0;
   for( const auto& p : *this ) 
   {
      if( p.first.atend( )) 
         ++ nr; 
   }
   return nr;
}

std::ostream& operator << ( std::ostream& out, const lr1itemset& itset ) 
{
   for( const auto& itm : itset. repr ) 
      out << "   " << itm. first << "  /  " << itm. second << "\n";
   out << "\n";
   return out;
}


