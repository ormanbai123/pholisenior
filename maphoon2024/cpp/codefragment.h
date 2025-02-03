
// Code written by Hans de Nivelle, Spring 2022. 

#ifndef CPP_CODEFRAGMENT_
#define CPP_CODEFRAGMENT_  1

#include <string>
#include <vector>
#include <iostream> 
#include <unordered_map>


namespace cpp 
{

   // A fragment of C++ code. It is important that it is output 
   // exactly as input without changes in spacing. 

   struct codefragment
   {
      std::string filename;        // As used by the operating system. 
      long unsigned int line; 
      long unsigned int column;      

      std::string repr;
         // The code without change. 

      std::vector< std::pair< size_t, size_t >> identifiers; 
         // Positions of replaceable identifiers in repr.
 
      codefragment( ) 
         : line(0), 
           column(0) 
      { }

      void push_back( std::string_view v, bool id = false );
         // The parameter id should be true if
         // v should be considered as identifier. 
         // In that case we also extend identifiers.
         // It works with std::string and const char* v too.

      void print( std::ostream& out ) const; 

      void printlinedirective( std::ostream& out ) const; 
         // print #line ....

      void printascode( std::ostream& out ) const; 
      void printascode( std::ostream& out, 
             const std::unordered_map< std::string, std::string > & ) const; 

         // If filename is not the empty string, we print a  
         // line directive in front of the code.

      size_t size( ) const { return repr. size( ); } 

      bool isempty( ) const; 
         // True if we are empty, which means that we contain only whitespace.

   };

   inline 
   std::ostream& operator << ( std::ostream& out, const codefragment& frag )
   {
      frag. print( out ); return out;
   }

}

inline
std::ostream&
operator << ( std::ostream& out, 
              const std::unordered_map< std::string, std::string > & repl )
{
   out << "variable replacement:\n";
   for( const auto& p : repl )
   {
      out << "   " << p. first << " --> " << p. second << "\n";
   }
   return out;
}

#endif


