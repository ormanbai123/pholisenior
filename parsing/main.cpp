#include <iostream>

#include "tokenizer.h"
#include "parser.h"

#include "logic/beliefstate.h"
#include "evaluator.h"

int main () {

    logic::beliefstate blfs;

    parsing::tokenizer tok(lexing::filereader(&std::cin, "std::cin"));

    tok.test();

    std::unordered_map<std::string, long long unsigned int> mp;
    long long unsigned int counter = 0;

    auto prs = parsing::parser(tok, blfs, mp, counter);
    prs.maxrecovery = 1000000;
    //prs.debug = 1;
    
    std::cout << ":- ";
    auto res = prs.parse(parsing::sym_Session);
    if (res.val == parsing::sym__recover_) {
        std::cout << "Syntax error in input!\n";
        return 1;
    }


    std::cout << "\n\n" << blfs << "\n";

    return 0;
}
