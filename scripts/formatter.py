import os
import pathlib

current_dir = os.getcwd()
source_dir_name = "src"

file_types_to_format = [".cpp", ".hpp"]

exclude_directories = [
    "build",
    "venv",
    "src/stm32",
    "src/CMakeFiles",
    "src/FreeRTOS",
    "src/wavetables",
    "src/CMakeFiles"
]

exclude_files = [
    "src/FreeRTOSConfig.h",
]

cwd = pathlib.Path(current_dir)

excluded_file_paths = list()
for file in exclude_files:
    file_path_str = f"{cwd.as_posix()}/{file}"
    excluded_file_paths.append(file_path_str)

excluded_directory_paths = list()
for dir in exclude_directories:
    dir_path_str = f"{cwd.as_posix()}/{dir}"
    excluded_directory_paths.append(dir_path_str)

def format(path: pathlib.PosixPath):
    for item in list(path.iterdir()):
        if item.is_dir():
            if item.as_posix() in excluded_directory_paths:
                print(f"Skipping dir:\t{item.as_posix()}")
            else:
                format(item)
        elif item.is_file():
            if item.as_posix() in excluded_file_paths:
                print(f"Skipping file:\t{item.as_posix()}")
            else:
                if item.suffix in file_types_to_format:
                    print(f"Formating:\t{item}")
                    os.system(f"clang-format -i {item.as_posix()}")

format(cwd)
