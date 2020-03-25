# Amiga utillity library for Kickstart 1.2+

This library contains utillity functions for development on Amiga operating system. My primary goal is to support Kickstart 1.2+. This library is intended for development on a raw Amiga without much support of the OS, so most of the functions can be used without any OS being available. Also the core utillity library is free of dependencies to standard C library functions.
All features of the library have simple but fully working demonstration projects for use in ASM or in C/CPP.

## CMake examples with for Amiga 

These are examples for creating Amiga Projects using AmigaOS compiler from bebbo: https://github.com/bebbo/amiga-gcc

`make binutils gcc gprof fd2sfd fd2pragma ira sfdc vasm vbcc vlink libnix libgcc clib2 ndk ndk13`


The examples should run on any kickstartfrom 1.x up.

For using this package, you need either a Linux installation or under Windows using MSYS2 MSYS (not MingW).

### Installation

The default prefix path for the toolchain is expected to be `/opt/amiga/`. If you have setup your toolchain in a different path, you may take a look into the tools directory and adjust the paths in `amiga-env` accordingly.

```
git clone https://github.com/skeetor/amiga-utils
export PATH=<MYPATH>/amiga-utils/tools:$PATH
. amiga-env
cd amiga-utils/utilslib
cmake . -Bbuild-release
cd build-release
make && make install
```

To also build all sample projects in one go, execute the following

```
cd amiga-utils/projects
cmake . -Bbuild-release
cd build-release
make
```

Now you will find the sample executables in `amiga-utils/projects/build-release/bin` and can be copied to an Amiga machine.

### Libraries

There will two libraries installed with utillity functions, usefull for common Amiga projects. Refer to the respective subdirectores/include files for more details.

##### libAmigaUtils.a

This is a library with no external dependencies, so it can be linked to any project. Exported functions are also usable from a GCC project.

##### libAmigaC.a

This is a library with a limited set of standard C functions, which is used to allow including c modules into the Utils library, without injecting a full C library. This is not supposed to be a full standard C library implementation and should not be linked if a C/CPP project is used as it is intended for pure ASM projects only.

##### libAmigaGCCUtils.a

This library provides tools which require a C runtime environment and thus can not be simply linked against a pure ASM project.

## Projects

Now you can build the example projects below.


### Example

```
cd amiga-utils/projects/VasmGCC
cmake . -Bbuild-release
cd build-release
make
```
Have fun with assembler :-).

### VASMOnly

A sample project which is for pure assembly projects using VAsm. This project can not be mixed with C/C++ code, as it is a "bare-metal" project, having no crt startup code.

### VASMGcc

This project is a sample to show how to mix assembler and C/C++ objects in a single project (or by using a library).
The assembler part is built as a seperate static library in CMake and linked to your executable, so you canuse the same approach for creatig asm libraries as well.

### VASMGccLib

This project shows an example where you create a library with ASM modules and link them to your main executable.

The above examples should cover all common project szenarios.

### SimpleCopper

A project which creates a very simple static copperlist.

### SimpleCopperAnim

A simple copperlist, where the copper is used to move a colorbar with the vertical blank interrupt. This project also shows how to take into account the difference between PAL and NTSC and how the copper has to be used when the extra lines of the PAL screen are to be used.

### IntegerMath

Demonstration for the 32 bit div/mult and printing formatted strings from ASM.
The CPP project also includes a some performance comparison and selftest.

### MODPlayer

Demonstrates the usage of the ptplayer module for playing MOD soundfiles.

### Compression

Project for testing and showing the usage of the zlib compression.

Help: -D CMAKE_FIND_DEBUG_MODE=ON

Compiling m68k-amigaos:
make update
make -j 4 binutils gcc vasm vbcc vlink libnix libgcc clib2 ndk ndk13

### PrintFormatted

Demonstrates the usage of printing formatted strings.


### ImageConvert

A tool to convert between PNG and Amiga Bitplane format in both directions, reduce
colors and other features.
This tool is also available for Windows and Linux. It is not recommended though
to run it directly on the Amiga as it is REALLY painfully slow.

**Convert the input PNG file to an Palette PNG with 32 colors.**<br>
ImageConvert -i tmp/green_orig.png -o tmp/green_orig_amiga.png --colors 32

**Convert the input PNG file to an amiga bitplane format with a default of 5 bitplanes.
The bitplanes are stored one after the other.**<br>
ImageConvert -i tmp/green_orig.png -o amiga tmp/green_amiga.abpl

**Convert the input PNG file to an amiga bitplane format with a default of 5 bitplanes.
The bitplanes are stored interleaved**<br>
ImageConvert -i tmp/green_orig.png -o amiga tmp/green_amiga.abpl --interleave

**Convert the amiga bitplanefile to PNG.**<br>
ImageConvert -i amiga tmp/green_amiga.abpl -o tmp/green_from_amiga.png

**Build on Windows:**<br>
`cd amiga-utils/pctools`<br>
Doubleclick on `AmigaTools.sln` and build with Visual Studio (tested with 2019 Community Edition).

**Build on Linux:**<br>
With CLion just point it to the `amiga-utils` directory and it will load the `CMakeLists.txt` file. Note that this will ONLY build the PC Tools and not the Amiga versions.
```
cd amiga-utils
cmake . -Bbuild-release
cd build-release
make
```

The tools works both as 3 2bit and 64 bit builds.
