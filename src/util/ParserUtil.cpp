#include "ParserUtil.hpp"

#include <unordered_map>

using namespace ParserUtil;

std::unordered_map<std::string, eGPR> GPRMap =
{
    { "r0", eGPR::R0 },   { "r1", eGPR::R1 },   { "r2", eGPR::R2 },   { "r3", eGPR::R3 },
    { "r4", eGPR::R4 },   { "r5", eGPR::R5 },   { "r6", eGPR::R6 },   { "r7", eGPR::R7 },
    { "r8", eGPR::R8 },   { "r9", eGPR::R9 },   { "r10", eGPR::R10 }, { "r11", eGPR::R11 },
    { "r12", eGPR::R12 }, { "r13", eGPR::R13 }, { "r14", eGPR::R14 }, { "r15", eGPR::R15 }
};

eGPR ParserUtil::GPRStringToEnum(std::string reg)
{
    return GPRMap[reg];
}

std::unordered_map<std::string, eCSR> CSRMap =
{
    { "status",  eCSR::STATUS },
    { "handler", eCSR::HANDLER },
    { "cause",   eCSR::CAUSE }
};

eCSR ParserUtil::CSRStringToEnum(std::string csr)
{
    return CSRMap[csr];
}