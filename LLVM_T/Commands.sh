
# Get llvm file
clang -S -emit-llvm example.c -o example.ll


# Run llvm-cbe
/Users/muyeedahmed/Desktop/Gitcode/ForkedLibrary/llvm-cbe/tools/llvm-cbe/llvm-cbe example.ll
