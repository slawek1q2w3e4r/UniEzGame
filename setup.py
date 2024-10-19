from setuptools import setup, Extension
import os
import pybind11
import shutil

# Ścieżki do SDL2
SDL2_INCLUDE_DIR = r'D:\dev\SDL2\include'
SDL2_LIB_DIR = r'D:\dev\SDL2\lib\x64'
SDL2_DLL = r'D:\dev\SDL2\lib\x64\SDL2.dll'
UNIEZGAME_DLL = r'C:\Users\huawe\OneDrive\Pulpit\modól\uniezgame.dll'  # Adjust this path accordingly

module_name = 'uniezgame'

# Tworzenie rozszerzenia
unigame_module = Extension(
    module_name,
    sources=['mylib.cpp'],
    include_dirs=[SDL2_INCLUDE_DIR, pybind11.get_include()],
    library_dirs=[SDL2_LIB_DIR],
    libraries=['SDL2', 'SDL2main'],
)

setup(
    name=module_name,
    version='0.2',
    description='Uniezgame - Python C++ bindings using pybind11',
    ext_modules=[unigame_module],
    include_package_data=True,
    package_data={module_name: ['SDL2.dll', 'uniezgame.dll']},  # Include both DLLs
    zip_safe=False,
    author='Michał Lewandowski',
    author_email='michal.lewandowski.113@gamil.com',
    url='https://github.com/twoje_github/unigame',
    license='MIT',
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)

# Kopiowanie plików DLL (jeśli to potrzebne)
build_dir = os.path.join(os.path.dirname(__file__), 'build', 'lib', module_name)

# Make sure build directory exists
os.makedirs(build_dir, exist_ok=True)

# Copy SDL2.dll
if os.path.exists(SDL2_DLL):
    shutil.copy(SDL2_DLL, os.path.join(build_dir, 'SDL2.dll'))
    print(f"SDL2.dll skopiowany do: {build_dir}")
else:
    print(f"Plik SDL2.dll nie istnieje w ścieżce: {SDL2_DLL}")

# Copy uniezgame.dll
if os.path.exists(UNIEZGAME_DLL):
    shutil.copy(UNIEZGAME_DLL, os.path.join(build_dir, 'uniezgame.dll'))
    print(f"uniezgame.dll skopiowany do: {build_dir}")
else:
    print(f"Plik uniezgame.dll nie istnieje w ścieżce: {UNIEZGAME_DLL}")
