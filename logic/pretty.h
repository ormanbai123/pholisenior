
// Everything related to pretty printing. 
// Written by Akhmetzhan Kussainov 
// and Hans de Nivelle.
// May 2023.

#ifndef LOGIC_PRETTY_
#define LOGIC_PRETTY_

#include <iostream>
#include "term.h"
#include "context.h"
#include "beliefstate.h"

#include "uniquenamestack.h"

namespace logic { 
namespace pretty 
{

   struct attractions
   {
      unsigned int left;
      unsigned int right;
         // Higher number means : more attraction. Zero means:
         // No. attraction. at. all. Not. even. the. slighest.

      attractions( ) noexcept 
         : left(0), right(0) 
      { }

      attractions( unsigned int left, unsigned int right )
         : left( left ), right( right )
      { }

      void print( std::ostream& out ) const
         { out << left << '/' << right; } 

   };

   attractions getattractions( selector sel );

   inline 
   std::ostream& operator << ( std::ostream& out, 
                               std::pair< unsigned int , unsigned int > env )
   { 
      out << env. first << '/' << env. second; 
      return out; 
   }

   std::pair< unsigned int, unsigned int > 
   inline 
   between( std::pair< unsigned int, unsigned int > env , attractions attr )
      { return { env. first, attr. left }; }
      
   std::pair< unsigned int, unsigned int >
   inline between( attractions attr1 , attractions attr2 )
      { return { attr1. right, attr2. left }; }

   std::pair< unsigned int, unsigned int >
   inline
   between( attractions attr, std::pair< unsigned int , unsigned int > env )
      { return { attr. right, env. second }; }

   struct parentheses
   {
      unsigned int nr;
    
      parentheses( ) noexcept 
         : nr(0)
      { }

      operator bool( ) const { return nr; }

      bool check( attractions attr,
                  std::pair< unsigned, unsigned int > env );

      void open( std::ostream& out ) const;
      void close( std::ostream& out ) const;
   };


   void print( std::ostream& out, const beliefstate& blfs, 
               const type& tp, std::pair< unsigned int, unsigned int > env );
 
   void print( std::ostream& out, const beliefstate& blfs,
               uniquenamestack& names,
               const term& t, std::pair< unsigned int, unsigned int > env );

   void print( std::ostream& out, const beliefstate& blfs,
               context& ctxt, const term& t );

#if 0
   void print( std::ostream& out, const logic::term& tm,
               logic::context& ctxt, logic::proofstate& state, 
               uniquenamestack& names, attractions attr );

   void print( std::ostream& out, const logic::term& tm,
               logic::context& ctxt, logic::proofstate& state,
               attractions attr = attractions( ));
#endif

   uniquenamestack getnames( const logic::context& ctxt, size_t ss );

   void print( std::ostream& out, 
               uniquenamestack& names, const logic::belief& );

   uniquenamestack 
   print( std::ostream& out, const beliefstate& blfs,
          const context& cxt ); 
      // Print a context. Since a term is often printed
      // with its term, we remember the uniquenamestack,
      // so that it can be used for printing the term. 

   void print( std::ostream& out, const logic::beliefstate& );

}}

#endif