%startsymbol Session EOF

%symbol Session
%symbol{} Statement Expr

%symbol{logic::term} term
%symbol{logic::belief} struct_specifier def_specifier
%symbol{std::stack<std::vector<logic::vartype>>} args_seq 
%symbol{logic::type} type func 
%symbol{std::vector<logic::type>} type_list
%symbol{logic::term} iff_expr implication_expr or_expr and_expr not_expr lazy_implication lazy_or lazy_and

%symbol{std::vector<std::pair<std::vector<std::string>, logic::type>>} idents_type_list 
%symbol{std::string} IDENTIFIER
%symbol{std::vector<std::string>} identifier_list 
%symbol{std::pair<std::vector<std::string>, logic::type>} identifiers_colon_type
%symbol{} STRUCT DEF FRM

%symbol{std::vector<logic::vartype>} vartypes
%symbol{std::pair<logic::selector, std::vector<logic::vartype>>} exist forall 
%symbol{std::stack<std::pair<logic::selector, std::vector<logic::vartype>>>}quantifiers

%symbol{} EOF FILEBAD WHITESPACE COMMENT EMPTY
%symbol{} LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET 
%symbol{} EQ LT GT ASSIGN
%symbol{} AND OR IMPLY NOT IFF PROP
%symbol{} COLON SEMICOLON COMMA DOT 

%symbol{} FORALL EXISTS

%symbol{ std::string } SCANERROR

%symbol{std::vector<logic::term>} apply_args
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
%symbolcode_h { #include "./parsing/util.h"}

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

vartypes => identifiers_colon_type:ict 
          {
            std::vector<logic::vartype> vars;
            for (auto var : ict.first) {
              vars.emplace_back(var, ict.second);
            }
            return vars;
          }
          | vartypes:vars COMMA identifiers_colon_type:ict
          {
            for (auto var : ict.first) {
              vars.emplace_back(var, ict.second);
            }
            return vars;
          }
;

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

def_specifier => DEF IDENTIFIER:s args_seq:as ASSIGN term:trm {
	std::cout << "Definition!\n";

	// TODO change this.
	using namespace logic;

	auto tp = type( type_truthval );
	auto body = trm;

	while (!as.empty()) {
		auto vars = as.top();
		body = logic::term(op_lambda, body, vars.begin(), vars.end());

		std::vector<logic::type> vars_type;
		for (auto& v: vars) {
			vars_type.push_back(v.tp);
		}

		tp = type(type_func, tp, vars_type.begin(), vars_type.end());
		as.pop();
	}

	return belief(bel_def, identifier() + s, body, tp);
};

args_seq => LPAR vartypes:vars RPAR args_seq:st {st.push(vars); return st;}
		  | LPAR vartypes:vars RPAR {
			  std::stack<std::vector<logic::vartype>> st; st.push(vars); return st;
		  }
		  | LPAR RPAR {
		      std::stack<std::vector<logic::vartype>> st; return st;
		  };

//-----------------------terms---------------------------------

term => quantifiers:qntf iff_expr: trm { return QuantifiedTerm(qntf, trm); }
		  | iff_expr:trm { return trm; }
      ;  

iff_expr => iff_expr:trm_left IFF implication_expr:trm_right { return logic::term(logic::op_equiv, trm_left, trm_right); }
          | iff_expr:trm_left IFF quantifiers:qntf implication_expr:trm_right {
				return logic::term(logic::op_equiv, trm_left, QuantifiedTerm(qntf, trm_right));
			}
          | implication_expr:trm { return trm; }
          ;

implication_expr => or_expr:trm_left IMPLY quantifiers:qntf implication_expr:trm_right {
	return logic::term(logic::op_implies, trm_left, QuantifiedTerm(qntf, trm_right));
}
					        | or_expr:trm_left IMPLY implication_expr:trm_right { return logic::term(logic::op_implies, trm_left, trm_right); }
                  | or_expr:trm { return trm; }
                  ;

or_expr => or_expr:trm_left OR quantifiers:qntf and_expr:trm_right {
	return logic::term(logic::op_or, trm_left, QuantifiedTerm(qntf, trm_right));
}
		     | or_expr:trm_left OR and_expr:trm_right { return logic::term(logic::op_or, trm_left, trm_right); }
         | and_expr:trm { return trm; }
         ;

and_expr => and_expr:trm_left AND quantifiers:qntf lazy_implication:trm_right {
	return logic::term(logic::op_and, trm_left, QuantifiedTerm(qntf, trm_right));
}
		      | and_expr:trm_left AND lazy_implication:trm_right { return logic::term(logic::op_and, trm_left, trm_right); }
          | lazy_implication:trm { return trm; }
          ;

lazy_implication => forall:qntf LBRACE term:trm_left RBRACE IMPLY lazy_or:trm_right {
	return QuantifiedTerm(qntf, logic::term(logic::op_lazy_implies, trm_left, trm_right));
}
                  | lazy_or:trm { return trm; }
                  ;

lazy_or => exist:qntf LBRACE term:trm_left RBRACE OR lazy_or:trm_right {
	return QuantifiedTerm(qntf, logic::term(logic::op_lazy_or, trm_left, trm_right));
}
         | lazy_and:trm { return trm; }
         ;

lazy_and => exist:qntf LBRACE term:trm_left RBRACE AND lazy_and:trm_right {
	return QuantifiedTerm(qntf, logic::term(logic::op_lazy_and, trm_left, trm_right));
}
         | not_expr:trm { return trm; }
         ;

not_expr => NOT not_expr:trm { return logic::term( logic::op_not, trm ); }
          | PROP not_expr:trm { return logic::term( logic::op_prop, trm ); }
		      | NOT quantifiers:qntf not_expr:trm {
				return QuantifiedTerm(qntf, trm);
				}
		      | PROP quantifiers:qntf not_expr:trm { return QuantifiedTerm(qntf, trm); }
          | member_apply_expr:trm { return trm; }
          | apply_expr:trm { return trm; }
          | LPAR term:trm RPAR { return trm; }
          ;

apply_args => term:t COMMA apply_args:v { v.push_back(t); return v; }
            | term:t { return {t}; }
            ;

apply_expr => IDENTIFIER:s LPAR apply_args:v RPAR {
				auto f = logic::term(logic::op_unchecked, identifier() + s.c_str());
				return logic::term(logic::op_apply, f, v.begin(), v.end());
			}
            ; 

member_apply_expr => member_apply_expr:trm DOT IDENTIFIER { return trm; } // TODO fix this
                   | member_apply_expr:trm DOT apply_expr { return trm; } // TODO fix this
                   | IDENTIFIER:s { return logic::term(logic::op_unchecked, identifier() + s.c_str()); }
                   ;

quantifiers => forall:q 
             {
                std::stack<std::pair<logic::selector, std::vector<logic::vartype>>> st;
                st.push(q);
                return st;
             }
             | quantifiers:qst forall:q {qst.push(q); return qst;}
             | exist:q 
             {
                std::stack<std::pair<logic::selector, std::vector<logic::vartype>>> st;
                st.push(q);
                return st;
             }
             | quantifiers:qst exist:q {qst.push(q); return qst;}
             ;

forall => LBRACKET vartypes:vars RBRACKET {return {logic::selector::op_forall, vars};}
         ;

exist => LT vartypes:vars GT {return {logic::selector::op_exists, vars};}
       ;

%end