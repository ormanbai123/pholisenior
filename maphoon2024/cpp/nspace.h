
// Code written by Hans de Nivelle, Spring 2022. 

#ifndef CPP_NSPACE_
#define CPP_NSPACE_  1

#include <string>
#include <vector>
#include <iostream> 

namespace cpp 
{

   // A C++ namespace:

   class nspace 
   {
      std::vector< std::string > repr;

   public:
      nspace( ) = default;

      bool seemssafe( ) const; 
         // True if the namespace seems save to use.
         // It should consist of identifiers and not contain reserved words.

      size_t size( ) const { return repr. size( ); } 

      void printasprefix( std::ostream& out ) const; 
         // print the namespace as prefix to a name. 

      void open( std::ostream& out ) const;
      void close( std::ostream& out ) const; 
      std::string includeguard( const std::string& filename ) const;

      void print( std::ostream& out ) const;
         // Print in readable fashion. Should not be used for code
         // generation. Use printasprefix. 

      nspace& operator += ( const std::string& s ) 
      {
         repr. push_back(s);
         return *this;
      }

   };

   inline
   nspace operator + ( nspace sp, const std::string& s )
   {
      sp += s;
      return sp;
   }

   inline 
   std::ostream& operator << ( std::ostream& out, const nspace& sp )
   {
      sp. print( out ); return out;
   }

}


#endif


