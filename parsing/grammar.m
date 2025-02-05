%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{} term def_specifier
%symbol{} struct_specifier struct_declaration_list
%symbol{} args_seq args
%symbol{} type type_list func

%symbol{} IDENTIFIER
%symbol{} identifier_list identifiers_colon_type
%symbol{} STRUCT DEF FRM

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT LT_EQ GT_EQ ASSIGN
%symbol{} AND OR IMPLY
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


term => IDENTIFIER;

identifier_list => IDENTIFIER
				 | IDENTIFIER COMMA identifier_list
				 ;

identifiers_colon_type => identifier_list COLON type;

type => IDENTIFIER
      | func
      ;

func => type LPAR type_list RPAR
      ;    

type_list => type   
          | type_list COMMA type
          ;

//-----------------------structs---------------------------------

struct_specifier => STRUCT IDENTIFIER ASSIGN struct_declaration_list {std::cout << "STRUCT!\n";}
                  ; 

struct_declaration_list => identifiers_colon_type
                         | identifiers_colon_type COMMA struct_declaration_list 
                         ;


//-----------------------defs---------------------------------


def_specifier => DEF IDENTIFIER args_seq ASSIGN {std::cout << "Definition!\n";};		  
args_seq => args_seq args
		  | args
		  ;

args => LPAR identifiers_colon_type RPAR
	  | LPAR RPAR
	  ;

//-----------------------defs---------------------------------

/* 
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

