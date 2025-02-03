
// Written by Hans de Nivelle, March 2021.

// These are the same tables that go into the code of the parser. 
// This code eventually has to be copied to idee.x. 
// In the parser, symbol must be replaced by symbolval.

#ifndef MAPH_TABLECODEBUILDER
#define MAPH_TABLECODEBUILDER   1

#include "symbol.h"
#include "symbolset.h"
#include "parsetable.h"
#include "cpp/nspace.h"
#include "expectation.h"

#include <vector>


struct tablecodebuilder
{

   struct statesymbol
   {
      size_t state;
      symbol sym;

      statesymbol( size_t state, symbol sym )
         : state( state ), sym( sym )
      { }

      struct hash
      {
         size_t operator( ) ( const statesymbol& ) const;
      };

      struct equal_to
      {
         bool operator( ) ( const statesymbol &, const statesymbol& ) const;
      };

      template< typename D > using map =
      std::unordered_map< statesymbol, D, hash, equal_to > ;
   };


   symbol::map< std::pair< size_t, symbolset >> startsymbols;
      // Maps possible start symbols to pairs consisting of
      // initial state, and their associated terminator sets.

   std::vector< std::pair< char, size_t >> reductioninfo;
      // Indexed by the states. 
      // char = 'T' : the state consists of a single reduction (trivial). 
      // char = 'D' : the state has a default reduction, which will be
      //              carried out when lookahead has no shift. 
      // char = 'N' : the state has no reductions.
      // char = 'H' : none of the above. The state is hard. 

   statesymbol::map< size_t > shifts; 
   statesymbol::map< std::vector< size_t >> reductions;

      // We use different tables for shifts and for reductions.
      // This is not inefficient, because most reductions will be handled
      // by defaults, which will be not in the reduction table. 
      // If you would store reductions in the same table as shift,
      // you would not another boolean or whatever to store the fact that 
      // there are no reductions. 
      // I hope that in this way, reductions will be looked at very rarely.

   tablecodebuilder( ) = default;

   void 
   printinspace( const cpp::nspace& space, 
                 const char* c, std::ostream& out ) const;

   void 
   print( const cpp::nspace& space, symbol s, std::ostream& out ) const;

   void
   print( const cpp::nspace& space, statesymbol ss, std::ostream& out ) const;

   void printascode( std::ostream& out, 
                     const cpp::nspace& parserspace,
                     const cpp::nspace& symbolspace ) const;
      // Print them as code that can be included in parser.cpp.
      // prefix is prepended to every token. If you want them to be
      // in a namespace, you have to put it in prefix. 

};

#endif

