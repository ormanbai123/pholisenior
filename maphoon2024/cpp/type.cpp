
// Written by Hans de Nivelle.

#include "type.h"

bool cpp::type::islight( ) const
{
   // If we are a pointer, we are light:

   if( repr. size( ) >= 2 && repr. back( ) == "*" )
      return true;

   if( repr. size( ) == 1 && repr[0] == "void" )
      return true;

   size_t i = 0;
   while( i < repr. size( ) && 
          ( repr[i] == "short" || repr[i] == "long" || 
            repr[i] == "unsigned" || repr[i] == "signed" ||
            repr[i] == "const" ))
   {
      ++ i;
   }

   if( i == repr. size( ))
      return true;     // Only things like short/long/unsigned/signed. 
 
   if( i + 1 == repr. size( ))
   { 
      const static std::unordered_set< std::string > lighttypes =
         { "bool", "char", "int", "float", "double", "size_t",
           "int8_t", "int16_t", "int32_t", "int64_t",
           "uint8_t", "uint16_t", "uint32_t", "uint64_t" };

      return lighttypes. contains( repr[i] );
   }

   return false; 
}


bool cpp::type::isconst( ) const
{
   return repr. size( ) >= 2 && repr[0] == "const"; 
}


bool cpp::type::isref( ) const
{
   return repr. size( ) >= 2 && repr. back( ) == "&";
}


size_t cpp::type::hash::operator( ) ( const type& tp ) const
{
   std::hash< std::string > h;
   size_t hv = 0;
   for( size_t i = 0; i != tp. repr. size( ); ++ i )
   {
      hv = hv * 17 + h( tp. repr[i] );
         // not great, but good enough. 
   }
   return hv; 
}


bool 
cpp::type::equal_to::operator( ) ( const type& tp1, const type& tp2 ) const
{
   if( tp1. repr. size( ) != tp2. repr. size( ))
      return false; 

   for( size_t i = 0; i != tp1. repr. size( ); ++ i )
   {
      if( tp1. repr[i] != tp2. repr[i] )
         return false; 
   }
   return true; 
}


namespace
{
   // True if no space between s1 and s2 would merge s1 and s2
   // into a single C++ token. 

   bool spacebetween( const std::string& s1, const std::string& s2 )
   {
      if( s1. size( ) == 0 || s2. size( ) == 0 )
         return true;

      if( s1. back( ) == ',' ) 
         return true;

      bool a1 = isalpha( s1. back( )) || isdigit( s1. back( ));
      bool a2 = isalpha( s2. front( )) || isalpha( s2. front( ));

      if( a1 && !a2 ) return false;
      if( !a1 && a2 ) return false;

      return true;
   }
}


void cpp::type::print( std::ostream& out ) const
{
   for( size_t i = 0; i != repr. size( ); ++ i )
   {
      if( i != 0 && spacebetween( repr[i-1], repr[i] ))
         out << ' ';
      out << repr[i];
   }
}


std::string cpp::type::tostring( ) const
{
   std::string res; 

   for( size_t i = 0; i != repr. size( ); ++ i )
   {
      if( i != 0 && spacebetween( repr[i-1], repr[i] ))
         res += ' ';
      res += repr[i];
   }
   return res;
}

void cpp::type::printascode( std::ostream& out, cv_ref r ) const
{
   switch(r)
   {
      case value: print( out ); return; 
      case ref: print( out );  out << " &"; return;
      case cref: out << "const ";  print( out ); out << " &"; return;
      case mref: print( out );  out << " &&"; return;
   }
}


