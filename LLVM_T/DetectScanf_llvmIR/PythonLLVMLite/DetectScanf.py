from llvmlite import binding as llvm
import os

def find_scanf_variables(file_path):
    llvm.initialize()
    llvm.initialize_native_target()
    llvm.initialize_native_asmprinter()
    
    # Load the LLVM IR file
    with open(file_path, 'r') as f:
        llvm_ir = f.read()
    
    # Parse the module
    mod = llvm.parse_assembly(llvm_ir)
    
    variables = []

    # Iterate over functions in the module
    for func in mod.functions:
        for block in func.blocks:
            for instr in block.instructions:
                # Check for 'call' instructions that call 'scanf'
                if instr.opcode == 'call' and '@scanf' in str(instr):
                    args = instr.operands[1:]  # Skip format string (first operand)
                    for arg in args:
                        if "ptr" in str(arg.type):
                            # Analyze the instruction that defines this pointer
                            defining_instr = find_defining_instruction(arg)
                            if defining_instr and "getelementptr" in str(defining_instr):
                                variables.append((str(arg), "Array"))
                            else:
                                variables.append((str(arg), "Scalar (by reference)"))
                        else:
                            variables.append((str(arg), "Scalar"))
    
    return variables

def find_defining_instruction(arg):
    """Traces back the defining instruction of a given argument."""
    defining_instr = getattr(arg, 'parent', None)
    while defining_instr:
        if defining_instr.opcode in ['alloca', 'getelementptr']:
            return defining_instr
        defining_instr = getattr(defining_instr, 'parent', None)
    return None

def main(directory):
    scanf_variables = {}

    for filename in os.listdir(directory):
        if filename.endswith(".ll"):
            file_path = os.path.join(directory, filename)
            variables = find_scanf_variables(file_path)
            if variables:
                scanf_variables[filename] = variables
    
    # Print results
    for file, vars in scanf_variables.items():
        print(f"File: {file}")
        for var, var_type in vars:
            print(f"Variable: {var}, Type: {var_type}")
        print("-" * 40)

# Specify the directory containing the .ll files
llvm_directory = "TestSet/"

main(llvm_directory)
