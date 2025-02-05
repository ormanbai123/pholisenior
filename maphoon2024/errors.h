
// this file defines syntaxerror and readerror. A readerror 
// means that reading of input was attempted and failed, for some
// reason different than encountering end of file.
//
// A syntaxerror means that it was attempted to read a term, 
// that enough characters were successfully read, but that they could not
// be parsed according to the input grammar.
//
// Code written by Hans de Nivelle, April 2006.

#ifndef MAPH_ERRORS_
#define MAPH_ERRORS_ 1 

#include <string>
#include <iostream>

#include "itemsets.h"

struct syntaxerror
{
   std::string problem;
      
   std::string filename;
   long unsigned int line;
   long unsigned int column;

   syntaxerror( const std::string& problem,
		const std::string& filename,
		long unsigned int line,
                long unsigned int column ) 
      : problem( problem ),
        filename( filename ),
	line( line ),
        column( column ) 
   { }

};

std::ostream& operator << ( std::ostream&, const syntaxerror& ); 


struct generalerror
{
   std::string problem;

   std::string filename;
   long unsigned int line;
   long unsigned int column; 

   // The description of the problem should be such that in can
   // be followed by 'in line ... of file ...'.

   generalerror( const std::string& problem,
                 const std::string& filename,
                 long unsigned int line,
                 long unsigned int column ) 
      : problem( problem ),
        filename( filename ),
	line( line ),
        column( column ) 
   { }
};

std::ostream& operator << ( std::ostream&, const generalerror& );


struct conflict 
{
   size_t statenr;
   symbol::map< std::vector< size_t >> badrules;
      // Maps lookahead symbols to the numbers of the problematic rules. 
      // If the left hand sides are not included in one of the reduction 
      // sequences, then these are all rules for which a reduction is 
      // possible. 
      // Otherwise, these are the rules that are not last, and that
      // have no condition (requires) attached. 

   explicit conflict( size_t statenr )
      : statenr( statenr )
   { } 

   bool isempty( ) const 
      { return badrules. size( ) == 0; } 

};

std::ostream& operator << ( std::ostream&, const conflict& );


struct ideeproblem 
{
   std::string problem; 
   long unsigned int line;
  
   ideeproblem( const std::string& problem, long unsigned int line )
      : problem( problem ), line( line )
   { }
};

std::ostream& operator << ( std::ostream&, const ideeproblem& ); 

#endif


