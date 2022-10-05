# List of MathProjects

1. **[LagrangePolynomial:](#LagrangePolynomial.cpp)** started in `08-10-2022`.

## LagrangePolynomial.cpp

Lagrange polynomial is a polynomial that can pass through every given points `(xi, yi)`.
This project creates a generator of that.
Users can generate such polynomials via inputting points with the following syntax:
```
>>  1, 2
>>  3, 5
>>  -6, 20
```
Finally if no more points needed to be added, just leave everything else then hit enter,
the result will be shown on the screen. For example in this case:  
`Polynomial: (19/42)x^2 - (13/42)x + (13/7)`

### Remarks

The input must be an integer, the current version does not accept rational numbers.  
At least two points are required, no exceptions.  
If an error is detected, then the program will exit `(return -1)` as usual.