
// This code was produced by Maphoon 2023.
// Code for struct symbol:

#include "symbol.h"

#include <string>


namespace {

   template< typename Iter > 
   void print_range( Iter i0, Iter i1, char c0, char c1, std::ostream& out );

#line 50 "grammar.m"
 

   void print_attr( const std::vector< double > & vect, std::ostream& out )
   {
      print_range( vect. begin( ), vect. end( ), '{', '}', out );
   }     

}

namespace {

   template< typename T > 
   void print_attr( const T& t, std::ostream& out )
   {
      out << "(no print)";
   }

   template< typename T > 
   requires requires( const T t, std::ostream& out ) {{ out << t }; }
   void print_attr( const T& t, std::ostream& out )
   {
      out << t;
   }
   
   void print_attr( bool b, std::ostream& out )
   {
      if(b)
         out << "true";
      else
         out << "false";
   }

   void print_char( char c, std::ostream& out )
   {
      if( isprint(c))
         putchar(c);
      else
         out << '{' << (int)c << '}';
   }

   void print_attr( char c, std::ostream& out )
   {
      out << '\'';
      print_char( c, out );
      out << '\'';
   }

   void print_attr( const std::string& s, std::ostream& out )
   {
      out << '"';
      for( char c : s )
         print_char( c, out );
      out << '"';
   }

   template< typename T1, typename T2 > 
   void print_attr( const std::pair<T1,T2> & pr, std::ostream& out )
   {
      out << '[';
      print_attr( pr. first, out ); out << ',';
      print_attr( pr. second, out ); out << ']';
   }

   template< typename Iter > 
   void print_range( Iter i0, Iter i1, char c0, char c1, std::ostream& out )
   {
      out << '{';
      for( auto it = i0; it != i1; ++ it )
      {
         if( it != i0 )
            out << ',';
         print_attr( *it, out );
      }
      out << '}';
   }

}


const char* getcstring( symbolval val )
{
   switch( val )
   {
   case sym__recover_ :
      return "_recover_";
   case sym_DIVIDES :
      return "DIVIDES";
   case sym_DOUBLE :
      return "DOUBLE";
   case sym_TIMES :
      return "TIMES";
   case sym_COMMA :
      return "COMMA";
   case sym_MODULO :
      return "MODULO";
   case sym_MINUS :
      return "MINUS";
   case sym_Show :
      return "Show";
   case sym_SEMICOLON :
      return "SEMICOLON";
   case sym_Command :
      return "Command";
   case sym_WHITESPACE :
      return "WHITESPACE";
   case sym_ASSIGN :
      return "ASSIGN";
   case sym_BAD :
      return "BAD";
   case sym_PLUS :
      return "PLUS";
   case sym_EOF :
      return "EOF";
   case sym_FACTORIAL :
      return "FACTORIAL";
   case sym_Nodebug :
      return "Nodebug";
   case sym_RPAR :
      return "RPAR";
   case sym_Debug :
      return "Debug";
   case sym_IDENT :
      return "IDENT";
   case sym_E :
      return "E";
   case sym_EMPTY :
      return "EMPTY";
   case sym_SCANERROR :
      return "SCANERROR";
   case sym_F :
      return "F";
   case sym_G :
      return "G";
   case sym_H :
      return "H";
   case sym_Quit :
      return "Quit";
   case sym_Arguments :
      return "Arguments";
   case sym_Session :
      return "Session";
   case sym_LPAR :
      return "LPAR";
   case sym_COMMENT :
      return "COMMENT";
   }
   return "(unknown symbol type)";
      // unreachable, but makes the compiler shut up.
}

void symbol::print( std::ostream& out ) const
{
   out << val << '(';

   if( std::holds_alternative< std::monostate > ( attr ))
      { out << ')'; return; }

   if( std::holds_alternative< std::string > ( attr ))
      { ::print_attr( std::get<std::string> ( attr ), out ); out << ')'; return; }
   if( std::holds_alternative< std::vector<double> > ( attr ))
      { ::print_attr( std::get<std::vector<double>> ( attr ), out ); out << ')'; return; }
   if( std::holds_alternative< double > ( attr ))
      { ::print_attr( std::get<double> ( attr ), out ); out << ')'; return; }

   throw std::runtime_error( "symbol: attribute is corrupted" );
}


bool symbol::has_correct_attribute( ) const
{
   switch( val )
   {
   case sym_IDENT :
   case sym_SCANERROR :
      return std::holds_alternative< std::string > ( attr );
   case sym_Arguments :
      return std::holds_alternative< std::vector<double> > ( attr );
   case sym_DOUBLE :
   case sym_E :
   case sym_F :
   case sym_G :
   case sym_H :
      return std::holds_alternative< double > ( attr );
   case sym__recover_ :
   case sym_DIVIDES :
   case sym_TIMES :
   case sym_COMMA :
   case sym_MODULO :
   case sym_MINUS :
   case sym_Show :
   case sym_SEMICOLON :
   case sym_Command :
   case sym_WHITESPACE :
   case sym_ASSIGN :
   case sym_BAD :
   case sym_PLUS :
   case sym_EOF :
   case sym_FACTORIAL :
   case sym_Nodebug :
   case sym_RPAR :
   case sym_Debug :
   case sym_EMPTY :
   case sym_Quit :
   case sym_Session :
   case sym_LPAR :
   case sym_COMMENT :
      return std::holds_alternative< std::monostate > ( attr );
   }

   return false; // because the type is corrupted. 
}

