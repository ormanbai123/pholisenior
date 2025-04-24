
#include "beliefstate.h"
#include "termoperators.h"
#include "pretty.h"

logic::exact logic::beliefstate::append( belief&& bl )
{

   switch( bl. sel( ))
   {
   case bel_struct:
      {
         auto exstruct = exact( vect. size( ));
            // The exact name that the struct will have.

         structdefs[ bl. name( ) ]. push_back( exstruct );

         // We temporarily insert an empty belief, because we still need
         // access to bl. If we put in the vector already now, it moves 
         // with every insertion, which is annoying.

         vect. emplace_back( belief( bel_empty, identifier( ) ), 
                             dependencies( ));

         auto exconstr = exact( vect. size( ));
         functions[ bl. name( ) ]. push_back( exconstr );

         vect. emplace_back( belief( logic::bel_constr, bl. name( ), exstruct ), 
                             dependencies( ));

         // We also need to create the field functions:

         const structdef& sdef = bl. view_struct( ). def( ); 
            // sdef is not in vect. That would be dangerous.

         for( size_t offset = 0; offset != sdef. size( ); ++ offset )
         {
            auto fieldfunc = belief( logic::bel_fld, 
                                     sdef. at( offset ). name, 
                                     exstruct, offset );
            auto exfld = exact( vect. size( ));
            functions[ sdef. at( offset ). name ]. push_back( exfld ); 
            vect. emplace_back( std::move( fieldfunc ), dependencies( ));
         }

         at( exstruct ). first = std::move( bl );
         return exstruct;
      }      

   case bel_decl:
      {
         exact ex = exact( vect. size( ));
         functions[ bl. name( ) ]. push_back( ex );
         vect. emplace_back( std::move( bl ), dependencies( ));
         return ex; 
      }

   case bel_def:
      {
         exact ex = exact( vect. size( ));
         functions[ bl. name( ) ]. push_back( ex );
         vect. emplace_back( std::move( bl ), dependencies( ));
         return ex;
      }

   case bel_form:
      {
         exact ex = exact( vect. size( )); 
         formulas[ bl. name( ) ]. push_back( ex );
         vect. emplace_back( std::move( bl ), dependencies( ));
         return ex;
      }
      break;

   }

   std::cout << "dont know how to append : " << bl << "\n";
   throw std::runtime_error( "stopping" );
}


const std::vector< logic::exact > & 
logic::beliefstate::getstructdefs( const identifier& id ) const
{
   auto p = structdefs. find( id );
   if( p != structdefs. end( ))
      return p -> second;
   else
      return empty; 
}

const std::vector< logic::exact > & 
logic::beliefstate::getfunctions( const identifier& id ) const
{
   auto p = functions. find( id );
   if( p != functions. end( ))
      return p -> second;
   else
      return empty; 
}

const std::vector< logic::exact > & 
logic::beliefstate::getformulas( const identifier& id ) const
{
   auto p = formulas. find( id );
   if( p != formulas. end( ))
      return p -> second;
   else
      return empty;
}


namespace
{
   void print( std::ostream& out, const std::vector< logic::exact > & uses )
   {
      out << '{';
      for( auto p = uses. begin( ); p != uses. end( ); ++ p )
      {
         if( p != uses. begin( ))
            out << ", ";
         else
            out << " ";
         out << *p;
      }
      out << " }";
   }
}

void logic::beliefstate::print( std::ostream& out ) const
{
   pretty::uniquenamestack names;
   
   out << "Beliefstate:\n"; 
   for( size_t i = 0; i != size(); ++ i ) {
      out << "   " << exact(i) << " : "; 

      const auto& belif = at(exact(i)).first;

      switch( belif.sel( ) )
      {
      case bel_empty:
            out << "empty belief (should not be used)";
            break;

      case bel_struct: {
         out << belif.name( ) << " := ";

         auto& strct_def = belif.view_struct().def(); 

         out << "struct(";
         for( auto p = strct_def.begin(); p != strct_def.end(); ++ p )
         {
            if( p == strct_def.begin()) 
               out << " ";
            else
               out << ", ";
            out << (*p).name << " : ";
            pretty::print(out, *this, (*p).tp, {});  
         }
         out << " )"; 
         break;
      }

      case bel_decl: {
         out << "decl " << belif.name( ) << " : " << belif.view_decl( ). tp( );
         break;
      }
      case bel_def:
         {
            auto d = belif.view_def( );
            out << belif.name( ) << " := ";
            pretty::print( out, *this, names, d.val(), {0,0} ); 
         }
         break;

      case bel_form: {
         out << "form " << belif.name( ) << " : "; 
         
         auto f = belif.view_form();
         pretty::print(out, *this, names, f.form(), {});
         
         break;
      }

      case bel_fld:
         {
            auto f = belif.view_field( );
            out << belif.name( ) << " : field at offset " << f. offset( );
            out << " in ";
            exact ex = f.sdef();

            if (contains(ex))
            { 
               const auto& id = at(ex).first.name();
               if( id.size() == 1 && !names.issafe(id.at(0)) )
                  out << "::";
               out << id;
            }
            else
               out << ex;
         }
         break;

      case bel_constr:
         {
            auto c = belif.view_constr( );
            out << belif.name( ) << " : " << "constructor of ";
            
            auto& ex = c.tp();
            if (contains(ex))
            { 
               auto bel = at(ex).first;
               // Print the type of constructor
               switch(bel.sel()) {
                  case bel_struct: {
                     const auto& strct_def = bel.view_struct().def();
                     out << "(";

                     for (auto itr = strct_def.repr.begin(); itr != strct_def.repr.end(); itr++) {
                        if (itr == strct_def.repr.begin())
                           out << " ";
                        else
                           out << ", ";

                        pretty::print(out, *this, (*itr).tp, {});
                     }

                     out << " )";
                     break;
                  } 

                  default: 
                  // (TODO) Handle other cases if they even exist???
                  {
                     throw std::runtime_error("Something went wrong!");
                  }
               }
            }
            else
               out << ex;

         }
         break;
      default:
         {
            out << "belief has selector: " << belif.sel( ) << "\n";
            throw std::runtime_error( "wrong selector for belief" );
         }
      }

      out << "\n";
   }
   out << "\n";

   out << "Functions:\n";
   for( const auto& f : functions )
   {
      out << "   " << f. first << " :   ";
      
      switch (at(f.second[0]).first.sel())
      {
      case bel_def: {
         out << "{";
         for(auto itr = f.second.begin(); itr != f.second.end(); itr++) {
            auto& b = at(*itr).first;
            auto def = b.view_def();

            if (itr == f.second.begin())
               out << " ";
            else
               out << ", ";

            pretty::print(out, *this, def.tp(), {});
         }
         out << " }";
         break;
      }
      default:
         // (TODO)
         // Finish me
         ::print(out, f.second);
         break;
      }

      out << '\n';
   }
   out << '\n';

   out << "Structdefs:\n";
   for( const auto& sdef : structdefs )
   {
      out << "   " << sdef. first << " :   ";
      
      for(const auto& el : sdef.second) {
         auto& bel = at(el).first;
         auto& strct_def = bel.view_struct().def();
         
         out << "struct(";
         for( auto p = strct_def.begin(); p != strct_def.end(); ++ p )
         {
            if( p == strct_def.begin()) 
               out << " ";
            else
               out << ", ";
            out << (*p).name << " : ";
            pretty::print(out, *this, (*p).tp, {});  
         }
         out << " )"; 
      }
      
      out << '\n';
   }
   out << '\n';

   // (TODO)
   // Finish me
   out << "Formulas:\n";
   for( const auto& f : formulas )
   {
      out << "   " << f. first << " :   ";
      ::print( out, f. second ); 
      out << '\n';
   }
   out << '\n';
}


