clang -emit-llvm -c -g test.c -I $(brew --prefix klee)/include -o test.bc

klee --solver-backend=z3 test.bc

klee --solver-backend=z3 --write-smt2s test.bc

ktest-tool klee-last/test000001.ktest

z3 klee-last/test000001.smt2    


clang -emit-llvm -c -g test.c -I $(brew --prefix klee)/include -o test.bc ; klee --solver-backend=z3 test.bc ; ktest-tool klee-last/test000001.ktest