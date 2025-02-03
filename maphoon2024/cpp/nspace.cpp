
// Written by Hans de Nivelle.

#include "nspace.h"
#include "identifier.h"

bool cpp::nspace::seemssafe( ) const
{
   for( const auto& s : repr )
   {
      if( !identifier::seemssafe(s) )
         return false;
   }
   return true;
}


void cpp::nspace::printasprefix( std::ostream& out ) const 
{
   for( const auto& s : repr ) 
      out << s << "::"; 
}


void cpp::nspace::open( std::ostream& out ) const

{
   for( const auto& n : repr )
      out << "namespace " << n << " { ";
   if( repr. size( ))
      out << "\n\n";
}


void cpp::nspace::close( std::ostream& out ) const
{
   for( const auto& n : repr )
      out << "}";
   if( repr. size( ))
      out << "\n";
}


std::string 
cpp::nspace::includeguard( const std::string& filename ) const 
{
   std::string guard; 

   for( const auto& n : repr ) 
   {
      for( char c : n )
         guard. push_back( toupper( c )); 
      guard. push_back( '_' );
   }
 
   for( char c : filename )
      guard. push_back( toupper(c));

   guard. push_back( '_' );
 
   return guard;
}


void cpp::nspace::print( std::ostream& out ) const
{
   for( size_t i = 0; i != size( ); ++ i ) 
   {
      if(i) out << " :: ";
      out << repr[i];
   }
}



