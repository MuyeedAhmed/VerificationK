#!/bin/bash

SOURCE_DIR="$1"
OUTPUT_DIR="$1/klee_outputs"

mkdir -p "$OUTPUT_DIR"

for c_file in "$SOURCE_DIR"/*.c; do
    base_name=$(basename "$c_file" .c)
    echo $base_name
    file_output_dir="$OUTPUT_DIR/$base_name"
    
    rm -rf "$file_output_dir"
    mkdir -p "$file_output_dir"
    
    cp "$c_file" "$file_output_dir/"

    clang -emit-llvm -c -g "$file_output_dir/$base_name.c" -I $(brew --prefix klee)/include -o "$file_output_dir/$base_name.bc"
    klee --solver-backend=z3 "$file_output_dir/$base_name.bc"

done


# clang++ -std=c++17 -o DetectScanf DetectScanf.cpp \
#   -I/usr/lib/llvm-16/include -L/usr/lib/llvm-16/lib \
#   -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver \
#   -lclangSerialization -lclangCodeGen -lclangParse -lclangSema \
#   -lclangAnalysis -lclangRewriteFrontend -lclangRewrite -lclangEdit \
#   -lclangAST -lclangASTMatchers -lclangLex -lclangBasic -lLLVM \
#   -lclangSupport -lclangARCMigrate -lclangStaticAnalyzerFrontend \
#   -lclangStaticAnalyzerCore -lclangStaticAnalyzerCheckers