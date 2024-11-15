#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Define a category for command-line options
static llvm::cl::OptionCategory MyToolCategory("PrintfToPuts Tool");

class PrintfToPutsRewriter : public MatchFinder::MatchCallback {
public:
    PrintfToPutsRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
        if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("printfCall")) {
            const SourceManager &SM = *Result.SourceManager;
            
            // Check if the location is valid
            SourceLocation StartLoc = CE->getBeginLoc();
            if (!StartLoc.isValid() || !SM.isInMainFile(StartLoc)) {
                llvm::errs() << "Invalid SourceLocation. Skipping replacement.\n";
                return;
            }

            // Check if the replacement length (6 for "printf") is valid
            if (SM.getFileID(StartLoc) != SM.getMainFileID()) {
                llvm::errs() << "Replacement out of main file bounds. Skipping replacement.\n";
                return;
            }

            // Perform the replacement from "printf" to "puts"
            TheRewriter.ReplaceText(StartLoc, 6, "puts");

            // Ensure there are extra arguments to remove
            if (CE->getNumArgs() > 1) {
                SourceRange ArgRange = CE->getArg(1)->getSourceRange();
                if (ArgRange.isValid()) {
                    TheRewriter.RemoveText(ArgRange);
                }
            }
        }
    }

private:
    Rewriter &TheRewriter;
};

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
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
