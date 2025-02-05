
#include "symbol.h" 
#include "tokenizer.h"
#include "parser.h"

void testtokenizer( tokenizer& tok )
{

   symbol sym = tok. read( );
   while( sym. val != sym_EOF )
   {
      std::cout << sym << "\n";
      sym = tok. read( );
   }
   std::cout << "that's end of file\n";
}


int main( int argc, char* argv [] )
{
   std::map< std::string, double > memory;

   memory[ "e" ] = 2.71828182845904523536028747135266249;
   memory[ "pi" ] = 3.14159265358979323846264338327950288; 

   lexing::filereader read( &std::cin, "stdin" );
   tokenizer tok( std::move( read ));

   std::vector< std::string > errorlog;

   parser prs( memory, errorlog, tok );
 
   prs. debug = 1;  
   prs. maxtrialperiod = 0;
   std::cout << "type a command (e.g. a := 4 + 5;)\n"; 
   std::cout << "type show; to see all assigned variables\n";
   std::cout << "type quit; or # to quit\n";

   auto sym = prs. parse( sym_Session );
   std::cout << "parser returned " << sym << "\n";
   return 0;
}


