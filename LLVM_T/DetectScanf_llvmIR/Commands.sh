
# Get llvm file
clang -S -emit-llvm example.c -o example.ll
// clang -S -emit-llvm TestSet/Test.c -o TestSet/test.ll

# Generate llvm_scanf_tool file to detect scanf
clang++ -o llvm_scanf_tool llvm_scanf_tool.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core irreader`

# Run llvm_scanf_tool on a .ll
./llvm_scanf_tool TestSet/test.ll 

# Run llvm-cbe to get .ll to .c
/Users/muyeedahmed/Desktop/Gitcode/ForkedLibrary/llvm-cbe/tools/llvm-cbe/llvm-cbe example.ll
