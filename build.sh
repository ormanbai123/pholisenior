#!/bin/bash
maphoon2024/maphoon parsing/grammar.m parsing/ maphoon2024/ > maphoon_out	
make 
echo -e "\nRunning tests...\n" 
./parser < gramtest
