#include <iostream>
#include <gtest/gtest.h>
#include <termdb.hpp>


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
