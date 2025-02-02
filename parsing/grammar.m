%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr
%symbol{} term def_specifier arg_list
%symbol{} struct_specifier struct_declaration_list struct_declaration
%symbol{} type type_list func

%symbol{ std::string } SCANERROR

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT LT_EQ GT_EQ ASSIGN  
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} FORALL EXISTS

%symbol{} AND OR IMPLY

%symbol{} IDENTIFIER
%symbol{} identifier_list
%symbol{} STRUCT DEF FRM

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
Session => 
    | Session Statement SEMICOLON
    | Session _recover_ SEMICOLON
    ;

Statement => struct_specifier
           | def_specifier
           ;


term => IDENTIFIER;

//-----------------------structs---------------------------------

struct_specifier => STRUCT IDENTIFIER ASSIGN struct_declaration_list {std::cout << "STRUCT!\n";}
                  ; 

struct_declaration_list => struct_declaration
                         | struct_declaration COMMA struct_declaration_list 
                         ;
identifier_list => IDENTIFIER
				 | IDENTIFIER COMMA identifier_list
				 ;

struct_declaration => identifier_list COLON type
                    ; 

type => IDENTIFIER
      | func
      ;

func => type LPAR type_list RPAR
      ;    

type_list => type   
          | type_list COMMA type
          ;

//-----------------------defs---------------------------------

def_specifier => DEF IDENTIFIER LPAR arg_list RPAR ASSIGN term {std::cout << "Definition!\n";}
               ;

arg_list => 
          | struct_declaration_list
          ;


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

