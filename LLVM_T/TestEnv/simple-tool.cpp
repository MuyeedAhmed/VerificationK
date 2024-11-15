#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;

class SimpleASTVisitor : public RecursiveASTVisitor<SimpleASTVisitor> {
public:
    bool VisitFunctionDecl(FunctionDecl *FD) {
        llvm::outs() << "Function name: " << FD->getNameAsString() << "\n";
        return true;
    }
};

class SimpleASTConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
        SimpleASTVisitor Visitor;
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
};

class SimpleFrontendAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef) override {
        return std::make_unique<SimpleASTConsumer>();
    }
};

int main(int argc, const char **argv) {
    if (argc > 1) {
        return !runToolOnCode(std::make_unique<SimpleFrontendAction>(), argv[1]);
    }
    llvm::errs() << "Expected input source code.\n";
    return 1;
}
