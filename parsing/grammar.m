%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{} term def_specifier
%symbol{logic::belief} struct_specifier
%symbol{std::stack<std::vector<std::pair<std::vector<std::string>, logic::type>>>} args_seq 
%symbol{logic::type} type func 
%symbol{std::vector<logic::type>} type_list
%symbol{} iff_expr implication_expr or_expr and_expr not_expr quantifier_expr

%symbol{std::vector<std::pair<std::vector<std::string>, logic::type>>} idents_type_list 
%symbol{std::string} IDENTIFIER
%symbol{std::vector<std::string>} identifier_list 
%symbol{std::pair<std::vector<std::string>, logic::type>} identifiers_colon_type
%symbol{} STRUCT DEF FRM

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT LT_EQ GT_EQ ASSIGN
%symbol{} AND OR IMPLY NOT IFF
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} FORALL EXISTS

%symbol{ std::string } SCANERROR

%symbol{} apply_args apply_expr member_apply_expr

%symbol{} iff_expr_q implication_expr_q or_expr_q and_expr_q not_expr_q

%symbolcode_h { #include "location.h" }
%symbolcode_h { #include <vector> }
%symbolcode_h { #include <string> }
%symbolcode_h { #include <stack> }
%symbolcode_h { #include "./logic/type.h" }
%symbolcode_h { #include "./logic/selector.h" }
%symbolcode_h { #include "./identifier.h" }
%symbolcode_h { #include <typeinfo> }
%symbolcode_h { #include "./logic/belief.h"}
%symbolcode_h { #include "./logic/beliefstate.h"}

%symbolspace parsing
%parserspace parsing

%parsercode_h { #include "tokenizer.h" }
%parsercode_h { #include "evaluator.h" }

%infotype {location}

%parameter {tokenizer}              tok
%parameter {evaluator}              eval

%source { tok.read(); }

%rules 
//-------------------------common--------------------------------

Session => 
	     | Session Statement SEMICOLON
         | Session _recover_ SEMICOLON
         ;

Statement => struct_specifier : strct { eval.add_belief(strct); }
           | def_specifier
           ;

idents_type_list => identifiers_colon_type:ict { return {ict}; }
		      	  | identifiers_colon_type:ict COMMA idents_type_list:v 
				      { v.push_back(ict); return v; }
			      ;

identifiers_colon_type => identifier_list:v COLON type:t {return {v, t};};

identifier_list => IDENTIFIER:s            { return {s}; }
  | IDENTIFIER:s COMMA identifier_list:v   { v.push_back(s); return v; }
  ;

type => IDENTIFIER:s 
      {
			return logic::type( logic::type_unchecked, identifier() + s );
	  }
      | func:t { return t; }
	  ;


func => type:t LPAR type_list:v RPAR {
			return logic::type (logic::type_func, t, v.begin(), v.end());
		}; 

type_list => type:t {return {t};}
           | type_list:v COMMA type:t {v.push_back(t); return v;};

//-----------------------structs---------------------------------

struct_specifier => STRUCT IDENTIFIER:s ASSIGN idents_type_list:v  
{
	std::cout << "STRUCT!\n";

    logic::structdef strctseq;
    for (auto it = v.end(); it-- != v.begin(); ) {
    	for (auto jt = it -> first.end(); jt-- != it -> first.begin(); ) {
        	strctseq.append(identifier() + (*jt), it -> second);
       	}
    } 
    return logic::belief(logic::bel_struct, identifier() + s, strctseq);
}; 

//-----------------------defs---------------------------------

def_specifier => DEF IDENTIFIER args_seq ASSIGN term {std::cout << "Definition!\n";};		  

args_seq => args_seq:st LPAR idents_type_list:v RPAR {st.push(v); return st;}
		  | LPAR idents_type_list:v RPAR {
			  std::stack<std::vector<std::pair<std::vector<std::string>,
		      logic::type>>> st; st.push(v); return st;
		  }
		  | LPAR RPAR {
		      std::stack<std::vector<std::pair<std::vector<std::string>,
		  	  logic::type>>> st; return st;
		  };
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

or_expr => and_expr OR or_expr 
         | LBRACE term RBRACE OR and_expr
         | and_expr
         | and_expr OR quantifier_expr or_expr
         | LBRACE term RBRACE OR quantifier_expr and_expr
         ;

and_expr => not_expr AND and_expr 
          | LBRACE term RBRACE AND not_expr
          | not_expr
          | not_expr AND quantifier_expr and_expr
          | LBRACE term RBRACE AND quantifier_expr not_expr 
          ;

not_expr => NOT not_expr 
          | member_apply_expr
          | apply_expr
          | NOT quantifier_expr not_expr
          | LPAR term RPAR
          ;

quantifier_expr => LBRACKET identifiers_colon_type RBRACKET
                 | LT identifiers_colon_type GT
                 | quantifier_expr LBRACKET identifiers_colon_type RBRACKET
                 | quantifier_expr LT identifiers_colon_type GT
                 ;


apply_args => term
            | term COMMA apply_args
            ;

apply_expr => IDENTIFIER LPAR RPAR
            | IDENTIFIER LPAR apply_args RPAR 
            ; 

member_apply_expr => member_apply_expr DOT IDENTIFIER
                   | member_apply_expr DOT apply_expr
                   | IDENTIFIER
                   ;

%end

