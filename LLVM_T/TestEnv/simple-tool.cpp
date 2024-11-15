#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;

class SimpleVisitor : public RecursiveASTVisitor<SimpleVisitor> {
public:
    bool VisitFunctionDecl(FunctionDecl *FD) {
        llvm::outs() << "Found function: " << FD->getNameInfo().getName().getAsString() << "\n";
        return true;
    }
};

class SimpleConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
        SimpleVisitor Visitor;
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
};

class SimpleFrontendAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &, StringRef) override {
        return std::make_unique<SimpleConsumer>();
    }
};

int main(int argc, const char **argv) {
    if (argc > 1) {
        return !runToolOnCode(std::make_unique<SimpleFrontendAction>(), argv[1]);
    }
    llvm::errs() << "Usage: ./basic-tool '<code>'\n";
    return 1;
}
