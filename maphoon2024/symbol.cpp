
// This code was produced by Maphoon 2024.
// Code for struct symbol:

#include "symbol.h"

#include <string>



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


const char* parsing::getcstring( symbolval val )
{
   switch( val )
   {
   case sym__recover_ :
      return "_recover_";
   case sym_FRM :
      return "FRM";
   case sym_DEF :
      return "DEF";
   case sym_STRUCT :
      return "STRUCT";
   case sym_IDENTIFIER :
      return "IDENTIFIER";
   case sym_OR :
      return "OR";
   case sym_EXISTS :
      return "EXISTS";
   case sym_FORALL :
      return "FORALL";
   case sym_COLON :
      return "COLON";
   case sym_ASSIGN :
      return "ASSIGN";
   case sym_COMMA :
      return "COMMA";
   case sym_func :
      return "func";
   case sym_SCANERROR :
      return "SCANERROR";
   case sym_struct_specifier :
      return "struct_specifier";
   case sym_type :
      return "type";
   case sym_EQ :
      return "EQ";
   case sym_AND :
      return "AND";
   case sym_DOT :
      return "DOT";
   case sym_GT_EQ :
      return "GT_EQ";
   case sym_identifier_colon_type :
      return "identifier_colon_type";
   case sym_RBRACKET :
      return "RBRACKET";
   case sym_def_specifier :
      return "def_specifier";
   case sym_LPAR :
      return "LPAR";
   case sym_struct_declaration_list :
      return "struct_declaration_list";
   case sym_EOF :
      return "EOF";
   case sym_IMPLY :
      return "IMPLY";
   case sym_term :
      return "term";
   case sym_Statement :
      return "Statement";
   case sym_arg_list :
      return "arg_list";
   case sym_COMMENT :
      return "COMMENT";
   case sym_Session :
      return "Session";
   case sym_identifier_list :
      return "identifier_list";
   case sym_FILEBAD :
      return "FILEBAD";
   case sym_SEMICOLON :
      return "SEMICOLON";
   case sym_GT :
      return "GT";
   case sym_EMPTY :
      return "EMPTY";
   case sym_Expr :
      return "Expr";
   case sym_RPAR :
      return "RPAR";
   case sym_WHITESPACE :
      return "WHITESPACE";
   case sym_LBRACE :
      return "LBRACE";
   case sym_RBRACE :
      return "RBRACE";
   case sym_LBRACKET :
      return "LBRACKET";
   case sym_type_list :
      return "type_list";
   case sym_LT :
      return "LT";
   case sym_LT_EQ :
      return "LT_EQ";
   }
   return "(unknown symbol type)";
      // unreachable, but makes the compiler shut up.
}

void parsing::symbol::print( std::ostream& out ) const
{
   out << val << '(';

   if( info. has_value( ))
      out << info. value( );

   if( std::holds_alternative< std::monostate > ( attr ))
      { out << ')'; return; }

   if( info. has_value( ))
      out << ',';

   if( std::holds_alternative< std::string > ( attr ))
      { ::print_attr( std::get<std::string> ( attr ), out ); out << ')'; return; }

   throw std::runtime_error( "symbol: attribute is corrupted" );
}


bool parsing::symbol::has_correct_attribute( ) const
{
   switch( val )
   {
   case sym_SCANERROR :
      return std::holds_alternative< std::string > ( attr );
   case sym__recover_ :
   case sym_FRM :
   case sym_DEF :
   case sym_STRUCT :
   case sym_IDENTIFIER :
   case sym_OR :
   case sym_EXISTS :
   case sym_FORALL :
   case sym_COLON :
   case sym_ASSIGN :
   case sym_COMMA :
   case sym_func :
   case sym_struct_specifier :
   case sym_type :
   case sym_EQ :
   case sym_AND :
   case sym_DOT :
   case sym_GT_EQ :
   case sym_identifier_colon_type :
   case sym_RBRACKET :
   case sym_def_specifier :
   case sym_LPAR :
   case sym_struct_declaration_list :
   case sym_EOF :
   case sym_IMPLY :
   case sym_term :
   case sym_Statement :
   case sym_arg_list :
   case sym_COMMENT :
   case sym_Session :
   case sym_identifier_list :
   case sym_FILEBAD :
   case sym_SEMICOLON :
   case sym_GT :
   case sym_EMPTY :
   case sym_Expr :
   case sym_RPAR :
   case sym_WHITESPACE :
   case sym_LBRACE :
   case sym_RBRACE :
   case sym_LBRACKET :
   case sym_type_list :
   case sym_LT :
   case sym_LT_EQ :
      return std::holds_alternative< std::monostate > ( attr );
   }

   return false; // because the type is corrupted. 
}

