#include "PROJECTS"
#if USING_PROJECT == LAGRANGE_POLYNOMIAL
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

// Contains numerator and denominator (integer)
struct Fraction
{
	long long numerator, denominator;
	constexpr void simplify();
};

struct Point { long long x, y; }; // Point struct
// Polynomial, where the index is treated as degree and the value is the coefficient
using Polynomial = std::vector<Fraction>;

// simplify the fraction by factor out gcd 
constexpr void Fraction::simplify()
{
	if (!denominator || !numerator) return; // zero or undefined
	long long gcd = std::gcd(std::abs(numerator), std::abs(denominator));
	numerator /= gcd;
	denominator /= gcd;
	if (denominator < 0) // no negative sign on denominator
	{
		numerator *= -1;
		denominator *= -1;
	}
}

// addition operator overloaded
static constexpr Fraction operator+(Fraction x, Fraction y)
{
	if (x.denominator == y.denominator)
		return Fraction{ x.numerator + y.numerator, x.denominator };
	x.numerator = x.numerator * y.denominator + y.numerator * x.denominator;
	x.denominator = x.denominator * y.denominator;
	return x;
}

// call emplace_back if the current point has a correct syntax, return false otherwise
static bool point_validation(std::string const& str, std::vector<Point>& points)
{
	std::string temp[2] = { "", "" };
	unsigned char now = 0, has_num = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			temp[now] += str[i];
			has_num |= 1 << now;
		}
		else if (str[i] == '-') // negative sign
		{
			if (has_num & (1 << now))
				return false;
			temp[now] += str[i];
		}
		// comma separation
		else if (str[i] == ',')
		{
			if (now == 1)
				return false;
			now++;
		}
		else if (str[i] == ' '); // ignore space
		else return false; // invalid character
	}
	if (!now || has_num != 3) return false;
	Point pt = { std::stoll(temp[0]), std::stoll(temp[1]) };
	auto f = std::find_if(points.begin(), points.end(),
		[&](Point const& p) -> bool { return p.x == pt.x && p.y == pt.y; });
	if (f == points.end()) // ensures that no identical poinst are being inputted
		points.emplace_back(pt);
	return true;
}

// find all combination using recursion
static void recursive_multiplicative_combination_sum(
	std::vector<Point> const& points, long long& result, int ignore,
	std::vector<long long>& tmpcache, int start, int index, int r)
{
	// current combination is ready
	if (index == r)
	{
		long long tmp = 1;
		for (int j = 0; j < r; j++)
			tmp *= tmpcache[j];
		result += tmp;
		return;
	}
	// replace index with all possible elements
	// the condition "points.size() - i >= r - index" guaranteed that 
	// including one element at index will make a combination with remaining elements at remaining positions
	for (int i = start; i <= points.size() - 1 && points.size() - i >= r - index; i++)
	{
		if (i == ignore) continue; // ignore a certain index
		tmpcache[index] = points[i].x;
		recursive_multiplicative_combination_sum(
			points, result, ignore, tmpcache, i + 1, index + 1, r);
	}
}

// gives out the sum of all mulitplicative combination, such as (xy + xz + yz)
static long long multiplicative_combination_sum(std::vector<Point> const& points, int ignore, int r)
{
	long long result = 0; // zero initialization
	std::vector<long long> tmpcache(r);
	recursive_multiplicative_combination_sum(
		points, result, ignore, tmpcache, 0, 0, r);
	return result;
}

// generate Kronecker delta with regards to the current point
static void generate_kronecker_delta(std::vector<Point> const& points, Polynomial& result, int crnt)
{
	long long den = 1; // denominator
	for (int i = 0; i < points.size(); i++) // product of (xj-xi)
		if (i != crnt)
			den *= points[crnt].x - points[i].x;
	for (int i = 0; i < points.size(); i++)
	{
		result[i] = Fraction{
			(points.size() % 2 != i % 2 ? 1 : -1) // negative sign change
			* multiplicative_combination_sum( // the coefficient of each terms
				points, crnt, static_cast<int>(points.size() - i - 1)),
			den };
		result[i].simplify();
	}
}

// generate lagrange polynomial for the points
static Polynomial generate_lagrange_polynomial(std::vector<Point> const& points)
{
	Polynomial result(points.size()), tmp(points.size()); // degree = points.size() - 1
	std::fill(result.begin(), result.end(), Fraction{ 0, 1 }); // fill in zeros
	for (int i = 0; i < points.size(); i++)
	{
		generate_kronecker_delta(points, tmp, i);
		for (auto& y : tmp) y.numerator *= points[i].y; // the y part
		for (int j = 0; j < result.size(); j++)
			result[j] = result[j] + tmp[j]; // add the polynomial to the result;
	}
	for (auto& x : result)
		x.simplify(); // finally, simplify the fractions
	return result;
}

// the main process of the menu
static int main_process()
{
	// introduction
	std::cout << "\033[37m\033[4mLagrange Polynomial Generator\n\033[0m";
	std::cout << "\033[36mLagrange polynomial is a polynomial that can pass through every given points (xi, yi), ";
	std::cout << "the polynomial can be generated via inputing these points into this generator. ";
	std::cout << "In order to input points, please enter two integers separated by comma, ";
	std::cout << "or anything else if all the process are done.\n\033[0m";
	// taking inputs
	std::vector<Point> points;
	points.reserve(10);
	bool valid_input = true;
	std::cout << "\033[37m\n";
	while (valid_input)
	{
		std::string input;
		std::cout << ">>  ";
		getline(std::cin, input);
		valid_input = point_validation(input, points);
		if (!valid_input && points.size() < 2) // input points < 2
		{
			std::cout << "\n\033[31mERROR: Please input at least two points.\n";
			std::cout << "Press any key to exit . . .";
			std::cin.get();
			std::cout << "\033[0m";
			return -1;
		}
	}
	// generate such a lagrange polynomial
	auto polynomial = generate_lagrange_polynomial(points);
	for (auto const& x : polynomial)
	{
		if (!x.denominator) // if there is a undefined fraction
		{
			std::cout << "\n\033[31mThe polynomial cannot be generated\n\n\033[0m";
			return 0;
		}
	}
	// output the polynomial (formatted)
	std::cout << "\n\033[93mPolynomial: ";
	bool first = true;
	for (int i = static_cast<int>(polynomial.size() - 1); i >= 0; i--)
	{
		if (!polynomial[i].numerator) continue;
		if (!first) std::cout << (polynomial[i].numerator > 0 ? " + " : " - ");
		if (first && polynomial[i].numerator < 0) std::cout << "-";
		if (polynomial[i].denominator != 1) std::cout << '(';
		std::cout << std::abs(polynomial[i].numerator);
		if (polynomial[i].denominator != 1)
			std::cout << '/' << polynomial[i].denominator << ')';
		if (i > 1) std::cout << "x^" << std::to_string(i);
		else if (i == 1) std::cout << "x";
		first = false;
	}
	std::cout << "\n\n\033[0m";
	return 0;
}

// main function
int main(int argc, char** argv)
{
	std::ios_base::sync_with_stdio(false); // no printf
	while (true) // infinite loop
	{
		system("cls"); // clear screen
		int x = main_process();
		if (x == -1) return -1; // exit if the main process gives out error
		std::cout << "\033[36mWanna try again? Type (yes/y) or (no/n) to response.\n\033[37m>>  ";
		std::string response;
		getline(std::cin, response);
		std::cout << "\033[0m";
		for (char& c : response) c = std::tolower(c); // case non-sensitive
		if (response == "yes" || response == "y") continue;
		else if (response == "no" || response == "n") return 0;
		// unexpected response
		std::cout << "\n\033[31mERROR: Expects (yes/y) or (no/n).\n";
		std::cout << "Press any key to exit . . .";
		std::cin.get();
		std::cout << "\033[0m";
		return -1;
	}
	return 0;
}
#endif