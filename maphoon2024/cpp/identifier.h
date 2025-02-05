
// Code written by Hans de Nivelle, December 2022. 

#ifndef CPP_IDENTIFIER_ 
#define CPP_IDENTIFIER_   1

#include <string>
#include <vector>
#include <iostream> 

namespace cpp 
{

   // An identifier without the namespace.

   struct identifier
   {
      std::string str;

      identifier( const std::string& str )
         : str( str ) 
      { }

      identifier( std::string&& str )
         : str( std::move( str ))
      { }

      struct hash
      {
         std::hash< std::string > h;  

         size_t operator( ) ( const identifier& id ) const
            { return h( id. str ); }
      };

      struct equal_to
      {
         std::equal_to< std::string > eq; 

         bool operator( ) ( const identifier& id1, 
                            const identifier& id2 ) const 
            { return eq( id1. str, id2. str ); }
      };

      void print( std::ostream& out ) const 
         { out << str; } 

      bool seemssafe( ) const { return seemssafe( str ); }
         // True if the identifier seems safe to use.
         // That means: Not a reserved word and a valid identifier. 

      static bool seemssafe( const std::string& s );
         // Does the real work. 
   };

   inline 
   std::ostream& operator << ( std::ostream& out, const identifier& id )
      { id. print( out ); return out; } 

}

#endif


