#include "./logic/type.h"
#include "./logic/selector.h"
#include "./logic/belief.h"
#include "./logic/beliefstate.h"
#include "./identifier.h"
#include <stack>
#include <vector>


logic::term QuantifiedTerm (std::stack<std::pair<logic::selector, std::vector<logic::vartype>>> quantifiers, logic::term term) {
    while (!quantifiers.empty()){
        auto& [sel, vars] = quantifiers.top();
        term = logic::term(sel, term, {vars.begin(), vars.end()});
        quantifiers.pop();
    }
    return term;
}