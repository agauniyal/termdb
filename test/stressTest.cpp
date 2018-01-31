#include "termdb.hpp"
#include <iostream>
#include <sstream>

// #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
// namespace tdb{
// constexpr const auto numCapBool = 0;
// constexpr const auto numCapNum = 0;
// constexpr const auto numCapStr = 0;
// }
// #endif


int main()
{
    using namespace tdb;
    using namespace std;

    ifstream names("stressTestTerms.txt");
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

    TermDb parser;
    ostringstream buffer;
    for (auto &term : nameList) {
        parser.parse(term, "mirror/");
        string termName(parser.getName());

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
            const auto s = parser.get(static_cast<str>(i), 1, 1, 1, 1,
                                               1, 1, 1, 1, 1);
            buffer << s << " ";
        }
        buffer << "\n\n";
    }

    ofstream output("output.txt");
    if (output) {
        output << buffer.str();
    } else {
        return -1;
    }
}
