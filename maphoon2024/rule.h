
// Written by Hans de Nivelle, October 2005. 
// A rule is a thing that looks like
// A -> B1 ... Bn, in which A, B1, ..., Bn symbols and n >= 0. 
// 
// In June 2008, I removed the priority and associativity from rules,
// because I think that priorities are a thing between symbols.
//
// Rewritten in February 2021. 

#ifndef MAPH_RULE_
#define MAPH_RULE_  1

#include <vector>
#include <iostream>

#include "symbol.h"

struct rule 
{
   size_t nr;

   symbol lhs;
   std::vector< symbol > rhs;

   rule( size_t nr, symbol lhs, const std::vector< symbol > & rhs )
      : nr( nr ), lhs( lhs ), rhs( rhs )
   { }

   rule( size_t nr, symbol lhs, std::vector< symbol > && rhs )
      : nr( nr ), lhs( lhs ), rhs( std::move( rhs ))
   { }

   using rhs_iterator = std::vector< symbol > :: const_iterator;
      // The dot in an item. 
 
   rhs_iterator rhs_begin( ) const { return rhs. begin( ); }
   rhs_iterator rhs_end( ) const { return rhs. end( ); }
};

std::ostream& operator << ( std::ostream& , const rule& );

#endif


