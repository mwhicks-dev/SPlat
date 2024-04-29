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

def process_individual_file(file_path: str) -> list[str]:
    output = []
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
            value = strip_file_peripheral(line)
            output.append(strip_file_peripheral(line))
    return output

def process_all_files() -> dict[str, str]:
    files = get_file_paths('include') + get_file_paths('include/events') + get_file_paths('include/model') #+ get_file_paths('src')
    dependencies = {}
    for f in files:
        key = strip_file_peripheral(f)
        if key not in dependencies.keys():
            dependencies[key] = []
        file_dependencies = process_individual_file(f)
        for value in file_dependencies:
            if value not in dependencies[key]:
                dependencies[key].append(value)
    return dependencies

def get_src_units() -> list[str]:
    files = get_file_paths('src')
    files = [strip_file_peripheral(f) for f in files]
    return files

def main() -> None:
    dependencies = process_all_files()
    not_compiled = get_src_units()
    src_units_order = []
    while len(not_compiled) > 0:
        src_units_order.append([])
        for target in not_compiled:
            can_compile = True
            for value in dependencies[target]:
                if value != target and value in not_compiled:
                    #print(f'{target} needs unit {value} not compiled')
                    can_compile = False
                    break
            if can_compile:
                src_units_order[-1].append(target)
        for target in src_units_order[-1]:
            not_compiled.pop(not_compiled.index(target))
    layer_number = 65
    for units_layer in src_units_order:
        print(f'set(L{chr(layer_number)}', end='')
        for unit in units_layer:
            print(f' src/{unit}.cpp', end='')
        print(')')
        layer_number += 1
    print('set(ALL_IMPLEMENTATIONS', end='')
    for i in range(65, layer_number):
        print(f' ${{L{chr(i)}}}', end='')
    print(')')

if __name__ == '__main__':
    main()