#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IRReader/IRReader.h>
#include <iostream>
#include <string>

void findScanfVariables(llvm::Module &M) {
    for (auto &F : M) {
        for (auto &BB : F) {
            for (auto &I : BB) {
                if (auto *Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                    if (auto *Func = Call->getCalledFunction()) {
                        if (Func->getName() == "scanf") {
                            llvm::outs() << "Found scanf call in function: " << F.getName() << "\n";
                            for (unsigned i = 1; i < Call->arg_size(); ++i) { // Skip format string
                                llvm::Value *Arg = Call->getArgOperand(i);
                                llvm::outs() << "  Argument " << i << ": ";
                                Arg->print(llvm::outs());
                                llvm::outs() << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ll>\n";
        return 1;
    }

    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;

    // Read the input LLVM IR file
    std::unique_ptr<llvm::Module> Mod = llvm::parseIRFile(argv[1], Err, Context);
    if (!Mod) {
        Err.print(argv[0], llvm::errs());
        return 1;
    }

    findScanfVariables(*Mod);

    return 0;
}
