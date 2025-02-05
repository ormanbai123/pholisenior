
// Written by Hans de Nivelle.

#include "identifier.h"

#include <unordered_set>


bool cpp::identifier::seemssafe( const std::string& s )
{
   if( s. size( ) == 0 || !isalpha( s[0] )) return false; 
      
   for( char c : s )
   {
      if( !isalpha(c) && !isdigit(c) && c != '_' )
         return false;
   }

   // Finally, we compare against keywords, taken from
   // https://en.cppreference.com/w/cpp/keyword

   const static std::unordered_set< std::string > keywords = 
   { "alignas", "alignof", "and", "and_eq", "asm",
     "auto", "bitand", "bitor", "bool", "break", "case",
     "catch", "char", "char8_t", "char16_t", "char32_t",
     "class", "compl", "concept", "const",
     "consteval", "constexpr", "constinit",
     "const_cast", "continue", "co_await", "co_return", "co_yield",

     "decltype", "default", "delete", "do", "double",
     "dynamic_cast", "else", "enum", "explicit", "export",
     "extern", "false", "float", "for", "friend", "goto", "if",
     "inline", "int", "long", "mutable", "namespace",
     "new", "noexcept", "not", "not_eq", "nullptr", 
     "operator", "or", "or_eq", "private", "protected", "public",

     "register", "reinterpret_cast", "requires", "return",
     "short", "signed", "sizeof", "static", "static_assert",
     "static_cast", "struct", "switch", "template", "this",
     "thread_local", "throw", "true", "try", "typedef",
     "typeid", "typename", "union", "unsigned", "using",
     "virtual", "void", "volatile", "wchar_t", "while", 
     "xor", "xor_eq"  
   };

   if( keywords. contains(s) )
      return false;  

   return true; 
}



