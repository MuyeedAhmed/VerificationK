#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/raw_ostream.h"
#include <fstream>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Define a command-line option category
static llvm::cl::OptionCategory MyToolCategory("ReplaceIntLiterals Tool");

class ReplaceIntLiteralsRewriter : public MatchFinder::MatchCallback {
public:
    ReplaceIntLiteralsRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
    if (const IntegerLiteral *IL = Result.Nodes.getNodeAs<IntegerLiteral>("intLiteral")) {
        const SourceManager &SM = *Result.SourceManager;
        SourceLocation StartLoc = IL->getBeginLoc();
        SourceRange SR = IL->getSourceRange();
        CharSourceRange TokenRange = CharSourceRange::getTokenRange(SR);

        llvm::errs() << "Attempting replacement at line: "
                     << SM.getSpellingLineNumber(StartLoc)
                     << ", column: " << SM.getSpellingColumnNumber(StartLoc)
                     << ", text: ";

        llvm::StringRef TokenText = Lexer::getSourceText(TokenRange, SM, Result.Context->getLangOpts());
        llvm::errs() << TokenText << "\n";

        // Debug buffer validation
        FileID FID = SM.getFileID(StartLoc);
        llvm::StringRef BufferData = SM.getBufferData(FID, /*Invalid=*/nullptr);
        if (BufferData.empty()) {
            llvm::errs() << "Empty buffer; skipping replacement.\n";
            return;
        }

        llvm::errs() << "Buffer confirmed for file.\n";

        try {
            // Avoid complex ranges; just log replacement details
            llvm::errs() << "Replacing: " << TokenText << " with 0\n";

            // Replace exactly at location by inserting and removing in steps
            TheRewriter.ReplaceText(SR, "0");

            llvm::errs() << "Successfully replaced literal.\n";
        } catch (const std::exception &e) {
            llvm::errs() << "Exception caught: " << e.what() << "\n";
        } catch (...) {
            llvm::errs() << "Unknown failure during replacement.\n";
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
    Finder.addMatcher(integerLiteral().bind("intLiteral"), &Callback);

    return Tool.run(newFrontendActionFactory(&Finder).get());
}
