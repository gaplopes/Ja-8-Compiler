# Ja-8-Compiler

This is a compiler for a subset of the Java (version 8) programming language.

The project is a part of the course "Compilers" at the University of Coimbra for the 2016/2017 academic year for the Informatics Engineering degree.

_Note:_ The project statement is in Portuguese and can be found in the pdf file "Project Statement.pdf".

The compiler construction is divided into 4 phases:

- Phase 1: Lexical Analysis (using Lex)
- Phase 2: Syntax Analysis (using Yacc)
- Phase 3: Semantic Analysis (using C)
- Phase 4: Code Generation (using C)

The phases are arranged in folders, each containing the source code for the phase, the input files and the output files.

The following description is a brief description of the phases and the files in each folder.

## - Phase 1: Lexical Analysis

The lexical analysis is done using the Lex tool. The Lex tool is a program that generates a lexical analyzer (scanner) from a description of the tokens in the language. The lexical analyzer is a program that reads the input character by character and returns the next token in the input. The lexical analyzer is implemented in the file `jac.l`.

## - Phase 2: Syntax Analysis

The syntax analysis is done using the Yacc tool. The Yacc tool is a program that generates a parser from a description of the grammar of the language. The parser is a program that reads the input token by token and returns the next syntactic unit in the input. The parser is implemented in the file `jac.y`.

To print the syntax tree, the functions are implemented in the file `ast.c` and the data structures are implemented in the file `ast.h`.

## - Phase 3: Semantic Analysis

The semantic analysis is done using C. The semantic analysis is implemented in the file `semantic.c` and the data structures are implemented in the file `semantic.h`.

## - Phase 4: Code Generation

The code generation is done using C. The code generation is implemented in the file `llvm.c` and the data structures are implemented in the file `header.h`.

Note that this phase has the code for all the phases, since it is the last phase.

--------------------

## Compile, run and verify memory leaks

To test and run the project go to the Phase 4 folder and run the following commands:

```bash
sh script.sh
```

_Note:_ To use the compiler for the different phases, change the flag in the script file.

- For Phase 1, change the flag to `./jac -l < Echo1.ja`.

- For Phase 2, change the flag to `./jac -t < Echo1.ja`.

- For Phase 3, change the flag to `./jac -s < Echo1.ja`.

- For Phase 4, change the flag to `./jac < Echo1.ja`.

To verify memory leaks and other errors, run the following command:

```bash
  valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./jac < input_file.Ja
```

--------------------

## Authors

- Gonçalo Lopes [galopes@dei.uc.pt]
