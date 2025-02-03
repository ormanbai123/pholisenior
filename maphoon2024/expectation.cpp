
#include "expectation.h"

std::ostream& operator << ( std::ostream& out, const expectation& e )
{
   for( auto p = e. trigger. begin( ); p != e. trigger. end( ); ++ p )
   {
      if( p != e. trigger. begin( ))
         out << " ";

      const auto& set = p -> set;
 
      if( set. size( ) == 0 )
         out << "(IMPOSSIBLE)";
      else
         if( set. size( ) == 1 )
            out << * ( set. begin () );
         else 
         {
            out << "( ";
            for( auto s = set. begin( ); s != set. end( ); ++ s ) 
            {
               if( s != set. begin( ))
                  out << " | "; 
               out << *s;
            }
            out << " )";
         }

      size_t maxskip = p -> maxskip;
      if( maxskip > 0 )
      {
         if( maxskip <= 3 )
         {
            out << " *";
            for( size_t i = 1; i < maxskip; ++ i )
               out << " *";
         }
         else
            out << " " << maxskip; 
      }
   }
   out << " => " << e. expected;
   return out; 
}

