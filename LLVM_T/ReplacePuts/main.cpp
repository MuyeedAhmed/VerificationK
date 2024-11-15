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
        SourceRange SR = IL->getSourceRange();

        // Validate the SourceLocation
        if (!StartLoc.isValid() || !SM.isInMainFile(StartLoc)) {
            llvm::errs() << "Invalid or out-of-main-file SourceLocation\n";
            return;
        }

        // Validate the SourceRange
        if (!SM.isInMainFile(SR.getBegin()) || !SM.isInMainFile(SR.getEnd())) {
            llvm::errs() << "SourceRange spans out of main file\n";
            return;
        }

        llvm::StringRef TokenText = Lexer::getSourceText(CharSourceRange::getTokenRange(SR), SM, Result.Context->getLangOpts());

        llvm::errs() << "Attempting replacement at line: "
                     << SM.getSpellingLineNumber(StartLoc)
                     << ", column: " << SM.getSpellingColumnNumber(StartLoc)
                     << ", text: " << TokenText << "\n";

        if (TokenText.empty()) {
            llvm::errs() << "Empty or invalid token text\n";
            return;
        }

        try {
            TheRewriter.ReplaceText(SR, "0");
            llvm::errs() << "Successfully replaced integer literal.\n";
        } catch (...) {
            llvm::errs() << "Replacement failed for range: " << SR.printToString(SM) << "\n";
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
