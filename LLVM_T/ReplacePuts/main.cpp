#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class PrintfToPutsRewriter : public MatchFinder::MatchCallback {
public:
    PrintfToPutsRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
        if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("printfCall")) {
            const SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = CE->getBeginLoc();
            TheRewriter.ReplaceText(StartLoc, 6, "puts"); // Replace "printf" with "puts"
            TheRewriter.RemoveText(CE->getArg(1)->getSourceRange());
        }
    }

private:
    Rewriter &TheRewriter;
};

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, nullptr);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    Rewriter TheRewriter;
    PrintfToPutsRewriter Callback(TheRewriter);

    MatchFinder Finder;
    Finder.addMatcher(
        callExpr(callee(functionDecl(hasName("printf")))).bind("printfCall"),
        &Callback
    );

    return Tool.run(newFrontendActionFactory(&Finder).get());
}