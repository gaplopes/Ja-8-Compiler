lex jac.l
yacc -d -v jac.y
cc -g -Wall -o jac y.tab.c ast.c semantic.c llvm.c lex.yy.c
# cc -g -Wall -o jac y.tab.c ast.c semantic.c llvm.c lex.yy.c -ll -ly

./jac -t < Echo1.ja

./jac < Echo1.ja > out.ll

llc-3.8 out.ll
clang-3.8 -o out out.s
./out
