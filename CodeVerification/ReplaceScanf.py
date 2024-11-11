import os
import re

def replace_scanf_with_klee(input_file, output_file):
    with open(input_file, 'r') as file:
        code = file.read()

    scanf_pattern = re.compile(r'scanf\(".*?"\s*,\s*(&\w+)(?:\s*,\s*&\w+)*\);')

    def replace_scanf(match):
        variables = re.findall(r'&(\w+)', match.group(0))
        klee_lines = [
            f'klee_make_symbolic(&{var}, sizeof({var}), "{var}");' for var in variables
        ]
        return '\n'.join(klee_lines)

    modified_code = scanf_pattern.sub(replace_scanf, code)
    modified_code = '#include <klee/klee.h>\n' + modified_code

    with open(output_file, 'w') as file:
        file.write(modified_code)


if __name__ == "__main__":
    dataset = 'Lab-3/2811/'
    input_directory = '../ITSP/dataset/' + dataset
    output_directory = 'ModifiedCode/' + dataset

    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    for filename in os.listdir(input_directory):
        if filename.endswith(".c"):
            input_file = os.path.join(input_directory, filename)
            output_file = os.path.join(output_directory, filename)
            replace_scanf_with_klee(input_file, output_file)
