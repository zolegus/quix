# Command to remove a file; default ‘rm -f’.
RM:=rm -f

# Archive-maintaining program; default ‘ar’.
AR:=

# Flags to give the archive-maintaining program; default ‘rv’.
ARFLAGS:=

# Program for compiling assembly files; default ‘as’.
AS:=

# Extra flags to give to the assembler (when explicitly invoked on a ‘.s’ or ‘.S’ file).
ASFLAGS:=

# Program for compiling C programs; default ‘cc’.
CC := /usr/local/bin/gcc

# Extra flags to give to the C compiler.
CFLAGS :=
CFLAGS += -L/usr/local/lib
CFLAGS += -L/usr/local/lib64
CFLAGS += -O3

# Program for compiling C++ programs; default ‘g++’.
CXX := /usr/local/bin/gcc

# Extra flags to give to the C++ compiler.
CXXFLAGS :=
CXXFLAGS += -std=c++17
#CXXFLAGS += -fno-omit-frame-pointer
CXXFLAGS += -L/usr/local/lib
CXXFLAGS += -L/usr/local/lib64
CXXFLAGS += -lstdc++
CXXFLAGS += -lpthread
CXXFLAGS += -Isrc/
CXXFLAGS += -O3

# Program for running the C preprocessor, with results to standard output; default ‘$(CC) -E’.
CPP:=

# Extra flags to give to the C preprocessor and programs that use it (the C and Fortran compilers).
CPPFLAGS:=

# Library flags or names given to compilers when they are supposed to invoke the linker, ‘ld’. LOADLIBES is a deprecated (but still supported) alternative to LDLIBS. Non-library linker flags, such as -L, should go in the LDFLAGS variable.
LDLIBS:=

# Extra flags to give to compilers when they are supposed to invoke the linker, ‘ld’, such as -L. Libraries (-lfoo) should be added to the LDLIBS variable instead.
LDFLAGS:=

# Program for compiling or preprocessing Fortran and Ratfor programs; default ‘f77’.
FC:=

# Extra flags to give to the Fortran compiler for Ratfor programs.
RFLAGS:=

# Extra flags to give to the Fortran compiler.
FFLAGS:=

# Program to use to compile Modula-2 source code; default ‘m2c’.
M2C:=

# Program for compiling Pascal programs; default ‘pc’.
PC:=

# Extra flags to give to the Pascal compiler.
PFLAGS:=

# Program for extracting a file from RCS; default ‘co’.
CO:=

# Extra flags to give to the RCS co program.
COFLAGS:=

# Program for extracting a file from SCCS; default ‘get’.
GET:=

# Extra flags to give to the SCCS get program.
GFLAGS:=

# Program to use to turn Lex grammars into source code; default ‘lex’.
LEX:=

# Extra flags to give to Lex.
LFLAGS:=

# Program to use to turn Yacc grammars into source code; default ‘yacc’.
YACC:=

# Extra flags to give to Yacc.
YFLAGS:=

# Program to use to run lint on source code; default ‘lint’.
LINT:=

# Extra flags to give to lint.
LINTFLAGS:=

# Program to convert a Texinfo source file into an Info file; default ‘makeinfo’.
MAKEINFO:=

# Program to make TeX DVI files from TeX source; default ‘tex’.
TEX:=

# Program to make TeX DVI files from Texinfo source; default ‘texi2dvi’.
TEXI2DVI:=

# Program to translate Web into TeX; default ‘weave’.
WEAVE:=

# Program to translate C Web into TeX; default ‘cweave’.
CWEAVE:=

# Program to translate Web into Pascal; default ‘tangle’.
TANGLE:=

# Program to translate C Web into C; default ‘ctangle’.
CTANGLE:=

