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
            SourceLocation StartLoc = CE->getBeginLoc();

            // Check if the SourceLocation is valid
            if (!StartLoc.isValid()) {
                llvm::errs() << "Invalid SourceLocation\n";
                return;
            }

            // Ensure that StartLoc is in the main file
            if (!SM.isInMainFile(StartLoc)) {
                llvm::errs() << "SourceLocation not in main file\n";
                return;
            }

            // Ensure that the length to replace is within valid range
            const char *Text = SM.getCharacterData(StartLoc, nullptr);
            if (!Text || strncmp(Text, "printf", 6) != 0) {
                llvm::errs() << "StartLoc does not point to 'printf'\n";
                return;
            }

            // Safely replace "printf" with "puts"
            TheRewriter.ReplaceText(StartLoc, 6, "puts");

            // Safely remove second argument if it exists
            if (CE->getNumArgs() > 1) {
                SourceRange ArgRange = CE->getArg(1)->getSourceRange();
                if (ArgRange.isValid() && SM.isInMainFile(ArgRange.getBegin())) {
                    TheRewriter.RemoveText(ArgRange);
                } else {
                    llvm::errs() << "Invalid or out-of-bounds SourceRange for second argument\n";
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
