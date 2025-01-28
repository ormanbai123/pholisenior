%startsymbol Session EOF

%symbol Session
%symbol{} Expr

%symbol{ std::string } SCANERROR

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT LT_EQ GT_EQ ASSIGN  
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} OBJ TRUTHVAL

%symbol{} FORALL EXISTS

%symbol{} AND OR IMPLY

%symbol{} IDENTIFIER
%symbol{} STRUCT DEF FRM

//%symbol{} E F G Q PLUS MUL MINUS NUM

%symbol{} struct_specifier struct_declaration_list struct_declaration type type_list func

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
    | Session Expr SEMICOLON
    | Session _recover_ SEMICOLON
    ;

Expr => struct_specifier
     ;

struct_specifier => STRUCT IDENTIFIER ASSIGN struct_declaration_list {std::cout << "STRUCT!\n";}
                  ; 

struct_declaration_list => struct_declaration
                         | struct_declaration COMMA struct_declaration_list 
                         ;

struct_declaration => IDENTIFIER COLON type 
                    ; 

type => IDENTIFIER
      | func
      ;

func => type LPAR type_list RPAR
      ;    

type_list => type   
          | type_list COMMA type
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

