# Build and Run
* Run make. this produces the compiler executable 'adc'.
* Run ./run.sh decaf_source.dcf
* This calls adc on the source file, and then runs clang on it to produce an a.out executable file.

# Structure 

* driver.cc: Contains the driver, which runs the semantic check first and then generates the code.
* AstNode.{h,cc}: Contains the basic AstNode superclass from which the others are derived, along with some helpers.
* ExprNode.{h,cc}: Mostly contains the classes for nodes related to expr, along with some related enums and helpers associated with those enums.
* RootNode.{h,cc}: Contains the classes for nodes at the root level: the rootnode and method nodes, and also fields.
* TypeCheckVisitor.{h,cc}: contains a visitor class that performs a full semantic check for all the rules in the spec.
* CodeGenVisitor.{h,cc}}: contains a visitor class that generates the llvm IR.
* AstVisitor.{h,cc}: contains the AstVisitor superclass.
* BlockNode.{h,cc}: contains the class for nodes that are related to block.
* lexer.l: contains the flex source code for the lexer.
* parser.y: contains the bison source code for the parser.

# Description 
The driver calls typecheck first, and then the codegen. The generated code is output to stdout. Callout function overloading is implemented. All required runtime and semantic errors are handled.
