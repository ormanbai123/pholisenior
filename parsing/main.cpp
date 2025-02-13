#include <iostream>

#include "tokenizer.h"
#include "parser.h"

int main () {

    parsing::tokenizer tok(lexing::filereader(&std::cin, "std::cin"));

    tok.test();

    auto prs = parsing::parser(tok);
    prs.maxrecovery = 1000000;
    // prs.debug = 1;
    
    std::cout << ":- ";
    auto res = prs.parse(parsing::sym_Session);
    if (res.val == parsing::sym__recover_) {
        std::cout << "Syntax error in input!\n";
        return 1;
    }

    return 0;
}