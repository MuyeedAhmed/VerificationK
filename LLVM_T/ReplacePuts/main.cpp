#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Define a category for command-line options
static llvm::cl::OptionCategory MyToolCategory("ReplaceIntLiterals Tool");

class ReplaceIntLiteralsRewriter : public MatchFinder::MatchCallback {
public:
    ReplaceIntLiteralsRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
    if (const IntegerLiteral *IL = Result.Nodes.getNodeAs<IntegerLiteral>("intLiteral")) {
        const SourceManager &SM = *Result.SourceManager;
        SourceLocation StartLoc = IL->getBeginLoc();
        CharSourceRange TokenRange = CharSourceRange::getTokenRange(IL->getSourceRange());

        if (!StartLoc.isValid() || !SM.isInMainFile(StartLoc)) {
            llvm::errs() << "Invalid or external SourceLocation\n";
            return;
        }

        llvm::StringRef TokenText = Lexer::getSourceText(TokenRange, SM, Result.Context->getLangOpts());

        if (TokenText.empty()) {
            llvm::errs() << "Empty token: Invalid Range\n";
            return;
        }

        // Validate again:
        llvm::errs() << "Valid: Replace line: "
                     << SM.getSpellingLineNumber(StartLoc) << " ->" << TokenText << "\n";

        // Try simple insert/remove by exact range offsets:
        try {
            TheRewriter.RemoveText(TokenRange);  // Single token literal only.
            TheRewriter.InsertText(StartLoc, "0");
            llvm::errs() << "Rewritten successfully to `0`!\n";
        } catch (...) {
            llvm::errs() << "Failed to edit Text buffer at \n";
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
    ReplaceIntLiteralsRewriter Callback(TheRewriter);

    MatchFinder Finder;
    Finder.addMatcher(
        integerLiteral().bind("intLiteral"),
        &Callback
    );

    return Tool.run(newFrontendActionFactory(&Finder).get());
}
