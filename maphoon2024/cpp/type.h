
// Code written by Hans de Nivelle, Spring 2022. 

#ifndef CPP_TYPE_
#define CPP_TYPE_  1 

#include <string>
#include <vector>
#include <iostream> 
#include <unordered_map>
#include <unordered_set> 


namespace cpp 
{

   struct type
   {
      std::vector< std::string > repr;  

      type( ) = delete;

      type( const std::vector< std::string > & repr )
         : repr( repr )
      { }

      type( std::vector< std::string > && repr )
         : repr( std::move( repr ))
      { }

      bool isvoid( ) const
         { return repr. size( ) == 1 && repr[0] == "void"; }

      bool islight( ) const;
         // True if we are light to copy. In that case, we don't
         // create const& and && constructurs, but a simple
         // value constructor.

      bool isconst( ) const;
         // True if we start with 'const'.

      bool isref( ) const;
         // True if we end with '&' or '&&'.

      struct hash
      {
         std::hash< std::string > h;
         size_t operator( ) ( const type& t ) const;
      };

      struct equal_to
      {
         std::equal_to< std::string > eq;
         bool operator( ) ( const type& t1, const type& t2 ) const;
      };

      enum cv_ref { value, ref, cref, mref };
         // We have value, reference, const reference, moving reference.

      void print( std::ostream& out ) const; 
      std::string tostring( ) const;

      void printascode( std::ostream& out, cv_ref r = value ) const;

      using set = std::unordered_set< type, hash, equal_to > ;

      template< typename D > using map = 
         std::unordered_map< type, D, hash, equal_to > ; 
   }; 

   inline 
   std::ostream& operator << ( std::ostream& out , const type& type )
      { type. print( out ); return out; }  

}

#endif


