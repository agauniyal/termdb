#include <gtest/gtest.h>
#include <termdb.hpp>

using namespace tdb;

TEST(Constructor, Success)
{
	TermDb parser("xterm", "terminfo/");

	ASSERT_TRUE(parser);
	ASSERT_EQ(0, parser.getStatus());
	ASSERT_NE(0, parser.getTermName().size());
}


TEST(Constructor, NON_EXISTENT_TERM)
{
	TermDb parser("NON_EXISTENT_TERM_FOR_DEMO");

	ASSERT_FALSE(parser);
	ASSERT_NE(0, parser.getStatus());
	ASSERT_EQ(-2, parser.getStatus());
}


TEST(Parse, Success)
{
	TermDb parser;
	auto result = parser.parse("xterm", "terminfo/");

	ASSERT_TRUE(result);
	ASSERT_TRUE(parser);
	ASSERT_EQ(0, parser.getStatus());
}


TEST(Parse, NON_EXISTENT_TERM)
{
	TermDb parser;
	auto result = parser.parse("NON_EXISTENT_TERM_FOR_DEMO");

	ASSERT_FALSE(result);
	ASSERT_FALSE(parser);
	ASSERT_NE(0, parser.getStatus());
	ASSERT_EQ(-2, parser.getStatus());
	ASSERT_EQ(0, parser.getTermName().size());
}


TEST(Parse, NoMagicBytes)
{
	TermDb parser;
	auto result = parser.parse("corrupt-magic", "terminfo/");

	ASSERT_FALSE(result);
	ASSERT_FALSE(parser);
	ASSERT_NE(0, parser.getStatus());
	ASSERT_EQ(-3, parser.getStatus());
}


TEST(Parse, Size0)
{
	TermDb parser;
	auto result = parser.parse("corrupt-size", "terminfo/");

	ASSERT_FALSE(result);
	ASSERT_FALSE(parser);
	ASSERT_NE(0, parser.getStatus());
	ASSERT_EQ(-2, parser.getStatus());
}


TEST(Parse, Corrupted)
{
	TermDb parser;
	auto result = parser.parse("corrupted", "terminfo/");

	ASSERT_FALSE(result);
	ASSERT_FALSE(parser);
	ASSERT_NE(0, parser.getStatus());
	ASSERT_EQ(-4, parser.getStatus());
}


TEST(Parse, DataResets)
{
	TermDb parser;
	auto result = parser.parse("xterm", "terminfo/");
	EXPECT_TRUE(result);

	auto name = parser.getTermName();
	result    = parser.parse("adm3a", "terminfo/");
	EXPECT_TRUE(result);

	ASSERT_NE(name, parser.getTermName());
}


TEST(Parse, WrongArguments)
{
	TermDb parser;
	auto result = parser.parse("");
	ASSERT_FALSE(result);

	result = parser.parse("xterm", "");
	ASSERT_FALSE(result);
}


TEST(Capablities, Name)
{
	TermDb parser("adm3a", "terminfo/");
	auto name = parser.getTermName();

	ASSERT_EQ("adm3a|lsi adm3a", name);
}


TEST(Capablities, Booleans)
{
	TermDb parser("adm3a", "terminfo/");

	std::bitset<44> arr;

	for (auto i = 0; i < 44; ++i) {
		auto currCap = static_cast<bin>(i);
		arr[i]       = parser.getCapablity(currCap);
	}

	auto revString = "00000010000000000000000000000000000000000010";
	ASSERT_EQ(revString, arr.to_string());
}


TEST(Capablities, Numbers)
{
	TermDb parser("adm3a", "terminfo/");

	std::vector<uint16_t> hardNums(39, 65535);
	// hardocoded for adm3a
	hardNums[0] = 80;
	hardNums[2] = 24;

	std::vector<uint16_t> parsedNums(39, 65535);

	for (auto i = 0; i < 39; ++i) {
		auto currCap  = static_cast<num>(i);
		parsedNums[i] = parser.getCapablity(currCap);
	}

	ASSERT_EQ(hardNums.size(), parsedNums.size())
	  << "Vectors hardNums and parsedNums are of unequal length";

	for (size_t i = 0; i < hardNums.size(); ++i) {
		EXPECT_EQ(hardNums[i], parsedNums[i])
		  << "Vectors hardNums and parsedNums differ at index " << i;
	}
}
