#!/usr/bin/python
import os

def get_file_paths(dir_path: str) -> list[str]:
    output = []
    files = os.listdir(f"{os.environ['PWD']}/{dir_path}")
    files = [f'./{dir_path}/{f}' for f in files]
    for f in files:
        if f.endswith('.h') or f.endswith('.cpp'):
            output.append(f)
    return output

def strip_file_peripheral(identifier: str) -> str:
    identifier = identifier.split('/')
    while len(identifier) > 1:
        identifier.pop(0)
    identifier = identifier[0]
    if identifier.endswith('.h'):
        identifier = identifier[:-2]
    if identifier.endswith('.cpp'):
        identifier = identifier[:-4]
    return identifier

def process_individual_file(file_path: str) -> None:
    with open(file_path, "r") as fp:
        for line in fp.readlines():
            key_string = '#include'
            if not line.startswith(key_string):
                continue
            line = line[len(key_string):-1]
            line = line.lstrip().rstrip()
            if not (line.startswith("\"") and line.endswith("\"")):
                continue
            line = line[1:-1]
            print(f'{strip_file_peripheral(file_path)} {strip_file_peripheral(line)}')

def main() -> None:
    files = get_file_paths('include') + get_file_paths('include/events') + get_file_paths('include/model') + get_file_paths('src')
    for f in files:
        process_individual_file(f)

if __name__ == '__main__':
    main()