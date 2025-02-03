
// Written by Hans de Nivelle.

#include "codefragment.h"


void cpp::codefragment::push_back( std::string_view v, bool id ) 
{
   if( id )
   {
      size_t s = repr. size( );
      repr += v;
      
      identifiers. push_back( std::pair( s, repr. size( )) );
   }
   else
      repr += v;
}


bool cpp::codefragment::isempty( ) const
{
   for( char c : repr )
   {
      if( !isspace(c))
         return false;
   }
   return true;
}


void cpp::codefragment::print( std::ostream& out ) const
{
   out << "from file " << filename << ", line " << line << " :\n"; 
   size_t pos = 0;
   size_t id = 0;
   size_t ln = line; 
   out << "   " << ( ln ++ ) << " :   ";

   while( pos != repr. size( )) 
   {
      if( id < identifiers. size( ) && identifiers[id]. first == pos )
         out << '{';
      if( id < identifiers. size( ) && identifiers[id]. second == pos )
      {
         out << '}';  
         ++ id; 
      }
      char c = repr[ pos ++ ];
      out << c;
      if( c == '\n' && pos != repr. size( ))
      {
         out << "   " << ( ln ++ ) << " :   ";
      }         
   }
   out << "\n";
}


void cpp::codefragment::printlinedirective( std::ostream& out ) const 
{
   out << "#line " << ( line + 1 ) << " \"";

   for( char c : filename )
   {
      if( c != '\\' )
         out. put(c);
      else
         out. put( '/' );
   }

   out << "\"" << "\n";
}


void
cpp::codefragment::printascode( std::ostream& out ) const
{
   if( filename. size( )) 
      printlinedirective( out ); 

   out << repr;
}


void cpp::codefragment::printascode( std::ostream& out, 
         const std::unordered_map< std::string, std::string > & repl ) const
{
   if( filename. size( )) 
      printlinedirective( out );
 
   size_t pos = 0;
   for( size_t i = 0; i != identifiers. size( ); ++ i )
   {
      while( pos < identifiers[i]. first )
         out << repr[ pos ++ ];
      
      std::string id;
      while( pos < identifiers[i]. second )
         id. push_back( repr[ pos ++ ] );
      
      auto p = repl. find( id );
      if( p == repl. end( ))
         out << id;
      else
         out << ( p -> second );
   }

   while( pos != repr. size( )) 
      out << repr[ pos ++ ];
}


