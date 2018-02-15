#include "termdb.hpp"
#include <iostream>
#include <sstream>
#include <chrono>

using namespace tdb;
using namespace std;

template <typename TimeT = chrono::microseconds>
struct measure {
    template <typename F, typename... Args>
    static typename TimeT::rep execution(F &&func, Args &&... args)
    {
        const auto start = chrono::steady_clock::now();
        forward<decltype(func)>(func)(forward<Args>(args)...);
        const auto duration
          = chrono::duration_cast<TimeT>(chrono::steady_clock::now() - start);
        return duration.count();
    }
};


void runParser(const vector<TermDb<Exceptions::ON>> &parsers)
{
    ostringstream buffer;
    for (auto &parser : parsers) {
        buffer << parser.getName();
        for (auto i = 0; i < tdb::numCapBool; ++i) {
            const auto b = parser.get(static_cast<bin>(i));
            buffer << b;
        }
        for (auto i = 0; i < tdb::numCapNum; ++i) {
            const auto n = parser.get(static_cast<num>(i));
            buffer << n.value_or(0);
        }
        for (auto i = 0; i < tdb::numCapStr; ++i) {
            const auto s
              = parser.get(static_cast<str>(i), 1, 1, 1, 1, 1, 1, 1, 1, 1);
            buffer << s;
        }
    }
}

int main()
{
    // ifstream names("stressTestTerms.txt");
    ifstream names("..\\stressTestTerms.txt");
    if (!names) {
        return -1;
    }

    vector<string> nameList;
    nameList.reserve(2718);

    string name;
    while (getline(names, name)) {
        nameList.emplace_back(name);
    }
    
    vector<TermDb<Exceptions::ON>> parsers;
    parsers.reserve(nameList.size());
    for (auto &term : nameList) {
        try {
            // parsers.emplace_back(term, "mirror/");
             parsers.emplace_back(term, "mirror\\");
        } catch (error_code &e) {
            cerr << '\n' << e << ": " << e.message();
        }
    }

    cout << measure<>::execution(runParser, parsers) << " microseconds" << endl;
    return 0;
}
