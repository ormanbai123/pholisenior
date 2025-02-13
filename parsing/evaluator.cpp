#include "evaluator.h"

void evaluator::add_belief(logic::belief& belif) {
    blfs.append(std::move(belif));
}