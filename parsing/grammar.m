%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{logic::term} term
%symbol{logic::belief} struct_specifier def_specifier
%symbol{std::stack<std::vector<std::pair<std::vector<std::string>, logic::type>>>} args_seq 
%symbol{logic::type} type func 
%symbol{std::vector<logic::type>} type_list
%symbol{logic::term} iff_expr implication_expr or_expr and_expr not_expr lazy_implication lazy_or lazy_and
%symbol{} quantifier_expr // TODO change this.

%symbol{std::vector<std::pair<std::vector<std::string>, logic::type>>} idents_type_list 
%symbol{std::string} IDENTIFIER
%symbol{std::vector<std::string>} identifier_list 
%symbol{std::pair<std::vector<std::string>, logic::type>} identifiers_colon_type
%symbol{} STRUCT DEF FRM

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT ASSIGN
%symbol{} AND OR IMPLY NOT IFF PROP
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} FORALL EXISTS

%symbol{ std::string } SCANERROR

%symbol{} apply_args
%symbol{logic::term} apply_expr member_apply_expr

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

%parsercode_h { #include "./logic/beliefstate.h" }
%parsercode_h { #include "tokenizer.h" }
%parsercode_h { #include "evaluator.h" }

%infotype {location}

%parameter {tokenizer}              tok
%parameter { logic::beliefstate }                  blfs

%source { tok.read(); }

%rules 

//-------------------------common--------------------------------

Session => 
	     | Session Statement SEMICOLON
         | Session _recover_ SEMICOLON
         ;

Statement => struct_specifier : strct { blfs.append(std::move(strct)); }
           | def_specifier : def { blfs.append(std::move(def)); }
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

def_specifier => DEF IDENTIFIER:s args_seq ASSIGN term:trm {
	std::cout << "Definition!\n";

	// TODO change this.
	using namespace logic;

	auto T = type( type_truthval );
	auto tp = type(type_func, T, {type(type_obj)});

	return belief(bel_def, identifier() + s, trm, tp);
};

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

term => quantifier_expr iff_expr:trm { return trm; } // TODO change this.
      | iff_expr:trm { return trm; }
      ;  

iff_expr => iff_expr:trm_left IFF quantifier_expr implication_expr:trm_right { return logic::term(logic::op_equiv, trm_left, trm_right); } // TODO change this
          | iff_expr:trm_left IFF implication_expr:trm_right { return logic::term(logic::op_equiv, trm_left, trm_right); }
          | implication_expr:trm { return trm; }
          ;

implication_expr => or_expr:trm_left IMPLY quantifier_expr implication_expr:trm_right { return logic::term(logic::op_implies, trm_left, trm_right); } // TODO change this.
                  | or_expr:trm_left IMPLY implication_expr:trm_right { return logic::term(logic::op_implies, trm_left, trm_right); }
                  | or_expr:trm { return trm; }
                  ;

or_expr => or_expr:trm_left OR quantifier_expr and_expr:trm_right { return logic::term(logic::op_or, trm_left, trm_right); } // TODO change this
         | or_expr:trm_left OR and_expr:trm_right { return logic::term(logic::op_or, trm_left, trm_right); }
         | and_expr:trm { return trm; }
         ;

and_expr => and_expr:trm_left AND quantifier_expr lazy_implication:trm_right { return logic::term(logic::op_and, trm_left, trm_right); } // TODO change this
          | and_expr:trm_left AND lazy_implication:trm_right { return logic::term(logic::op_and, trm_left, trm_right); }
          | lazy_implication:trm { return trm; }
          ;

lazy_implication => LBRACE term:trm_left RBRACE IMPLY quantifier_expr lazy_or:trm_right { return logic::term(logic::op_lazy_implies, trm_left, trm_right); } // TODO change this
                  | LBRACE term:trm_left RBRACE IMPLY lazy_or:trm_right { return logic::term(logic::op_lazy_implies, trm_left, trm_right); }
                  | lazy_or:trm { return trm; }
                  ;

lazy_or => LBRACE term:trm_left RBRACE OR quantifier_expr lazy_or:trm_right { return logic::term(logic::op_lazy_or, trm_left, trm_right); } // TODO change this
         | LBRACE term:trm_left RBRACE OR lazy_or:trm_right { return logic::term(logic::op_lazy_or, trm_left, trm_right); }
         | lazy_and:trm { return trm; }
         ;

lazy_and => LBRACE term:trm_left RBRACE AND quantifier_expr lazy_and:trm_right { return logic::term(logic::op_lazy_and, trm_left, trm_right); } // TODO change this
         | LBRACE term:trm_left RBRACE AND lazy_and:trm_right { return logic::term(logic::op_lazy_and, trm_left, trm_right); }
         | not_expr:trm { return trm; }
         ;

not_expr => NOT not_expr:trm { return logic::term( logic::op_not, trm ); }
          | PROP not_expr:trm { return logic::term( logic::op_prop, trm ); }
          | member_apply_expr:trm { return trm; }
          | apply_expr:trm { return trm; }
          | NOT quantifier_expr not_expr:trm { return trm; } // TODO change this
          | PROP quantifier_expr not_expr:trm { return trm; } // TODO change this
          | LPAR term:trm RPAR { return trm; }
          ;

// TODO change this
// Probably should change the grammar
quantifier_expr => LBRACKET identifiers_colon_type RBRACKET {}
                 | LT identifiers_colon_type GT {}
                 | quantifier_expr LBRACKET identifiers_colon_type RBRACKET {}
                 | quantifier_expr LT identifiers_colon_type GT {}
                 ;

apply_args => term COMMA apply_args {}
            | term {}
            ;

apply_expr => IDENTIFIER:s LPAR apply_args RPAR {
				// TODO Fix this
				auto ident = logic::term(logic::op_unchecked, identifier() + s.c_str());
				// return logic::term(logic::op_apply, ident);
				return logic::term(logic::op_unchecked, identifier() + s.c_str());
			}
            ; 

member_apply_expr => member_apply_expr:trm DOT IDENTIFIER { return trm; } // TODO fix this
                   | member_apply_expr:trm DOT apply_expr { return trm; } // TODO fix this
                   | IDENTIFIER:s { return logic::term(logic::op_unchecked, identifier() + s.c_str()); }
                   ;

%end
 