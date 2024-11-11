import os
import subprocess

def compile_with_clang(directory):
    for filename in os.listdir(directory):
        if filename.endswith("_correct.c"):
            continue

        base_name = filename.replace(".c", "")
        input_file = os.path.join(directory, filename)
        output_file = os.path.join(directory, f"{base_name}.bc")
        clang_command = [
            "clang",
            "-emit-llvm",
            "-c",
            "-g",
            input_file,
            "-I", "$(brew --prefix klee)/include",
            "-o", output_file
        ]

        result = subprocess.run(" ".join(clang_command), shell=True, capture_output=True, text=True)

def run_klee(directory):
    for filename in os.listdir(directory):
        if filename.endswith(".bc"):
            if filename != "Main.bc":
                continue
            input_file = os.path.join(directory, filename)

            klee_command = [
                "klee",
                "--solver-backend=z3",
                "--write-smt2s",
                input_file
            ]
            result = subprocess.run(" ".join(klee_command), shell=True, capture_output=True, text=True)

            klee_out_dirs = sorted(
                [d for d in os.listdir(directory) if d.startswith("klee-out-")],
                key=lambda x: os.path.getmtime(os.path.join(directory, x)),
                reverse=True
            )

            if klee_out_dirs:
                latest_klee_dir = klee_out_dirs[0]
                new_name = f"{filename.replace('.bc', '')}_{latest_klee_dir}"
                os.rename(os.path.join(directory, latest_klee_dir), os.path.join(directory, new_name))


if __name__ == "__main__":
    output_directory = "ModifiedCode/Lab-3/2811/"
    # compile_with_clang(output_directory)
    run_klee(output_directory)

