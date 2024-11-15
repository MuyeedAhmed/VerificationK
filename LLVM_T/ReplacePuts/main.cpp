#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Define a category for command-line options
static llvm::cl::OptionCategory MyToolCategory("FooToBar Tool");

class FooToBarRewriter : public MatchFinder::MatchCallback {
public:
    FooToBarRewriter(Rewriter &R) : TheRewriter(R) {}

    void run(const MatchFinder::MatchResult &Result) override {
        if (const DeclRefExpr *DRE = Result.Nodes.getNodeAs<DeclRefExpr>("fooVar")) {
            SourceLocation StartLoc = DRE->getLocation();
            TheRewriter.ReplaceText(StartLoc, 3, "bar"); // Replace "foo" with "bar"
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
    FooToBarRewriter Callback(TheRewriter);

    MatchFinder Finder;
    Finder.addMatcher(
        declRefExpr(to(varDecl(hasName("foo")))).bind("fooVar"),
        &Callback
    );

    return Tool.run(newFrontendActionFactory(&Finder).get());
}
