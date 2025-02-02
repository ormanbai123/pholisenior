
// Written by Hans de Nivelle, February 2021.
// See the licence that was included with this code.

#ifndef MAPH_SYMBOLCODEBUILDER_
#define MAPH_SYMBOLCODEBUILDER_  1

// symbolcodebuilder contains the information that is needed to 
// create the C++ code that defines the class of symbols. 

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "cpp/codefragment.h"
#include "cpp/nspace.h"
#include "cpp/type.h"
#include "errors.h" 
#include "symbol.h"
#include "indentation.h" 


struct symbolcodebuilder
{
   cpp::nspace space; 
      // Namespace of generated symbol class. 
      // The full name will be space::symbol 

   cpp::type infotype; 
      // If infotype is not void, then symbols have a field
      // info of type std::optional< infotype >.
      // The info field is intended for storing source information 
      // about the symbol. Think about things like line number or 
      // filename. 
      // This is the second design. In the first design, symbols had 
      // common fields which could be declared in this class, 
      // but that does not work in practice,
      // because one has to include these common fields in the
      // constructors. While this is reasonable in the tokenizer,
      // it is very annoying when reducing a rule, because the
      // user would have to include source information in the action code, 
      // which spoils the pleasure.
      // In addition, not all symbols have source info, because 
      // they may originate from a reduction with empty right side. 
      // 
      // Our current solution is: During reduction, the lhs automaticaly 
      // receives the info from the first symbol in the rhs that has a 
      // defined info. 
      // Otherwise, it will be undefined. 

   cpp::codefragment code_h; 
   cpp::codefragment code_cpp; 
      // This is code that goes to the beginning of symbol.h 
      // (intended for includes), and code that goes to the 
      // beginning of symbol.cpp (intended for print functions in
      // anonymous namespace). 

   symbol::map< cpp::type > attributes; 
      // All symbols must be inserted here if you want to create 
      // a parser. If a symbol has no attribute,
      // it must be void (which will be replaced by std::monostate in
      // the code).

public:
   symbolcodebuilder( const cpp::nspace& space,
                      const cpp::type& infotype )
      : space( space ),
        infotype( infotype ) 
   { }

   symbolcodebuilder( cpp::nspace&& space,
                      cpp::type&& infotype ) 
      : space( std::move( space )),
        infotype( std::move( infotype ))  
   { }

   void setinfotype( const cpp::type& type ) 
      { infotype = type; } 

   bool addsymbol( symbol sym, const cpp::type& tp );
      // Symbol name, and the type of its attribute.
      // True if the addition took place. 

   cpp::type::map< std::vector< symbol >> attributes2symbolsmap( ) const;
      // Create a map that maps attributes to the symbols that have
      // this attribute. We do not include symbols that have void 
      // attribute, but they must be considered.
      // The first option of std::variant must always be
      // std::monostate. 

   void printconstructor( indentation ind, std::ostream& out,
             const cpp::type& attrtype, cpp::type::cv_ref r ) const;

      // Create a constructor for attribute type attr, which
      // maybe be void. In that case, no argument is created for 
      // the attribute.

   void print_print_attr( std::ostream& ) const;
      // Print default definitions of print_attr into
      // an anonymous namespace.

   void print_h_file( std::ostream& ) const; 
   void print_cpp_file( std::ostream& ) const;
      // Missing tokens must be added first with attribute void,
      // otherwise they will not appear in the constructors and in 
      // the print function. 
};

std::ostream& operator << ( std::ostream& stream, const symbolcodebuilder& );

#endif


