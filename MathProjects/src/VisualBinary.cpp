#include "PROJECTS"
#if USING_PROJECT == VISUAL_BINARY
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <vector>

namespace stdfs = std::filesystem; // filesystem namespace
// an array of bytes, unsigned char by default
using Bytes = std::vector<unsigned char>;

// types of data available, types with prefix "u" is unsigned
enum class DataType
{
	NONE,		// 1 byte
	SHORT,		// 2 bytes
	USHORT,
	INT,		// 4 bytes
	UINT,
	LONG,		// 8 bytes (long long)
	ULONG,
	FLOAT,		// 4 bytes
	DOUBLE,		// 8 bytes
	STRING
};

// store temporary data for values and statements
struct Data
{
	char readno = 0; // 0 == not read, 1 == first, 2 == after
	DataType type = DataType::NONE; // statement data type
	std::string data = ""; // data storage
};

// output the error with parameter pack and returns -1
static int output_error(auto&&... args)
{
	std::cout << "\033[31mERROR: ";
	(std::cout << ... << args) << "\033[0m";
	return -1;
}

// read bytes in hexadecimal
static int read_byte(std::string const& str, Bytes& result, int line)
{
	if (str.length() != 2) // check byte size
		return output_error("The size of value must be exactly one byte [line:", line, "]\n");
	std::stringstream ss;
	ss << std::hex << str;
	unsigned int x;
	ss >> x;
	if (!ss.eof() || ss.fail()) // error occurred
		return output_error("Value is not a byte (in hexadecimal) [line:", line, "]\n");
	result.emplace_back(static_cast<unsigned char>(x));
	return 0;
}

// read number value
template<class _Ty> // general type
static int read_number(std::string const& str, Bytes& result, int line)
{
	if (std::is_unsigned_v<_Ty> && str[0] == '-') // no negative sign for unsigned type
		return output_error("detected a negative sign, expects unsigned integer [line:", line, "]\n");
	std::stringstream ss;
	ss << str;
	_Ty x;
	ss >> x;
	bool success = ss.eof() && !ss.fail(); // check success
	if (!success) // error occurred
		return output_error("detected a non-", std::is_floating_point_v<_Ty> ? "floating-point" : "integer",
			" value or the value overflows [line:", line, "]\n");
	size_t s = result.size();
	result.resize(result.size() + sizeof(_Ty));
	std::memcpy(&result[s], &x, sizeof(_Ty)); // memory copy into result
	return success - 1;
}

// read string value
static int read_string(std::string const& str, Bytes& result, int line)
{
	if (str[0] != '"' || str.back() != '"') // make sure it is surrounded by the string delimiter
		return output_error("expression not a string, use a string delimiter to enclose it [line:", line, "]\n");
	for (int i = 1; i < str.length() - 1; i++)
		result.emplace_back(str[i]);
	return 0;
}

// check and assign the type of data
static int check_type(Data& data)
{
	if (data.data == "short")			data.type = DataType::SHORT;
	else if (data.data == "int")		data.type = DataType::INT;
	else if (data.data == "long")		data.type = DataType::LONG;
	else if (data.data == "ushort")		data.type = DataType::USHORT;
	else if (data.data == "uint")		data.type = DataType::UINT;
	else if (data.data == "ulong")		data.type = DataType::ULONG;
	else if (data.data == "float")		data.type = DataType::FLOAT;
	else if (data.data == "double")		data.type = DataType::DOUBLE;
	else if (data.data == "string")		data.type = DataType::STRING;
	else return -1;
	return 0;
}

// load and update data information
static int read_data(Data& data, Bytes& result, int line)
{
	int success = 0;
	// if the first value is being read
	if (data.readno < 2)
	{
		++data.readno;
		if (data.readno == 1)
			return 0; // not read
		if (check_type(data) == -1)
			success = read_byte(data.data, result, line); // read byte if not a type
		data.data.clear(); // clear data
		return success;
	}
	// for data.readno >= 2
	switch (data.type) // reads value
	{
		using enum DataType;
	case NONE:		success = read_byte(data.data, result, line); break;
	case SHORT:		success = read_number<short>(data.data, result, line); break;
	case INT:		success = read_number<int>(data.data, result, line); break;
	case LONG:		success = read_number<long long>(data.data, result, line); break;
	case USHORT:	success = read_number<unsigned short>(data.data, result, line); break;
	case UINT:		success = read_number<unsigned int>(data.data, result, line); break;
	case ULONG:		success = read_number<unsigned long long>(data.data, result, line); break;
	case FLOAT:		success = read_number<float>(data.data, result, line); break;
	case DOUBLE:	success = read_number<double>(data.data, result, line); break;
	case STRING:	success = read_string(data.data, result, line); break;
	}
	data.data.clear(); // clear data
	return success;
}

// read the src file and store the result into bytes
static int read_file(stdfs::path const& path, Bytes& result)
{
	std::ifstream ifs(path);
	Data data; // temporary data storage
	data.data.reserve(10);
	char tmp, prev = '\0';
	bool reading = false, string_read = false;
	unsigned int line = 1u;
	while (ifs.get(tmp)) // load character
	{
		switch (tmp)
		{
		case '[': // starts reading statement
			if (string_read) { // if reading string
				data.data += tmp;
				break;
			}
			if (reading)
				return output_error("nesting bracket are not allowed [line:", line, "]\n");
			reading = true;
			break;

		case ']': // end of statement
			if (string_read) { // if reading string
				data.data += tmp;
				break;
			}
			if (!reading)
				return output_error("the right bracket aren't closing anything [line:", line, "]\n");
			// ensures the last value is being read
			read_data(data, result, line);
			// reset statement
			reading = false;
			data.type = DataType::NONE;
			data.data.clear();
			data.readno = 0;
			break;

		case '\n': // new line count
			++line;
			break;

		case '"': // string delimiter
			if (!reading) break; // ignores if not reading statement
			if (!string_read) // begin of string delimiter
				read_data(data, result, line);
			data.data += tmp;
			string_read = !string_read;
			break;

		default: // reading characters
			if (!reading || (tmp == ' ' && !string_read)) break; // spaces are not read
			// check previous character
			if (!string_read && (prev == ' ' || prev == '[' || prev == '\n' || prev == '"' || prev == '\t'))
				read_data(data, result, line);
			data.data += tmp;
			break;
		}
		prev = tmp; // set previous character
	}
	if (reading) // ensures the statement is closed
		return output_error("bracket are not enclosed\n");
	ifs.close();
	return 0;
}

// output the bytes into the destination file 
static int output_file(stdfs::path const& path, Bytes const& bytes)
{
	std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);
	for (char byte : bytes)
		ofs.write((char*)&byte, 1);
	ofs.close();
	return 0;
}

// main process
int main(int argc, char** argv)
{
	if (argc < 2 || argc > 3)
		return output_error("expects 1 or 2 arguments\n");
	stdfs::path src = argv[1], dest = argc == 3 ? argv[2] : "";
	// check if the src file exists
	if (!stdfs::exists(src))
		return output_error("argument 1: src file doesn't exists\n");
	// if destination file is not given, then auto create it
	if (dest.empty())
	{
		if (!src.has_extension())
			dest = src.string() + " - binary";
		else (dest = src).replace_extension(); // remove extension
	}
	Bytes bytes;
	bytes.reserve(32768); // reserve a certain of bytes
	int success = read_file(src, bytes); // read file
	if (success == -1) return -1; // check success state
	success = output_file(dest, bytes); // write file
	return success;
}
#endif