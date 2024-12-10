
#ifndef LOGIC_TERMOPERATORS_ 
#define LOGIC_TERMOPERATORS_  

#include "term.h"

namespace logic
{

   inline term prop( const term& t ) 
   {
      return term( op_prop, t );
   }

#if 0
   inline term operator ! ( const term& t ) 
   {
      return term( op_not, t );
   }

   inline term operator || ( const term& t1, const term& t2 ) 
   {
      return term( op_or, t1, t2 );
   }
#endif

   inline term operator && ( const term& t1, const term& t2 )
      { return term( op_and, t1, t2 ); }

#if 0
   inline term lazy_conj( const term& t1, const term& t2 )
   {
      return term( op_lazy_and, t1, t2 );
   }
#endif

   inline term implies( const term& t1, const term& t2 )
      { return term( op_implies, t1, t2 ); }

   inline term lazy_implies( const term& t1, const term& t2 )
      { return term( op_lazy_implies, t1, t2 ); }

#if 0
   inline term equiv( const term& t1, const term& t2 )
   {
      return term( op_equiv, t1, t2 );
   }

   inline term operator == ( const term& t1, const term& t2 )
   {
      return term( op_equals, t1, t2 );
   }

   inline term operator != ( const term& t1, const term& t2 )
   {
      return ! ( t1 == t2 );
   }

#endif

   inline term forall( std::initializer_list< vartype > vars, const term& t )
      { return term( op_forall, t, vars ); }

   inline term exists( std::initializer_list< vartype > vars, const term& t )
      { return term( op_exists, t, vars ); }

#if 0
   term operator + ( const term& t, size_t depth );
      // This is the lifting operator.
#endif

   inline term apply( const term& f, std::initializer_list< term > args )
      { return term( op_apply, f, args ); }

   inline term lambda( std::initializer_list< vartype > vars, const term& t )
      { return term( op_lambda, t, vars ); }

}


inline logic::term operator "" _db( long long unsigned int i )
   { return logic::term( logic::op_debruijn, i ); }

inline logic::term operator "" _unchecked( const char* c, size_t len )
   { return logic::term( logic::op_unchecked, identifier() + c ); }

#endif 
