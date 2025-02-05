
// Written by Hans de Nivelle,

#include "rule.h"
#include <iomanip>

std::ostream& operator << ( std::ostream& stream, const rule& r )
{
   stream << "(" << r. nr << ")   "; 
   stream << r. lhs;
   stream << "  =>  ";
   for( rule::rhs_iterator p = r. rhs_begin( ); p != r. rhs_end( ); ++ p )
   {
      if( p != r. rhs. begin( ))
         stream << " ";
      stream << *p;
   }

   return stream;
}


