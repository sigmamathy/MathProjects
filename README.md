# About

**MathProjects** is a repository that collects all "little" projects created by me.
Although the name of repository is called "MathProjects", but they are not guaranteed to be related to math, just most of it is.
Notice that all those projects are console application, but not a library, though you can copy the "math part" only.

Each projects are represented as `.cpp` file, located at [`./MathProjects/src`](https://github.com/sigmamathy/MathProjects/tree/master/MathProjects/src).
Every `.cpp` file are independent to each other and has a main function inside it,
meaning that all functions and variables are declared as `static`,
except for only one file, `PROJECTS`, which contains a macro `USING_PROJECT` indicates the project currently used.
All projects has included that file and uses a `if-endif` guard to prevent `main` redefinition.
Also notice that all projects are supposed to be built in `c++20`, so using a previous version of C++ might failed.
`system("cls")` is also used for clear screen, if your platform does not support it, try change it to something else.

# List of MathProjects

1. [**LagrangePolynomial:**](#lagrangepolynomialcpp) started in `02-10-2022`.

2. [**VisualBinary:**](#visualbinarycpp) started in `09-10-2022`.

## LagrangePolynomial.cpp

Lagrange polynomial is a polynomial that can pass through every given points `(xi, yi)`.
This project provides a generator of that.
Users can generate such polynomials via inputting points with the following syntax:
```
>>  1, 2
>>  3, 5
>>  -6, 20
```
Finally if no more points needed to be added, just leave everything else then hit enter,
the result will be shown on the screen. For example in this case:  
`Polynomial: (19/42)x^2 - (13/42)x + (13/7)`

> ### Remarks
> 
> The input must be an integer, the current version does not accept rational numbers.  
> At least two points are required, no exceptions.  
> If an error is detected, then the program will call exit `(return -1)` as usual.

## VisualBinary.cpp

Visual Binary is a coding interface that aims to produce a binary file,
it allows users to customize the data "visually" and convert the code into a binary file.
The program was made for easy creation of non-volatile data storage,
as it avoid users to edit binary files or writing data using self-written programs.

### Compile a binary file

First of all, compile the `VisualBinary.cpp` into an executable file, let's say we named it `vbin.exe`.
In order to run it, type this command in the terminal:
```
vbin <source_file> <destination_file>
```
where the `<source_file>` is the location of the source file wanted to be converted,
and the `<destination_file>` is the location of the binary output.
`<destination_file>` can be empty, Visual Binary will automatically generate ones anyway.

### Syntax

The syntax is starightforward, we use `[]` to enclose all the data inside this bracket,
all the code outside the bracket will be ignored. Inside the bracket, we follows the following syntax:
```
[<type> <data> ... <data>]
```
- `<type>`:  
	Indicates the type of the following data,the options can be:  
	`short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `double`, `string`.  
	It basically follows the type of C++, the types with prefix `u` are unsigned.  
	Note that `long` has the size of 8 bytes, which is the equivalent of `long long` in C++.
	If no type was specified, then by default it is `byte`, which essentially a hexadecimal byte in the range of `[00, ff]`.

- `<data> ... <data>`:  
	List of all data with the type specified. We use spaces to separate multiple data.  
	The value specification of data are similar to C++.

For example, check out the following code:
```
[00 0a 3b]
[int -203 59]
[ulong 69696969696969]
[string "Hello World"]
[float -3.1415926535]
```
which produce the following result:
```
00 0A 3B
35 FF FF FF 3B 00 00 00
C9 E6 1E 97 63 3F 00 00
48 65 6C 6C 6F 20 57 6F 72 6C 64
DB 0F 49 C0
```

> ### Remarks
> 
> For any strings, use a string delimiter `""` to enclose it.  
> Escape characters are not supported, use `[0a]` and `[22]` instead.  
> For `float` type, do not put an `f` at the back of the value.  
> If an error is detected, then the program will call exit `(return -1)` as usual.