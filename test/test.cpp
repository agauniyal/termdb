#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "termdb.hpp"

using namespace tdb;

TEST_CASE("Successful construction")
{
    // TermDb<Exceptions::ON> parser("xterm", "terminfo/");
    TermDb<Exceptions::ON> parser("xterm", "terminfo\\");

    REQUIRE(parser);
    REQUIRE(parser.getName().size() != 0);
}


TEST_CASE("Failure to construct")
{
    try {
        TermDb<Exceptions::ON> parser("NON_EXISTENT_TERM_FOR_DEMO");
    } catch (std::error_code &e) {
        REQUIRE(e == ParseError::ReadError);
    }
}


TEST_CASE("Successfull parsing")
{
    TermDb<Exceptions::OFF> parser;
    // const auto result = parser.parse("xterm", "terminfo/");
    const auto result = parser.parse("xterm", "terminfo\\");

    REQUIRE(result);
    REQUIRE(parser);
}


TEST_CASE("Failure before parsing")
{
    TermDb<Exceptions::OFF> parser;
    const auto result = parser.parse("NON_EXISTENT_TERM_FOR_DEMO");

    REQUIRE_FALSE(result);
    REQUIRE_FALSE(parser);
    REQUIRE(parser.getName().size() == 0);
}


TEST_CASE("No Magic Bytes")
{
    TermDb<Exceptions::OFF> parser;
    // const auto result = parser.parse("corrupt-magic", "terminfo/");
    const auto result = parser.parse("corrupt-magic", "terminfo\\");

    REQUIRE_FALSE(result);
    REQUIRE_FALSE(parser);
}


TEST_CASE("Size0 Error")
{
    TermDb<Exceptions::OFF> parser;
    // const auto result = parser.parse("corrupt-size", "terminfo/");
    const auto result = parser.parse("corrupt-size", "terminfo\\");

    REQUIRE_FALSE(result);
    REQUIRE_FALSE(parser);
}


TEST_CASE("Corrupted Database")
{
    TermDb<Exceptions::OFF> parser;
    // const auto result = parser.parse("corrupted", "terminfo/");
    const auto result = parser.parse("corrupted", "terminfo\\");

    REQUIRE_FALSE(result);
    REQUIRE_FALSE(parser);
}


TEST_CASE("DataResets successfully")
{
    TermDb<Exceptions::OFF> parser;
    // auto result = parser.parse("xterm", "terminfo/");
    auto result = parser.parse("xterm", "terminfo\\");
    REQUIRE(result);

    auto name = parser.getName();
    // result    = parser.parse("adm3a", "terminfo/");
    result    = parser.parse("adm3a", "terminfo\\");
    REQUIRE(result);

    REQUIRE_FALSE(parser.getName() == name);
}


TEST_CASE("Wrong Arguments")
{
    TermDb<Exceptions::OFF> parser;
    auto result = parser.parse("");
    REQUIRE_FALSE(result);

    result = parser.parse("xterm", "");
    REQUIRE_FALSE(result);
}


TEST_CASE("Name")
{
    // TermDb<Exceptions::ON> parser("adm3a", "terminfo/");
    TermDb<Exceptions::ON> parser("adm3a", "terminfo\\");
    auto name = parser.getName();

    REQUIRE(name == "adm3a|lsi adm3a");
}


TEST_CASE("Booleans")
{
    // TermDb<Exceptions::ON> parser("adm3a", "terminfo/");
    TermDb<Exceptions::ON> parser("adm3a", "terminfo\\");
    std::bitset<44> arr;

    for (auto i = 0; i < 44; ++i) {
        auto currCap = static_cast<bin>(i);
        arr[i]       = parser.get(currCap);
    }

    auto revString = "00000010000000000000000000000000000000000010";
    REQUIRE(arr.to_string() == revString);
}


TEST_CASE("Numbers")
{
    // TermDb<Exceptions::ON> parser("adm3a", "terminfo/");
    TermDb<Exceptions::ON> parser("adm3a", "terminfo\\");

    std::vector<uint16_t> hardNums(39, 65535);
    // hardocoded for adm3a
    hardNums[0] = 80;
    hardNums[2] = 24;

    std::vector<uint16_t> parsedNums(39, 65535);

    for (auto i = 0; i < 39; ++i) {
        auto currCap  = static_cast<num>(i);
        parsedNums[i] = parser.get(currCap).value_or(0);
    }

    REQUIRE(parsedNums.size() == hardNums.size());
}
