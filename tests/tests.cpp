#include "instruction.hpp"
#include "util.hpp"

#include <iostream>

using namespace AssemblyUtil;

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
    Instruction instr;
    uint32_t expected = 0x11223344;
    instr.SetInstructionCode(0x11);
    instr.SetInstructionRegisterA(0x2);
    instr.SetInstructionRegisterB(0x2);
    instr.SetInstructionRegisterC(0x3);
    instr.SetInstructionPayload(0x344);

    ASSERT_EQ(instr.GetInstructionUInt32(), expected)
}

int main()
{
    RUN_TEST(instruction1)
}