#include "tokenizer.h"

parsing::tokenizer::tokenizer( lexing::filereader&& inp )
   :inp(std::move(inp))
{}

lexing::classifier<char, parsing::symbolval>
parsing::tokenizer::buildclassifier()
{
    using namespace lexing;

    classifier<char, symbolval> cls(symbolval::sym_SCANERROR);   

   auto letter = range('a', 'z') | range('A', 'Z');
    auto digit = range('0', '9');
    cls.insert((just('_') | letter) * (just('_') | letter | digit).star(), symbolval::sym_IDENTIFIER);

   cls.insert(just('O'), parsing::symbolval::sym_OBJ);
   cls.insert(just('T'), parsing::symbolval::sym_TRUTHVAL);

   // Single-double char tokens
   cls.insert(just(']'), parsing::symbolval::sym_RBRACKET);
   cls.insert(just('['), parsing::symbolval::sym_LBRACKET);
   cls.insert(just(')'), parsing::symbolval::sym_RPAR);
   cls.insert(just('('), parsing::symbolval::sym_LPAR);
   cls.insert(just('}'), parsing::symbolval::sym_RBRACE);
   cls.insert(just('{'), parsing::symbolval::sym_LBRACE);

   cls.insert(word(":="), parsing::symbolval::sym_ASSIGN);
   cls.insert(just('='), parsing::symbolval::sym_EQ);
   cls.insert(just('<'), parsing::symbolval::sym_LT);
   cls.insert(just('>'), parsing::symbolval::sym_GT);
   cls.insert(word("<="), parsing::symbolval::sym_LT_EQ);
   cls.insert(word(">="), parsing::symbolval::sym_GT_EQ);

   cls.insert(just('&'), parsing::symbolval::sym_AND);
   cls.insert(just('|'), parsing::symbolval::sym_OR);
   cls.insert(word("->"), parsing::symbolval::sym_IMPLY);

   cls.insert(just(','), parsing::symbolval::sym_COMMA);
   cls.insert(just(':'), parsing::symbolval::sym_COLON);
   cls.insert(just(';'), parsing::symbolval::sym_SEMICOLON);

   cls.insert(just('#'), parsing::symbolval::sym_EOF);
   //

   // Keywords:
   cls.insert(word("struct"), parsing::symbolval::sym_STRUCT);
   cls.insert(word("def"), parsing::symbolval::sym_DEF);
   cls.insert(word("frm"), parsing::symbolval::sym_FRM);
   //

   cls.insert((just(' ') | just('\f') | just('\n') | just('\r') | just('\t') | just('\v')).plus(),
    parsing::symbolval::sym_WHITESPACE);

   cls.insert(word("//") * every<char>().without('\n').star() * just('\n'),
    parsing::symbolval::sym_COMMENT);

   cls.insert(word("/*") *
   (every<char>().without('*') | (just('*').plus() * every<char>().without('/').without('*'))).star()
   * (just('*').plus() * just('/')),
   parsing::symbolval::sym_COMMENT);

   return minimize(make_deterministic(cls));
}

parsing::symbol
parsing::tokenizer::read( )
{
   static auto cls = buildclassifier( );

restart:
   location startloc = getlocation();

   if( !inp.has(1))
      return symbol(sym_EOF, startloc);

   if( !inp.good() )
      return symbol(sym_FILEBAD, startloc);

   auto p = readandclassify( cls, inp );
  
   if( p.second == 0 )
   {
      std::string attr = std::string( inp.view(1) );
      inp.commit(1);
      return symbol( sym_SCANERROR, startloc ); 
   }

   if( p.first == sym_WHITESPACE || p.first == sym_COMMENT )
   {
      inp.commit( p.second );
      goto restart; 
   }

   if( p.first == sym_IDENTIFIER )
   {
      inp.commit( p.second );
      return symbol( sym_IDENTIFIER, startloc);
   }
   // All the remaining tokens have no attribute:

   inp.commit( p. second );
   return symbol( p. first, startloc );
}


void parsing::tokenizer::test() {
   auto sym = read();

   while(sym.val != symbolval::sym_EOF && sym.val != symbolval::sym_SCANERROR) {

      std::cout << sym << "\n";

      sym = read();
   }
   std::cout << sym << "\n";
}