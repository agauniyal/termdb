#include "termdb.hpp"
#include <iostream>
#include <sstream>

#if defined(__unix__) || defined(__unix) || defined(__linux__)                 \
  || defined(__APPLE__) || defined(__MACH__)
constexpr auto STPATH     = "stressTestTerms.txt";
constexpr auto MIRRORPATH = "mirror/";
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
constexpr auto STPATH     = "..\\stressTestTerms.txt";
constexpr auto MIRRORPATH = "..\\mirror\\";
#endif

int main()
{
    using namespace tdb;
    using namespace std;

    ifstream names(STPATH);

    if (!names) {
        return -1;
    }

    vector<string> nameList;
    nameList.reserve(2718);

    // make list of names
    string name;
    while (getline(names, name)) {
        nameList.emplace_back(name);
    }

    TermDb<Exceptions::OFF> parser;
    ostringstream buffer;
    for (auto &term : nameList) {
        parser.parse(term, MIRRORPATH);

        string termName(parser.getName());
        buffer << termName << '\n';

        for (auto i = 0; i < tdb::numCapBool; ++i) {
            const auto b = parser.get(static_cast<bin>(i));
            buffer << b << " ";
        }
        buffer << "\n";

        for (auto i = 0; i < tdb::numCapNum; ++i) {
            const auto n = parser.get(static_cast<num>(i));
            buffer << n.value_or(0) << " ";
        }
        buffer << "\n";

        for (auto i = 0; i < tdb::numCapStr; ++i) {
            const auto s
              = parser.get(static_cast<str>(i), 1, 1, 1, 1, 1, 1, 1, 1, 1);
            buffer << s << " ";
        }
        buffer << "\n\n";
    }

    ofstream output("output.txt");
    if (!output) {
        return -1;
    }
    output << buffer.str();
}
