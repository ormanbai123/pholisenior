
// Written by Hans de Nivelle, March 2021.

#include "tablecodebuilder.h"

size_t 
tablecodebuilder::statesymbol::hash::operator( ) ( 
                       const statesymbol& ss ) const
{
   symbol::hash symhash; 

   // After some trying, and looking at the distribution in the map,
   // this gives decent results:

   auto s = ss. state;
   auto t = symhash( ss. sym );

   size_t h = s + 41 * t + t * t;
   h ^= ( h >> 7 );
   return h;
}

bool
tablecodebuilder::statesymbol::equal_to::operator( ) ( 
                       const statesymbol& ss1,
                       const statesymbol& ss2 ) const
{
   return ( ss1. state == ss2. state ) && ( ss1. sym == ss2. sym );
}

void
tablecodebuilder::printinspace( const cpp::nspace& space,
                                const char* c,
                                std::ostream& out ) const
{
   space. printasprefix( out );
   out << c;
}

void 
tablecodebuilder::print( const cpp::nspace& space, symbol s, 
                         std::ostream& out ) const
{
   space. printasprefix( out );
   out << "sym_" << s;
}

void tablecodebuilder::print( const cpp::nspace& space, statesymbol ss,
                              std::ostream& out ) const
{
   out << "{ " << ss. state << ", ";
   print( space, ss. sym, out ); 
   out << " }"; 
}

void 
tablecodebuilder::printascode( std::ostream& out, 
                               const cpp::nspace& parserspace,
                               const cpp::nspace& symbolspace ) const
{
   // The start symbols:

   out << "const std::unordered_map< ";
   printinspace( symbolspace, "symbolval", out );
   out << ", std::pair< size_t, std::unordered_set< ";
   printinspace( symbolspace, "symbolval >>> ", out );
   printinspace( parserspace, "parser::startsymbols =\n", out );
   out << "{\n";

   size_t cnt = startsymbols. size( ); 
   for( const auto& p : startsymbols ) 
   {
      out << "   { ";
      print( symbolspace, p. first, out );
      out << ", { " << p. second. first << ", { ";
      for( auto q = p. second. second. begin( );
                q != p. second. second. end( );
                ++ q )
      {
         if( q != p. second. second. begin( ))
            out << ", ";
         print( symbolspace, *q, out );
      }
      out << " }} }";
      if( -- cnt ) out << ",\n";
   }
   out << "\n};\n";

   out << "\n";

   // Information about reductions per state:

   out << "const std::vector< std::pair< char, size_t >> ";
   printinspace( parserspace, "parser::reductioninfo =\n", out );
   out << "{\n   ";

   for( size_t i = 0; i != reductioninfo. size( ); ++ i )
   {
      if( i != 0 ) 
      {
         if( i % 8 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ " << "'" << reductioninfo[i]. first << "', ";
      out << reductioninfo[i]. second << " }";
   }
   out << "\n};\n\n";

   out << "const "; 
   printinspace( parserspace, "parser::statesymbol::map< size_t > ", out );
   printinspace( parserspace, "parser::shifts =\n", out ); 
   out << "{\n   ";

   size_t nr = 0; 
   for( const auto& p : shifts ) 
   {
      if( nr != 0 )
      {
         if( nr % 5 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ "; print( symbolspace, p. first, out ); 
      out << ", " << p. second << " }";
      ++ nr;
   }
   out << "\n};\n\n";
  
   out << "const "; 
   printinspace( parserspace, 
                 "parser::statesymbol::map< std::vector< size_t >> ", out );
   printinspace( parserspace, "parser::reductions =\n", out );   

   out << "{\n   ";
     
   nr = 0;
   for( const auto& p : reductions )
   {
      if( nr != 0 )
      {
         if( nr % 4 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ "; print( symbolspace, p. first, out );
      out << ", { ";
      for( size_t i = 0; i != p. second. size( ); ++ i ) 
      {
         if(i) out << ", ";
         out << p. second[i];
      }
      out << " } }";
      ++ nr; 
   } 
   out << "\n};\n"; 
}


