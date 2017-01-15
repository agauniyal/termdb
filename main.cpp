#include "include/termdb.hpp"
#include <iostream>

int main()
{
	TermDb parser;
	auto result = parser.parse("xterm");

	if (result) {
		auto name    = parser.getTermName();
		auto boolean = parser.getCapBin(cap::bin::has_meta_key);
		auto numeric = parser.getCapNum(cap::num::columns);
		auto str     = parser.getCapStr(cap::str::enter_bold_mode);

		std::cout << "\n"
		          << "Name: " << name << "\n"
		          << "Has Meta Key: " << std::boolalpha << boolean << "\n"
		          << "Number of columns(-1 if not present): "
		          << ((numeric != cap::NP) ? numeric : -1) << "\n"
		          << "Bold Mode on: " << str << "I should be BOLD!"
		          << "\n";
	} else {
		std::cerr << "Couldn't parse terminfo database\n";
	}
}
