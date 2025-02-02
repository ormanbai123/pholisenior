
// Code written by Hans de Nivelle, January 2021.

#include "symbol.h"

symbol symbol::recover( ) 
{
   static symbol s = symbol( "_recover_" );
   return s;
}

void symbol::printtable( std::ostream& stream )
{
   stream << "Symbol Table = {";

   const auto& ind = getindex( ); 

   for( auto p = ind. begin( ); p != ind. end( ); ++ p )
   {
      if( p != ind. begin( ))
         stream << ", ";
      else
         stream << " ";
      stream << *p;  
   }
   stream << " }\n";
}


