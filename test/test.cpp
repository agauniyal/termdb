#include <iostream>
#include <gtest/gtest.h>
#include <termdb.hpp>


TEST(Constructor, Success)
{
	TermDb parser("xterm");

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
	ASSERT_EQ(0, parser.getTermName().size());
}


TEST(Parse, Success)
{
	TermDb parser;
	auto result = parser.parse("xterm");

	ASSERT_TRUE(result);
	ASSERT_TRUE(parser);
	ASSERT_EQ(0, parser.getStatus());
	ASSERT_NE(0, parser.getTermName().size());
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


TEST(Parse, DataResets)
{
	TermDb parser;
	auto result = parser.parse("xterm");
	EXPECT_TRUE(result);

	auto name = parser.getTermName();
	result    = parser.parse("gnome");
	EXPECT_TRUE(result);

	ASSERT_NE(name, parser.getTermName());
}


TEST(Parse, WrongArguments){
	TermDb parser;
	auto result = parser.parse("");
	ASSERT_FALSE(result);

	result = parser.parse("xterm", "");
	ASSERT_FALSE(result);
}
