# MathProjects

### LagrangePolynomial.cpp
Lagrange polynomial is a polynomial that can pass through every given points `(xi, yi)`.
This project creates a generator of that.
Users can generate such polynomials via inputting points with the following syntax:
```
>>  1, 2
>>  3, 5
>>  -6, 20
```
Finally if no more poinst needed to be added, just leave everythiing else then hit enter,
the result will be shown on the screen.

#### Remarks
The input must be an integer, the current version does not accept rational numbers.  
At least two points are required, no exceptions.  
If an error is detected, then the program will exit `(return -1)` as usual.