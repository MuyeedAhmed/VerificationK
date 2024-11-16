#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include <iostream>

using namespace clang;
using namespace clang::tooling;

class ScanFVisitor : public RecursiveASTVisitor<ScanFVisitor> {
public:
    explicit ScanFVisitor(ASTContext *Context) : Context(Context) {}

    bool VisitCallExpr(CallExpr *Call) {
        if (const FunctionDecl *FuncDecl = Call->getDirectCallee()) {
            if (FuncDecl->getNameAsString() == "scanf") {
                llvm::errs() << "Found scanf call:\n";
                for (unsigned i = 1; i < Call->getNumArgs(); ++i) {
                    const Expr *Arg = Call->getArg(i);
                    Arg->dump();
                }
            }
        }
        return true;
    }

private:
    ASTContext *Context;
};

class ScanFConsumer : public ASTConsumer {
public:
    explicit ScanFConsumer(ASTContext *Context)
        : Visitor(Context) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    ScanFVisitor Visitor;
};

class ScanFAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        return std::make_unique<ScanFConsumer>(&CI.getASTContext());
    }
};

static llvm::cl::OptionCategory MyToolCategory("scanf detector options");

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << "Error parsing options: " 
                     << llvm::toString(ExpectedParser.takeError()) << "\n";
        return 1;
    }
    CommonOptionsParser &OptionsParser = *ExpectedParser;
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    return Tool.run(newFrontendActionFactory<ScanFAction>().get());
}
