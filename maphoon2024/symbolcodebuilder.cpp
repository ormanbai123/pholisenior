
// Code written by Hans de Nivelle, November 2006.
// Rewritten June 2008.
// See the licence that was included with this code.


#include "symbolcodebuilder.h"
#include "indentation.h"

bool symbolcodebuilder::addsymbol( symbol sym, const cpp::type& tp )  
{
   auto p = attributes. insert( std::pair( sym, tp ));
   return p. second; 
}

cpp::type::map< std::vector< symbol >> 
symbolcodebuilder::attributes2symbolsmap( ) const
{
   cpp::type::map< std::vector< symbol >> map;

   for( const auto& attr : attributes )
   {
      if( !attr.second.isvoid( ))
      {
         // Nur der Void darf nicht mit Heut. 

         map[ attr. second ]. push_back( attr. first ); 
      }
   }

   return map; 
}


void symbolcodebuilder::printconstructor( 
   indentation ind, std::ostream& out,
   const cpp::type & attrtype, cpp::type::cv_ref r ) const
{
   throw std::runtime_error( "printconstructor is not used any more" );

   if( !attrtype. isvoid( ))
   {
      out << ind << "template< typename _T >\n"; 
      out << (ind+3) << "requires std::same_as< ";
      attrtype. printascode( out );
      out << ", _T > && ";
      if( r == cpp::type::cref )
         out << "std::copy_constructible< _T > \n";
      if( r == cpp::type::mref || r == cpp::type::value )
         out << "std::move_constructible< _T > \n";
   }

   out << ind << "symbol( symbolval val"; 
   ind += 8; 

   if( !infotype. isvoid( )) 
   { 
      out << ",\n" << ind; 
 
      // Light types result in a value parameter, heavy types
      // in a const& parameter:
 
      if( infotype. islight( )) 
         out << "std::optional<infotype> info";  
      else
         out << "const std::optional<infotype> & info";  
   }

   if( !attrtype. isvoid( ))
   {
      out << ",\n" << ind;
      cpp::type( { "_T" } ). printascode( out, r );
      out << " attr )\n";
   }
   else
      out << " )\n";
 
   ind -= 8;

   out << ind << " : val( val )"; 
   ind += 3;

   if( !infotype. isvoid( )) 
   {
      out << ",\n" << ind; 
      out << "info( info )"; 
   }

   if( !attrtype. isvoid( ))
   {
      out << ",\n" << ind;
      if( r == cpp::type::mref ) 
         out << "attr( std::move( attr ))";
      else
         out << "attr( attr )";
   }

   ind -= 3; 
   out << "\n"; out << ind << "{ }\n\n";
}


void symbolcodebuilder::print_print_attr( std::ostream& out ) const
{
   indentation ind; 
   out << "\n";
   out << "namespace {\n\n";
   ind += 3;

   out << ind << "template< typename T > \n";
   out << ind << "void print_attr( const T& t, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   out << \"(no print)\";\n";
   out << ind << "}\n\n";

   out << ind << "template< typename T > \n";
   out << ind << "requires requires( const T t, std::ostream& out ) {{ out << t }; }\n";
   out << ind << "void print_attr( const T& t, std::ostream& out )\n"; 
   out << ind << "{\n";
   out << ind << "   out << t;\n";
   out << ind << "}\n";
   out << ind << "\n";
 
   out << ind << "void print_attr( bool b, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   if(b)\n"; 
   out << ind << "      out << \"true\";\n";
   out << ind << "   else\n";
   out << ind << "      out << \"false\";\n"; 
   out << ind << "}\n\n";

   out << ind << "void print_char( char c, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   if( isprint(c))\n";
   out << ind << "      putchar(c);\n";
   out << ind << "   else\n";
   out << ind << "      out << '{' << (int)c << '}';\n";
   out << ind << "}\n\n";

   out << ind << "void print_attr( char c, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   out << \'\\\'\';\n";
   out << ind << "   print_char( c, out );\n";
   out << ind << "   out << \'\\\'\';\n";
   out << ind << "}\n\n";

   out << ind << "void print_attr( const std::string& s, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   out << '\"';\n";
   out << ind << "   for( char c : s )\n";
   out << ind << "      print_char( c, out );\n"; 
   out << ind << "   out << '\"';\n";
   out << ind << "}\n\n";

   out << ind << "template< typename T1, typename T2 > \n";
   out << ind << "void print_attr( const std::pair<T1,T2> & pr, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   out << '[';\n";
   out << ind << "   print_attr( pr. first, out ); out << ',';\n"; 
   out << ind << "   print_attr( pr. second, out ); out << ']';\n";
   out << ind << "}\n\n";

   out << ind << "template< typename Iter > \n";
   out << ind << "void print_range( Iter i0, Iter i1, char c0, char c1, std::ostream& out )\n";
   out << ind << "{\n";
   out << ind << "   out << '{';\n";
   out << ind << "   for( auto it = i0; it != i1; ++ it )\n";
   out << ind << "   {\n";
   out << ind << "      if( it != i0 )\n";
   out << ind << "         out << ',';\n";
   out << ind << "      print_attr( *it, out );\n";
   out << ind << "   }\n";
   out << ind << "   out << '}';\n";
   out << ind << "}\n\n";

   ind -= 3;

   out << "}\n";
   out << "\n\n"; 
}


void symbolcodebuilder::print_h_file( std::ostream& out ) const 
{
   out << "\n"; 
   out << "// The code below was generated by Maphoon 2024.\n\n";
   out << "// Definition of struct symbol:\n\n"; 
   std::string guard = space. includeguard( "symbol" ); 
   out << "#ifndef " << guard << "\n";
   out << "#define " << guard << "    1\n";
   out << "\n";

   out << "#include <iostream>\n"; 
   out << "#include <variant>\n"; 
   out << "#include <optional>\n";
   out << "#include <concepts>\n";
   out << "#include <stdexcept>\n";

   out << "\n"; 


   code_h. printascode( out );
   if( code_h. size( ))
      out << "\n"; 

   space. open( out ); 

   indentation ind; 
   if( space. size( ))
      ind += 3;

   out << ind << "enum symbolval\n";
   out << ind << "{\n";

   {
      ind += 3;
      out << ind; 
      size_t column = 0; 
      for( auto p = attributes. begin( ); p != attributes. end( ); ++ p )
      {
         if( p != attributes. begin( ))
            out << ", ";
         if( column == 4 ) 
         {
            out << "\n" << ind; 
            column = 0;
         }
         ++ column;
         out << "sym_" << p -> first;
      }
      ind -= 3;
      out << "\n";
      out << ind << "};\n"; 
   }

   out << "\n"; 
   out << ind << "const char* getcstring( symbolval );\n\n";
   out << ind << "inline std::ostream& operator << ";
   out << "( std::ostream& out, symbolval val )\n";
   out << (ind+3) << "{ out << getcstring( val );  return out; }\n"; 

   out << "\n";
   out << ind << "struct symbol\n";
   out << ind << "{\n";
   
   ind += 3;

   cpp::type::map< std::vector< symbol >> attributes_inv = 
      attributes2symbolsmap( );
         // It's the inverse of attributes.

   out << ind << "using attrtype = std::variant < std::monostate";
   size_t i = 0;

   for( const auto& opt : attributes_inv )
   {
      out << ", ";

      if( i % 4 == 2 )
         out << "\n" << ind << "      ";

      opt. first. printascode( out, cpp::type::value ); 

      ++ i;
   }
   out << " > ;\n\n";

   if( !infotype. isvoid( ))
   {
      out << ind << "using infotype = ";
      infotype. printascode( out, cpp::type::value );
      out << ";\n\n"; 
   }
 
   out << ind << "symbolval val;\n";
   if( !infotype. isvoid( )) out << ind << "std::optional<infotype> info;\n"; 
   out << ind << "attrtype attr;\n";
   out << "\n";

   // We define the copy constructors, assignment and the destructor:

   out << ind << "symbol( ) = delete;\n";
   out << ind << "symbol( const symbol& ) = default;\n";
   out << ind << "symbol( symbol&& ) noexcept = default;\n";
   out << ind << "symbol& operator = ( const symbol& ) = default;\n";
   out << ind << "symbol& operator = ( symbol&& ) noexcept = default;\n";
   out << ind << "~symbol( ) = default;\n";
   out << "\n";

   out << ind << "// Construct symbol with void attribute:\n\n";

   if( infotype. isvoid( ))
   {
      out << ind << "symbol( symbolval val )\n";
      out << ind << " : val( val )\n";
      out << ind << "{ }\n";
   }
   else
   {
      out << ind << "symbol( symbolval val,\n";
      out << ind << "        const std::optional<infotype> & info )\n"; 
      out << ind << " : val( val ),\n";
      out << ind << "   info( info )\n";
      out << ind << "{ }\n";
   }
   out << "\n";

   for( cpp::type::cv_ref r : { cpp::type::cref, cpp::type::mref } )
   {
      if( r == cpp::type::cref )
         out << ind << "// Template constructor from const attribute& :\n\n";

      if( r == cpp::type::mref )
         out << ind << "// Template onstructor from attribute&& :\n\n";
 
      out << ind << "template< typename _T >\n";
      out << ind << "   requires std::convertible_to< _T, attrtype > && ";
      if( r == cpp::type::cref )
         out << "std::copy_constructible< _T >\n";

      if( r == cpp::type::mref )
         out << "std::move_constructible< _T >\n";

      out << ind << "symbol( symbolval val,\n";
      if( !infotype. isvoid( ))
         out << ind << "        const std::optional<infotype> & info,\n";
      out << ind << "        ";
      cpp::type( { "_T" } ). printascode( out, r ); 
      out << " attr )\n";

      // Member initializers:
 
      out << ind << " : val( val ),\n";
      if( !infotype. isvoid( ))
         out << ind << "   info( info ),\n";

      if( r == cpp::type::cref )
         out << ind << "   attr( attr )\n";

      if( r == cpp::type::mref )
         out << ind << "   attr( std::move( attr ))\n";
      out << ind << "{ }\n";
      out << "\n";
   }

   out << ind << "void print( std::ostream& out ) const;\n\n";

   out << ind << "bool has_correct_attribute( ) const;\n\n";
 
   out << ind << "template< typename T > T& get( )\n";
   out << ind << "   { return std::get<T> ( attr ); }\n"; 
   out << ind << "\n";

   out << ind << "template< typename T > const T& get( ) const\n";
   out << ind << "   { return std::get<T> ( attr ); }\n";
   out << ind << "\n";

   ind -= 3;
 
   out << ind << "};\n";
   out << "\n";
   out << ind << "inline\n";
   out << ind << "std::ostream& operator << ( ";
   out << "std::ostream& out, const symbol& sym )\n";
   out << ind << "   { sym. print( out ); return out; }\n";

   out << "\n\n";

   space. close( out ); 

   if( space. size( )) 
      out << "\n";

   out << "#endif\n";
   out << "\n\n";
}

void symbolcodebuilder::print_cpp_file( std::ostream& out ) const 
{
   out << "\n"; 
   out << "// This code was produced by Maphoon 2024.\n";
   out << "// Code for struct symbol:\n";
 
   out << "\n";
   out << "#include \"symbol.h\"\n\n";
   out << "#include <string>\n";
   out << "\n\n";

   indentation ind;
   if( !code_cpp. isempty ( ))
   {
      out << "namespace {\n\n";
      ind += 3;

      out << ind << "template< typename Iter > \n";
      out << ind << "void print_range( Iter i0, Iter i1, char c0, char c1, std::ostream& out );\n\n";

      code_cpp. printascode( out );
      ind -= 3;
      out << "}\n";
   }

   print_print_attr( out );

   out << "const char* "; space. printasprefix( out ); 
   out << "getcstring( symbolval val )\n";
   out << "{\n";

   ind += 3;
   out << ind << "switch( val )\n";
   out << ind << "{\n";

   for( const auto& a : attributes )
   {
      out << ind << "case " << "sym_" << a.first << " :\n";
      out << (ind+3) << "return \"" << a.first << "\";\n";
   }
   out << ind << "}\n";
   out << ind << "return \"(unknown symbol type)\";\n";
   out << "      // unreachable, but makes the compiler shut up.\n";
   ind -= 3;
   out << ind << "}\n";
   out << "\n";

   ind. clear( );


   out << "void "; space.printasprefix( out ); out << "symbol::print( ";
   out << "std::ostream& out ) const\n";
   out << "{\n";

   ind += 3; 
   out << ind << "out << val << '(';\n\n";
  
   cpp::type::map< std::vector< symbol >> attributes_inv =
      attributes2symbolsmap( );
         // It's the inverse of attributes.

   if( !infotype. isvoid( )) 
   {
      out << ind << "if( info. has_value( ))\n"; 
      out << (ind+3) << "out << info. value( );\n\n";
   }

   out << ind << "if( std::holds_alternative< std::monostate > ( attr ))\n";
   out << (ind+3) << "{ out << ')'; return; }\n\n";

   if( !infotype. isvoid( ) && attributes_inv. size( )) 
   {
      out << ind << "if( info. has_value( ))\n";
      out << (ind+3) << "out << ',';\n\n";
   }


   for( const auto& opt : attributes_inv ) 
   {
      out << ind << "if( std::holds_alternative< ";
      opt. first. printascode( out, cpp::type::value );
      out << " > ( attr ))\n";
    
      out << (ind+3) << "{ ::print_attr( std::get<";
      opt. first. printascode( out, cpp::type::value );
      out << "> ( attr ), out ); ";
      out << "out << ')'; return; }\n"; 
   }

   if( attributes_inv. size( ))
      out << "\n";

   out << ind << 
      "throw std::runtime_error( \"symbol: attribute is corrupted\" );\n"; 

   ind -= 3; 
   out << ind << "}\n\n";

   ind. clear( );
   out << "\n";

   out << "bool "; space.printasprefix( out ); 
   out << "symbol::has_correct_attribute( ) const\n";
   out << "{\n";

   ind += 3;

   out << ind << "switch( val )\n";
   out << ind << "{\n"; 
  
   for( const auto& attr: attributes_inv )
   {
      for( symbol s : attr. second )
         out << ind << "case sym_" << s << " :\n";

      ind += 3;
      out << ind << "return std::holds_alternative< ";
      attr. first. printascode( out, cpp::type::value );
      out << " > ( attr );\n";

      ind -= 3;
   } 

   for( const auto& attr : attributes )
   {
      if( attr.second.isvoid( ))
      {
         out << ind << "case sym_" << attr. first << " :\n"; 
      }
   }

   ind += 3;
   out << ind << "return std::holds_alternative< std::monostate > ( attr );\n";

   ind -= 3;
   out << ind << "}\n\n";

   out << ind << "return false; // because the type is corrupted. \n";

   ind -= 3; 
   out << ind << "}\n\n";

}


std::ostream& 
operator << ( std::ostream& out, const symbolcodebuilder& bld )
{
   out << "symbolcodebuilder:\n\n";
   if( !bld. code_h. isempty( )) 
      out << "code for .h:\n" << bld. code_h << "\n";

   out << "namespace:\n   " << bld. space << "\n\n";

   out << "infotype: " << bld. infotype << "\n\n";

   if( !bld. code_cpp. isempty( )) 
   {
      out << "code for .cpp:\n"; out << bld. code_cpp << "\n";
   }

   out << "declared attributes:\n"; 

   for( const auto& a : bld. attributes ) 
      out << "   " << a. first << " : " << a. second << "\n";
   out << "\n";

   return out;
}


