#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring/Refactoring.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class PrintfToPutsRewriter : public MatchFinder::MatchCallback {
public:
    PrintfToPutsRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
        if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("printfCall")) {
            const Expr *Arg = CE->getArg(0);

            // Replace 'printf' with 'puts'
            TheRewriter.ReplaceText(CE->getBeginLoc(), "printf", "puts");
            TheRewriter.RemoveText(CE->getArg(1)->getSourceRange());
        }
    }

private:
    Rewriter &TheRewriter;
};

int main(int argc, const char **argv) {
    clang::Tooling::CommonOptionsParser OptionsParser(argc, argv, nullptr);
    clang::Tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    clang::Rewriter Rewriter;
    PrintfToPutsRewriter;

    MatchFinder Finder;
    Finder.addMatcher(
        callExpr(callee(functionRef("printf"))).build(),
        &Finder
    );

    return Tool.run(newFrontendActionFactory<Finder>());
}

