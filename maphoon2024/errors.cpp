
// Code written by Hans de Nivelle, April 2006.

#include "errors.h"

std::ostream& operator << ( std::ostream& out, const syntaxerror& err )
{
   out << "Syntax error: " << err. problem;
   out << " at position " << (err.line+1) << "/" << (err.column+1); 
   out << " of inputstream " << err. filename;  

   return out;
}
            
std::ostream& operator << ( std::ostream& out, const generalerror& err )
{
   out << "Error: " << err. problem;
   out << " at position " << (err.line+1) << "/" << (err.column+1); 
   out << " of inputstream " << err. filename;

   return out;
}

std::ostream& operator << ( std::ostream& out, const conflict& confl )
{
   out << "missing precondition in state " << confl. statenr << "\n";
   for( const auto& p : confl. badrules )
   {
      out << "   with lookahead " << p. first;
      out << " involving reductions by rule(s) ";

      for( auto r = p. second. begin( ); r != p. second. end( ); ++ r )
      {
         if( r != p. second. begin( ))
            out << ", ";
         out << *r;
      }
      out << "\n";
   }
   return out;
}

std::ostream& operator << ( std::ostream& out, const ideeproblem& prob )
{
   out << "problem with idee.x in line " << (prob. line+1) << ": ";
   out << prob. problem << "\n";
   return out;
}

