# About

**MathProjects** is a repository that collects all "little" projects created by me.
Although the name of repository is called "MathProjects", but they are not guaranteed to be related to math, just most of it is.
Notice that all those projects are console application, but not a library, though you can copy the "math part" only.

Each projects are represented as `.cpp` file, located at "./MathProjects/src".
Every `.cpp` file are independent to each other and has a main function inside it,
meaning that all functions and variables are declared as `static`,
except for only one file, `PROJECTS`, which contains a macro `USING_PROJECT` indicates the project currently used.
All projects has included that file and uses a `if-endif` guard to prevent `main` redefinition.
Also notice that all projects are supposed to be built in `c++20`, so using a previous version of C++ might failed.
`system("cls")` is also used for clear screen, if your platform does not support it, try change it to something else.

# List of MathProjects

1. **[LagrangePolynomial:](#lagrangepolynomialcpp)** started in `02-10-2022`.

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