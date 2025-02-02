
// Written by Hans de Nivelle, January 2021.

#ifndef MAPH_SYMBOL_
#define MAPH_SYMBOL_   1

#include <string>
#include <iostream>
#include <unordered_set> 
#include <unordered_map>

class symbol 
{
   using indextype = std::unordered_set< std::string > ; 
   indextype::const_iterator p; 

public: 
   symbol( const std::string& s )
      : p( getindex( ). insert(s). first ) 
   { }

   symbol( const char* c ) 
      : symbol( std::string(c) )
   { }

   const std::string& getstring( ) const
      { return *p; } 

   static void printtable( std::ostream& stream );
      // Prints table of all symbols. 

   static symbol recover( ); 
      // Recover can be used for setting recovery points in 
      // rules. Like in Expression -> RECOVER dot.
      // Maphoon will try to recover on a dot after a syntax error.

   static indextype& getindex( ) 
   {
      static indextype ind;
      return ind;
   }

   bool operator == ( symbol s ) const { return p == s.p; } 
   bool operator != ( symbol s ) const { return p != s.p; } 

   struct hash
   {
      size_t operator( ) ( symbol s ) const
         { return reinterpret_cast<size_t> ( & * s.p ); }
   };

   struct equal_to
   {
      bool operator( ) ( symbol s1, symbol s2 ) const
         { return s1 == s2; }
   };

   template< typename D > using map =
   std::unordered_map< symbol, D, hash, equal_to > ;
};

inline std::ostream& operator << ( std::ostream& out, symbol s )
{
   out << s. getstring( ); 
   return out;
}

template< typename D > std::ostream&
operator << ( std::ostream& stream, const symbol::map<D> & mp )
{
   for( const auto& p : mp )
   {
      stream << "   " << p. first << " : " << p. second << "\n";
   }

   return stream;
}

#endif


