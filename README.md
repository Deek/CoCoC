# CoCo C

This is a repository of buildable OS9/NitrOS9 C source code and the tools needed
to build it.

## Commands Included (not a complete list)

Program|Description
-------|-----------
c.pass1/c.pass2 | Microware (MW) two-pass (Level I) C compiler, Edition #5
c.comp	| MW one-pass (Level II) C compiler Ed. #5
c.opt	| MW assembly source optimizer Ed. #5
c.asm	| MW RMA assembler, Ed. #4
cc1	| MW two-pass compiler executive, Ed. #4
cpp	| Advanced preprocessor for the MW C compiler
dcc	| Advanced, repaired replacement C compiler
grep/egrep/fgrep | Unix file/text search tools
help	| Tim Kientzle's public domain structured help facility
make	| Tim Kientzle's update of Neil Russell's public domain Make
rdump	| Classic `rdump` updated to work with ROF version 1
rlink	| MW ROF linker from the Development System, Ed. #10
rma	| MW RMA assembler, Ed. #11
sed	| Unix stream editor

## Source packages included:

### Source/Archiver/

> Dir|Description
> ---|-----------
> AR	| Carl Kreider's OS-9 Ar archiver
> CUTS	| The CoCo Usenet Transfer System (like uuencode)

### Source/Compiler/

> Dir|Description
> ---|-----------
> CC	| Carl Kreider's `cc` compiler executive
> CC2	| Rick Adams's `cc2` compiler executive
> CC09	| The DCC compiler
> COpt	| The DCC assembly optimizer
> CPP	| The DCC preprocessor
> DCC	| DCC compiler executive
> Lex	| Lexical analyzer generator
> Yacc	| Yet Another Compiler Compiler, a program for generating syntax parsers

### Source/Libs/

> Dir|Description
> ---|-----------
> CGfx	| Mike Sweet's C graphics library, V7 with fixes and enhancements
> KLibc	| Carl Kreider's standard C library, with fixes and enhancements

### Source/

> Dir|Description
> ---|-----------
> Forth09 | Dennis Weldy's Forth interpreter for OS-9
> Help	| Tim Kientzle's more advanced structured Help system
> Make	| Tim Kientzle's updated Russell Make build tool
> XLisp	| David Betz's OO Lisp interpreter for OS-9

## Using OS9-type line endings:

This repository is intended to be useful when overlaid right on top of an
OS9-type filesystem. If you are using it on a Unix-style system (or a Windows
system with the `tr` utility), you can add the contents of the file `gitsetup`
in the root of this repository to your checkout's `.git/config` file. If you
delete all files in the checkout (that is, all the files _not_ in the `.git/`
directory) and call `git checkout` again to get them all back, then all of the
text files (mostly source code) will be checked out with CR-only line endings
instead of LF for Unix-type systems or CRLF for DOS/Windows-type systems. At
this point, you should be able to copy the files over to an OS-9 filesystem
without causing trouble for it.
