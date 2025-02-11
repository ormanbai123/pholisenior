%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{} term def_specifier
%symbol{} struct_specifier struct_declaration_list
%symbol{} args_seq args args_list
%symbol{logic::type} type func 
%symbol{std::vector<logic::type>} type_list
%symbol{} iff_expr implication_expr or_expr and_expr not_expr quantifier_expr

%symbol{std::string} IDENTIFIER
%symbol{} identifier_list identifiers_colon_type
%symbol{} STRUCT DEF FRM

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT LT_EQ GT_EQ ASSIGN
%symbol{} AND OR IMPLY NOT IFF
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} FORALL EXISTS

%symbol{ std::string } SCANERROR

//%symbol{} E F G Q PLUS MUL MINUS NUM

%symbolcode_h { #include "location.h" }
%symbolcode_h { #include <vector> }
%symbolcode_h { #include <string> }
%symbolcode_h { #include "./logic/type.h" }
%symbolcode_h { #include "./logic/selector.h" }
%symbolcode_h { #include "./identifier.h" }


%symbolspace parsing
%parserspace parsing

%parsercode_h { #include "tokenizer.h" }
// %parsercode_h { #include "evaluator.h" }

%infotype {location}

%parameter {tokenizer}              tok
// %parameter {evaluator}              eval

%source { tok.read(); }

%rules 
//-------------------------common--------------------------------

Session => 
	     | Session Statement SEMICOLON
         | Session _recover_ SEMICOLON
         ;

Statement => struct_specifier
           | def_specifier
           ;

identifier_list => IDENTIFIER
				 | IDENTIFIER COMMA identifier_list
				 ;

identifiers_colon_type => identifier_list COLON type;

type => IDENTIFIER:s {return logic::type (logic::type_unchecked, identifier() + s); }
      | func:t {return t;}
      ;

func => type:t LPAR type_list:tl RPAR 
		{return logic::type (logic::type_func, t, tl.begin(), tl.end()); }; 

type_list => type:t {return std::vector<logic::type> (1, t);}
           | type_list:tl COMMA type:t {tl.push_back(t); return tl;}
           ;

//-----------------------structs---------------------------------

struct_specifier => STRUCT IDENTIFIER ASSIGN struct_declaration_list {std::cout << "STRUCT!\n";}; 

struct_declaration_list => identifiers_colon_type
                         | identifiers_colon_type COMMA struct_declaration_list 
                         ;

//-----------------------defs---------------------------------

def_specifier => DEF IDENTIFIER args_seq ASSIGN term {std::cout << "Definition!\n";};		  

args_seq => args_seq args
		  | args
		  ;

args => LPAR args_list RPAR
	  | LPAR RPAR
	  ;

args_list => identifiers_colon_type
		   | identifiers_colon_type COMMA args_list
		   ;

//-----------------------terms---------------------------------

term => iff_expr
      | quantifier_expr iff_expr
      ;  

iff_expr => implication_expr IFF iff_expr
          | implication_expr
          | implication_expr IFF quantifier_expr iff_expr
          ;

implication_expr => or_expr IMPLY implication_expr
                  | LBRACE term RBRACE IMPLY implication_expr
                  | or_expr
                  | or_expr IMPLY quantifier_expr implication_expr
                  | LBRACE term RBRACE IMPLY quantifier_expr implication_expr
                  ;

or_expr => or_expr OR and_expr 
         | LBRACE term RBRACE OR and_expr
         | and_expr
         | or_expr OR quantifier_expr and_expr
         | LBRACE term RBRACE OR quantifier_expr and_expr
         ;

and_expr => and_expr AND not_expr 
          | LBRACE term RBRACE AND not_expr
          | not_expr
          | and_expr AND quantifier_expr not_expr
          | LBRACE term RBRACE AND quantifier_expr not_expr 
          ;

not_expr => NOT not_expr 
          | IDENTIFIER
          | NOT quantifier_expr not_expr
          | LPAR term RPAR
          ;

quantifier_expr => LBRACKET identifiers_colon_type RBRACKET
                 | LT identifiers_colon_type GT
                 | quantifier_expr LBRACKET identifiers_colon_type RBRACKET
                 | quantifier_expr LT identifiers_colon_type GT
                 ;

%end

