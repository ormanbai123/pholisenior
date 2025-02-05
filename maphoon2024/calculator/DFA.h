
// This code was generated automatically.
// Generator written by Hans de Nivelle and Dina Muktubayeva.

namespace
{
   template< typename S >
   std::pair< symbolval, size_t > readandclassify( int triv, S& inp )
   {
      auto latest = std::make_pair< symbolval, size_t > ( sym_SCANERROR, 0 );
      if( !inp. has(1))
         return latest;
      char c = inp. peek(0);
      size_t i = 0;

   Q00:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 32:
         ++ i; goto Q01;
      case 33:
         ++ i; goto Q02;
      case 35:
         ++ i; goto Q03;
      case 37:
         ++ i; goto Q04;
      case 40:
         ++ i; goto Q05;
      case 41:
         ++ i; goto Q06;
      case 42:
         ++ i; goto Q07;
      case 43:
         ++ i; goto Q08;
      case 44:
         ++ i; goto Q09;
      case 45:
         ++ i; goto Q10;
      case 46:
         ++ i; goto Q11;
      case 47:
         ++ i; goto Q12;
      case 58:
         ++ i; goto Q14;
      case 59:
         ++ i; goto Q15;
      case 34:
      case 36:
         return latest;
      }
      if( c >= 60 )
      {
         if( c >= 91 )
         {
            if( c >= 123 ) return latest;
            if( c >= 97 ) { ++ i; goto Q16; }
            if constexpr( true ) return latest;
         }
         else
         {
            if( c >= 65 ) { ++ i; goto Q16; }
            if constexpr( true ) return latest;
         }
      }
      else
      {
         if( c >= 48 ) { ++ i; goto Q13; }
         if( c >= 14 ) return latest;
         if( c >= 9 ) { ++ i; goto Q01; }
         if constexpr( true ) return latest;
      }
   Q01:
      latest. first = sym_WHITESPACE; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 32 ) { ++ i; goto Q01; }
      if( c >= 14 ) return latest;
      if( c >= 9 ) { ++ i; goto Q01; }
      if constexpr( true ) return latest;
   Q02:
      latest. first = sym_FACTORIAL; latest. second = i;
      return latest;
   Q03:
      latest. first = sym_EOF; latest. second = i;
      return latest;
   Q04:
      latest. first = sym_MODULO; latest. second = i;
      return latest;
   Q05:
      latest. first = sym_LPAR; latest. second = i;
      return latest;
   Q06:
      latest. first = sym_RPAR; latest. second = i;
      return latest;
   Q07:
      latest. first = sym_TIMES; latest. second = i;
      return latest;
   Q08:
      latest. first = sym_PLUS; latest. second = i;
      return latest;
   Q09:
      latest. first = sym_COMMA; latest. second = i;
      return latest;
   Q10:
      latest. first = sym_MINUS; latest. second = i;
      return latest;
   Q11:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q17; }
      if constexpr( true ) return latest;
   Q12:
      latest. first = sym_DIVIDES; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q18; }
      if( c == 47 ) { ++ i; goto Q19; }
      if constexpr( true ) return latest;
   Q13:
      latest. first = sym_DOUBLE; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 46:
         ++ i; goto Q17;
      case 69:
      case 101:
         ++ i; goto Q20;
      case 47:
         return latest;
      }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q13; }
      if constexpr( true ) return latest;
   Q14:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 61 ) { ++ i; goto Q21; }
      if constexpr( true ) return latest;
   Q15:
      latest. first = sym_SEMICOLON; latest. second = i;
      return latest;
   Q16:
      latest. first = sym_IDENT; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 95 ) { ++ i; goto Q16; }
      if( c == 96 ) return latest;
      if( c >= 65 )
      {
         if( c >= 123 ) return latest;
         if( c >= 97 ) { ++ i; goto Q16; }
         if( c >= 91 ) return latest;
         if constexpr( true ) { ++ i; goto Q16; }
      }
      else
      {
         if( c >= 58 ) return latest;
         if( c >= 48 ) { ++ i; goto Q16; }
         if constexpr( true ) return latest;
      }
   Q17:
      latest. first = sym_DOUBLE; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 69 || c == 101 ) { ++ i; goto Q20; }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q17; }
      if constexpr( true ) return latest;
   Q18:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q22; }
      if constexpr( true ) { ++ i; goto Q18; }
   Q19:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 10 ) { ++ i; goto Q23; }
      if constexpr( true ) { ++ i; goto Q19; }
   Q20:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 43 || c == 45 ) { ++ i; goto Q24; }
      if( c == 44 ) return latest;
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q25; }
      if constexpr( true ) return latest;
   Q21:
      latest. first = sym_ASSIGN; latest. second = i;
      return latest;
   Q22:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q22; }
      if( c == 47 ) { ++ i; goto Q23; }
      if constexpr( true ) { ++ i; goto Q18; }
   Q23:
      latest. first = sym_COMMENT; latest. second = i;
      return latest;
   Q24:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q25; }
      if constexpr( true ) return latest;
   Q25:
      latest. first = sym_DOUBLE; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q26; }
      if constexpr( true ) return latest;
   Q26:
      latest. first = sym_DOUBLE; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q27; }
      if constexpr( true ) return latest;
   Q27:
      latest. first = sym_DOUBLE; latest. second = i;
      return latest;
   }
}

