#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/poldam_util.h"

TEST_CASE("[TEST_UTIL]: split()")
{
    const std::string tmp = "Hello, My name is Yebityon, Nice to meet you";
    const std::vector<std::string> res = POLDAM_UTIL::split(tmp);

    CHECK(res.size() == 3);
    CHECK(res[0] == "Hello");
    CHECK(res[2] == " Nice to meet you");
}

TEST_CASE("[TEST_UTIL]: split(), corner")
{
    const std::string tmp = ",hello,";
    const std::vector<std::string> res = POLDAM_UTIL::split(tmp);

    CHECK(res.size() == 3);
    CHECK(res[0].size() == 0);
    CHECK(res[1] == "hello");
    CHECK(res[2].empty());
}