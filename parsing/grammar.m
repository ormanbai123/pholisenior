%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{} term def_specifier
%symbol{} struct_specifier struct_declaration_list
%symbol{} args_seq args
%symbol{} type type_list func
%symbol{} iff_expr implication_expr or_expr and_expr not_expr quantifier_expr

%symbol{} IDENTIFIER
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

type => IDENTIFIER
      | func
      ;

func => type LPAR type_list RPAR;    

type_list => type   
           | type_list COMMA type
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

args => LPAR identifiers_colon_type RPAR
	  | LPAR RPAR
	  ;

//-----------------------terms---------------------------------

term => iff_expr
      | quantifier_expr iff_expr
      ;  

iff_expr => implication_expr IFF iff_expr
          | implication_expr IFF quantifier_expr iff_expr
          | implication_expr
          ;

implication_expr => or_expr IMPLY implication_expr
                  | or_expr IMPLY quantifier_expr implication_expr
                  | or_expr
                  ;

or_expr => or_expr OR and_expr 
         | or_expr OR quantifier_expr and_expr
         | and_expr
         ;

and_expr => and_expr AND not_expr 
          | and_expr AND quantifier_expr not_expr
          | not_expr
          ;

not_expr => NOT not_expr 
          | NOT quantifier_expr not_expr
          | IDENTIFIER
          ;

quantifier_expr => LBRACKET identifiers_colon_type RBRACKET
                 | LT identifiers_colon_type GT
                 | quantifier_expr LBRACKET identifiers_colon_type RBRACKET
                 | quantifier_expr LT identifiers_colon_type GT
                 ;
/* 
E => Q E
   | E + F
   | E + Q E
   | F
   ;

F => F MUL G
   | F MUL Q E
   | G
   ;

G => MINUS G 
   | MINUS Q E
   | IDENTIFIER
   | NUM
   ;

Q => FORALL
   | EXISTS
   | Q FORALL
   | Q EXISTS
   ; 
*/

%end

