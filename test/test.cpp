#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "termdb.hpp"

#if defined(__unix__) || defined(__unix) || defined(__linux__)
std::string testPath = "terminfo/";
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
std::string testPath = "..\\terminfo\\";
#elif defined(__APPLE__) || defined(__MACH__)
std::string testPath = "terminfo/";
#else
#error Unknown Platform
#endif

using namespace tdb;

TEST_CASE("Successful construction")
{
    TermDb<Exceptions::ON> parser("xterm", testPath);
    REQUIRE(parser.getName().size() != 0);

    TermDb<Exceptions::OFF> parser2("xterm", testPath);
    REQUIRE(parser2.getName().size() != 0);
}


TEST_CASE("Failure to construct")
{
    try {
        TermDb<Exceptions::ON> parser("NON_EXISTENT_TERM_FOR_DEMO");
    } catch (std::error_code &e) {
        REQUIRE(e == ParseError::ReadError);
    }

    TermDb<Exceptions::OFF> parser2("NON_EXISTENT_TERM_FOR_DEMO");
    REQUIRE_FALSE(parser2);
    REQUIRE(parser2.getName().size() == 0);
}


TEST_CASE("Failure before parsing")
{
    TermDb<Exceptions::OFF> parser("NON_EXISTENT_TERM_FOR_DEMO");
    REQUIRE_FALSE(parser);
    REQUIRE(parser.getName().size() == 0);
}


TEST_CASE("No Magic Bytes")
{
    TermDb<Exceptions::OFF> parser("corrupt-magic", testPath);
    REQUIRE_FALSE(parser);
    REQUIRE(parser.getName().size() == 0);

    try {
        TermDb<Exceptions::ON> parser2("corrupt-magic", testPath);
    } catch (std::error_code &e) {
        REQUIRE(e == ParseError::MagicByteError);
    }
}


TEST_CASE("Size0 Error")
{
    TermDb<Exceptions::OFF> parser("corrupt-size", testPath);
    REQUIRE_FALSE(parser);

    try {
        TermDb<Exceptions::ON> parser2("corrupt-size", testPath);
    } catch (std::error_code &e) {
        REQUIRE(e == ParseError::BadDatabase);
    }
}


TEST_CASE("Corrupted Database")
{
    TermDb<Exceptions::OFF> parser("corrupted", testPath);

    REQUIRE_FALSE(parser);
}


// TEST_CASE("DataResets successfully")
// {
//     TermDb<Exceptions::OFF> parser;
//     auto result = parser.parse("xterm", testPath);
//     REQUIRE(result);

//     auto name = parser.getName();
//     result    = parser.parse("adm3a", testPath);
//     REQUIRE(result);

//     REQUIRE_FALSE(parser.getName() == name);
// }


TEST_CASE("Wrong Arguments")
{
    TermDb<Exceptions::OFF> parser("");
    REQUIRE_FALSE(parser);
    REQUIRE(parser.getName().size() == 0);

    TermDb<Exceptions::OFF> parser2("xterm", "");
    REQUIRE_FALSE(parser2);
    REQUIRE(parser2.getName().size() == 0);
}


TEST_CASE("Name")
{
    TermDb<Exceptions::ON> parser("adm3a", testPath);
    auto name = parser.getName();

    REQUIRE(name == "adm3a|lsi adm3a");
}


TEST_CASE("Booleans")
{
    TermDb<Exceptions::ON> parser("adm3a", testPath);
    TermDb<Exceptions::OFF> parser2("adm3a", testPath);
    std::bitset<44> arr1, arr2;

    for (auto i = 0; i < 44; ++i) {
        auto currCap = static_cast<bin>(i);
        arr1[i]      = parser.get(currCap);
        arr2[i]      = parser2.get(currCap);
    }

    auto revString = "00000010000000000000000000000000000000000010";
    REQUIRE(arr1.to_string() == revString);
    REQUIRE(arr2.to_string() == revString);
}


TEST_CASE("Numbers")
{
    TermDb<Exceptions::ON> parser1("adm3a", testPath);
    TermDb<Exceptions::OFF> parser2("adm3a", testPath);

    std::vector<uint16_t> hardNums1(39, 65535), hardNums2(39, 65535);
    // hardocoded for adm3a
    hardNums1[0] = hardNums2[0] = 80;
    hardNums1[2] = hardNums2[2] = 24;

    std::vector<uint16_t> parsedNums1(39, 65535), parsedNums2(39, 65535);

    for (auto i = 0; i < 39; ++i) {
        auto currCap   = static_cast<num>(i);
        parsedNums1[i] = parser1.get(currCap).value_or(0);
        parsedNums2[i] = parser2.get(currCap).value_or(0);
    }

    REQUIRE(parsedNums1.size() == hardNums1.size());
    REQUIRE(parsedNums2.size() == hardNums2.size());
}
