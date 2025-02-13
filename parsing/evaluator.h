#ifndef __EVALUATOR__H
#define __EVALUATOR__H

#include "logic/belief.h"
#include "logic/beliefstate.h"

struct evaluator {
    evaluator(logic::beliefstate& _blfs)
        :blfs(_blfs) 
    {}
    
    logic::beliefstate& blfs;
    void add_belief (logic::belief& belif);
};

#endif