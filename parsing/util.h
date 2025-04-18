#ifndef PARSING_UTIL_H
#define PARSING_UTIL_H

#include "./logic/type.h"
#include "./logic/selector.h"
#include "./logic/belief.h"
#include "./logic/beliefstate.h"
#include "./identifier.h"
#include <stack>
#include <vector>


logic::term QuantifiedTerm (std::stack<std::pair<logic::selector, std::vector<logic::vartype>>> quantifiers, logic::term term);

#endif