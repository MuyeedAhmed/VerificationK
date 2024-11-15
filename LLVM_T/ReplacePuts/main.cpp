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
            CharSourceRange TokenRange = CharSourceRange::getTokenRange(IL->getSourceRange());

            llvm::errs() << "Attempting replacement at line: "
                         << SM.getSpellingLineNumber(StartLoc)
                         << ", column: " << SM.getSpellingColumnNumber(StartLoc)
                         << ", text: ";

            llvm::StringRef TokenText = Lexer::getSourceText(TokenRange, SM, Result.Context->getLangOpts());
            llvm::errs() << TokenText << "\n";

            // Debugging: Dump buffer contents to validate correctness
            llvm::errs() << "Dumping buffer:\n";
            FileID FID = SM.getFileID(StartLoc);
            llvm::StringRef BufferData = SM.getBufferData(FID, /*Invalid=*/nullptr);
            if (!BufferData.empty()) {
                llvm::errs() << "Buffer data: " << BufferData.substr(0, 100) << "...\n"; // Limit output for large files
            } else {
                llvm::errs() << "Failed to retrieve buffer data or buffer is empty\n";
                return;
            }

            // Ensure valid token and rewrite
            if (TokenText.empty()) {
                llvm::errs() << "Empty token text; skipping rewrite.\n";
                return;
            }

            try {
                // Safely replace the integer literal with "0"
                TheRewriter.ReplaceText(TokenRange, "0");
                llvm::errs() << "Successfully replaced integer literal.\n";
            } catch (const std::exception &e) {
                llvm::errs() << "Exception caught during ReplaceText: " << e.what() << "\n";
            } catch (...) {
                llvm::errs() << "Unknown error during ReplaceText.\n";
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
