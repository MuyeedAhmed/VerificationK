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

        // Log initial validation
        if (!StartLoc.isValid()) {
            llvm::errs() << "Invalid SourceLocation\n";
            return;
        }

        if (!SM.isInMainFile(StartLoc)) {
            llvm::errs() << "SourceLocation not in main file\n";
            return;
        }

        // Log the replacement context
        llvm::errs() << "Attempting replacement at line: "
                     << SM.getSpellingLineNumber(StartLoc)
                     << ", column: " << SM.getSpellingColumnNumber(StartLoc) << "\n";

        // Validate file content around location
        const char *Text = SM.getCharacterData(StartLoc);
        llvm::errs() << "CharacterData at StartLoc: " << llvm::StringRef(Text, 10) << "\n";

        try {
            TheRewriter.ReplaceText(StartLoc, "0");
            llvm::errs() << "Successfully replaced integer literal\n";
        } catch (const std::exception &e) {
            llvm::errs() << "Exception: " << e.what() << "\n";
        } catch (...) {
            llvm::errs() << "Unknown failure while replacing text.\n";
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
