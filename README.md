# CMake examples with for Amiga 

These are examples for creating Amiga Projects using AmigaOS compiler from bebbo: https://github.com/bebbo/amiga-gcc

In order to use this you have to put the tools directory into your path and execute

`. amiga-env`

The examples should run on any kickstartfrom 1.x up.

For using this package, you need either a Linux installation or under Windows using MSYS2 MSYS (not MingW).

### Installation

The default path for the toolchain is expected to be `/opt/amiga/`. If you have setup your toolchain in a different path, you may take a look into the tools directory and adjust the paths in `amiga-env` accordingly.

```
git clone https://github.com/skeetor/amiga-utils
export PATH=<MYPATH>/amiga-utils/CMake/tools:$PATH
. amiga-env
cd amiga-utils/utilslib
cmake . -B build-release
cd build-release
make && make install
```

### Libraries

There will two libraries installed with utillity functions, usefull for common Amiga projects. Refer to the respective subdirectores/include files for more details.

##### libAmigaUtils.a

This is a library with no external dependencies, so it can be linked to any project. Exported functions are also usable from a GCC project.

##### libAmigaGCCUtils.a

This library provides tools which require a C runtime environment and thus can not be simply linked against a pure ASM project.
<br>
<br>
<br>
Now you can build the example projects below.

### VASMOnly

A sample project which is for pure assembly projects using VAsm. This project can not be mixed with C/C++ code, as it is a "bare-metal" project, having no crt startup code.

### VASMGcc

This project is a sample to show how to mix assembler and C/C++ objects in a single project (or by using a library).
The assembler part is built as a seperate static library in CMake and linked to your executable, so you canuse the same approach for creatig asm libraries as well.

### VASMGccLib

This project shows an example where you create a library with ASM modules and link them to your main executable.

The above examples should cover all common project szenarios.

Have fun with assembler :-).


### Example

```
cd amiga-utils/CMake/VasmGCC
cmake . -B build-release
cd build-release
make
```

Help: -D CMAKE_FIND_DEBUG_MODE=ON
