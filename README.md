# TermDb [![Build Status](https://travis-ci.org/agauniyal/termdb.svg?branch=master)](https://travis-ci.org/agauniyal/termdb) [![codecov](https://codecov.io/gh/agauniyal/termdb/branch/master/graph/badge.svg)](https://codecov.io/gh/agauniyal/termdb)

Terminfo parser for modern c++

```cpp
#include "include/termdb.hpp"
#include <iostream>

int main()
{
	TermDb parser;
	auto result = parser.parse("xterm");

	if (result) {
		auto name = parser.getTermName();
		auto b    = parser.getCapBin(cap::bin::has_meta_key);
		auto n    = parser.getCapNum(cap::num::columns);
		auto s    = parser.getCapStr(cap::str::enter_bold_mode);

		std::cout << "\n"
		          << "Name: " << name << "\n"
		          << "Has Meta Key: " << std::boolalpha << b << "\n"
		          << "Number of columns: " << n << "\n"
		          << "Bold Mode on: " << s << "I should be BOLD!"
		          << "\n";
	} else {
		std::cerr << "Couldn't parse terminfo database\n";
	}
}
```
