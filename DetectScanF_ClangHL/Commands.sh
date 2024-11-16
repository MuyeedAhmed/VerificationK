clang++ -std=c++17 -o DetectScanf DetectScanf.cpp \
  -I/usr/lib/llvm-16/include -L/usr/lib/llvm-16/lib \
  -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse -lclangSema \
  -lclangAnalysis -lclangRewriteFrontend -lclangRewrite -lclangEdit \
  -lclangAST -lclangASTMatchers -lclangLex -lclangBasic -lLLVM \
  -lclangSupport -lclangARCMigrate -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCore -lclangStaticAnalyzerCheckers