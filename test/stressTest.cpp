#include <termdb.hpp>
#include <iostream>

using namespace tdb;

int main()
{
	std::ifstream names("stressTestTerms.txt");
	std::ofstream output("output.txt");
	if (!names || !output) {
		return -1;
	}

	std::vector<std::string> nameList;
	nameList.reserve(2718);
	std::string name;
	TermDb parser;

	while (std::getline(names, name)) {
		nameList.emplace_back(name);
	}

	for (auto &term : nameList) {
		parser.parse(term, "mirror/");
		std::string termName(parser.getTermName());

		for (auto i = 0; i < tdb::numCapBool; ++i) {
			const auto b = parser.getCapability(static_cast<bin>(i));
			output << b << " ";
		}
		output << "\n";

		for (auto i = 0; i < tdb::numCapNum; ++i) {
			const auto n = parser.getCapability(static_cast<num>(i));
			output << n << " ";
		}
		output << "\n";

		for (auto i = 0; i < tdb::numCapStr; ++i) {
			const auto s = parser.getCapability(static_cast<str>(i));
			output << s << " ";
		}
		output << "\n\n";
	}

	return 0;
}
