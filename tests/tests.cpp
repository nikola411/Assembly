#include "instruction.hpp"
#include "util.hpp"

#include <iostream>

using namespace AssemblyUtil;
using namespace LinkerUtil;

#define TEST_CASE(str, type) \
typedef type str ; \
static const std::string test##str(#str); \
std::string test_##str()

#define ASSERT_EQ(first, second) \
if (first == second) \
{ \
    return "Test passed!";\
} \
else \
{ \
    std::cout << "Values: " << std::hex << first << " and " << std::hex << second << " are not equal \n"; \
    return "Test failed;"; \
}

#define RUN_TEST(str) \
std::cout << "Test: " << test##str << "\n result: " << test_##str() << "\n";

TEST_CASE(instruction1, Instruction)
{
    std::string toSplit = "sekcijakurraac 23";

    auto ret = Split(toSplit, ' ');

    for (auto a: ret)
        std::cout << a << "|";

}

int main()
{
    RUN_TEST(instruction1)
}