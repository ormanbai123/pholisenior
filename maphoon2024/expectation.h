
#ifndef MAPH_EXPECTATION_
#define MAPH_EXPECTATION_

#include "symbolset.h"

#include <vector>
#include <string>

struct expectation
{

   struct seenonstack
   {
      symbolset set; 
      size_t maxskip;

      seenonstack( const symbolset& set, size_t maxskip )
         : set( set ), maxskip( maxskip )
      { }

      seenonstack( symbolset&& set, size_t maxskip )  
         : set( std::move( set )), maxskip( maxskip )
      { }
                    
   };

   std::vector< seenonstack > trigger; 
      // The pattern on the parse stack that triggers our expectation.
      // { { A, B }, 3 }, { { C }, 0 } means that we want to see 
      // C on top of stack, and an A or B at most 3 positions deeper.

   std::string expected;
      // The message must be something that we expect after trigger.
 
   expectation( ) = delete;

   expectation( const std::vector< seenonstack > & trigger, 
                const std::string& expected )
      : trigger( trigger ),
        expected( expected )
   { }

   expectation( std::vector< seenonstack > && trigger, 
                std::string&& expected )
      : trigger( std::move( trigger )),
        expected( std::move( expected ))
   { }

};

std::ostream& operator << ( std::ostream& out, const expectation& e );

#endif


