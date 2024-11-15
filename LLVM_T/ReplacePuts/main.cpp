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

        if (!StartLoc.isValid()) {
            llvm::errs() << "Invalid SourceLocation\n";
            return;
        }

        if (!SM.isInMainFile(StartLoc)) {
            llvm::errs() << "SourceLocation not in main file\n";
            return;
        }

        // Ensure text is accessible and safe
        llvm::StringRef Text = SM.getBufferData(SM.getFileID(StartLoc));
        unsigned Offset = SM.getFileOffset(StartLoc);
        llvm::StringRef TokenText = Text.substr(Offset, 1);

        // Confirm the token at this SourceLocation is expected
        if (TokenText != "4" && TokenText != "1" && TokenText != "0") {
            llvm::errs() << "Unexpected token: " << TokenText << "\n";
            return;
        }

        llvm::errs() << "Processing line: " << SM.getSpellingLineNumber(StartLoc)
                     << ", column: " << SM.getSpellingColumnNumber(StartLoc) << "\n";

        try {
            TheRewriter.ReplaceText(StartLoc, "0");
            llvm::errs() << "Successfully replaced\n";
        } catch (...) {
            llvm::errs() << "Failed at " << StartLoc.printToString(SM) << "\n";
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
