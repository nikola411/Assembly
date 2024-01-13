#include "emulator.hpp"

#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <string.h> 
#include <iomanip>

#define CallEmulatorMethod(code, subcode, regA, regB, regC, payload) \
    (this->*Actions[code])(subcode, regA, regB, regC, payload)

#define PC m_GPR[m_pcIndex]
#define SP m_GPR[m_stackIndex]
#define cause m_CSR[2]
#define handle m_CSR[1]
#define status m_CSR[0]

#define CHECK_SUCCESS(result) \
    if (result != EMU_SUCCESS) \
        return result
    
#define CHECK_PC_OVERFLOW(pc) \
    if (pc >= MemoryMappedRegisters)\
        return EMU_MEMORY_OVERFLOW

#define CHECK_GPR_OVERFLOW(gpr) \
    if (gpr > 15 || gpr < 0) \
        return EMU_UNKNOWN_REGISTER

#define CHECK_CSR_OVERFLOW(csr) \
    if (csr > 2 || csr < 0) \
        return EMU_INVALID_STATUS_REGISTER

Emulator::Emulator(std::string inputFile) :
    m_pcIndex(15),
    m_stackIndex(14),
    m_inputFile(inputFile)   
{
    m_memory = new BYTE[MemorySize];
    if (m_memory == nullptr)
        abort();
    memset(m_memory, (BYTE)0, MemorySize);

    for (int i = 0; i < 16; ++i)
        m_GPR.push_back((DWORD)0);

    PC = ProgramStart;
    SP = StackStart;

    for (int i = 0; i < 3; ++i)
        m_CSR.push_back((DWORD)0);

    InitActionsMap();
}

EmulatorResult Emulator::ReadInputProgram()
{
    std::string line("");
    std::ifstream file(m_inputFile, std::ios_base::openmode::_S_in);
    std::size_t pos{};
    int base(16);

    ADDRESS cnt = ProgramStart;

    while (file)
    {
        std::getline(file, line);
        auto inputCodes = Split(line, ' ');
        for (auto code: inputCodes)
        {
            BYTE data = std::stoi(code, &pos, base);
            if (cnt + 1 >= MemoryMappedRegisters)
                return EMU_MEMORY_OVERFLOW;
            
            m_memory[cnt] = data;
            cnt += 1;
        }
    }

    return EMU_SUCCESS;
}

EmulatorResult Emulator::StartEmulating()
{
    EmulatorResult result = EMU_ERROR;

    while (result != EMU_FINISH)
    {
        DWORD instruction = 0;
        for (int i = 0; i < 4; ++i)
        {
            BYTE byte = m_memory[PC];
            instruction |= (DWORD)((byte & 0xFF) << (3 - i) * ByteSize);
            PC += 1;
        }
        
        BYTE code = FetchInstructionCode(instruction);
        BYTE subcode = FetchInstructionSubCode(instruction);

        BYTE regA = FetchInstructionRegisterA(instruction);
        CHECK_GPR_OVERFLOW(regA);
        BYTE regB = FetchInstructionRegisterB(instruction);
        CHECK_GPR_OVERFLOW(regB);
        BYTE regC = FetchInstructionRegisterC(instruction);
        CHECK_GPR_OVERFLOW(regC);
        DWORD payload = FetchInstructionPayload(instruction);

        result = CallEmulatorMethod(code, subcode, regA, regB, regC, payload);
        if (result == EMU_ERROR)
            break;
    }

    return result;
}

Emulator::~Emulator()
{
    delete[] m_memory;
}

EmulatorResult Emulator::PushStack(BYTE value)
{
    SP -= 1;
    if (SP == -1)
        return EMU_STACK_OVERFLOW;

    m_memory[SP] = value;
    return EMU_SUCCESS;
}

EmulatorResult Emulator::PopStack(BYTE& value)
{
    value = m_memory[SP];

    SP += 1;
    if (SP == ProgramStart)
        return EMU_STACK_OVERFLOW;

    return EMU_SUCCESS;
}

EmulatorResult Emulator::PushStack(DWORD value)
{
    EmulatorResult result = EMU_SUCCESS;
    for (int i = 0; i < 4; ++i)
    {
        BYTE byte = (value >> (ByteSize * (3 - i))) & 0xFF;

        result = PushStack(byte);
        if (result != EMU_SUCCESS)
            break;
    }

    return result;
}

EmulatorResult Emulator::PopStack(DWORD& value)
{
    EmulatorResult result = EMU_SUCCESS;
    for (int i = 0; i < 4; ++i)
    {
        BYTE byte;
        result = PopStack(byte);
        if (result != EMU_SUCCESS)
            break;
        
        value |= (byte & 0xFF) << (ByteSize * i);
    }

    return result;
}

void Emulator::InitActionsMap()
{
    Actions =
    {
        { 0x00, &Emulator::Halt },
        { 0x10, &Emulator::SoftwareInterrupt },
        { 0x20, &Emulator::Call },
        { 0x30, &Emulator::Jump },
        { 0x40, &Emulator::AtomicExchange },
        { 0x50, &Emulator::ArithmeticOperation },
        { 0x60, &Emulator::LogicOperation },
        { 0x70, &Emulator::MoveOperation },
        { 0x80, &Emulator::DataStoreOperation },
        { 0x90, &Emulator::DataLoadOperation },
    };
}

const BYTE Emulator::FetchInstructionCode(DWORD instr) const
{
    return (instr & 0xF0000000) >> 3 * ByteSize;
}

const BYTE Emulator::FetchInstructionSubCode(DWORD instr) const
{
    return (instr & 0x0F000000) >> 3 * ByteSize;
}

const BYTE Emulator::FetchInstructionRegisterA(DWORD instr) const
{
    return (instr & 0x00F00000) >> (2 * ByteSize + 4);
}

const BYTE Emulator::FetchInstructionRegisterB(DWORD instr) const
{
    return (instr & 0x000F0000) >> (2 * ByteSize);
}

const BYTE Emulator::FetchInstructionRegisterC(DWORD instr) const
{
    return (instr & 0x0000F000) >> (1 * ByteSize + 4);
}

const DWORD Emulator::FetchInstructionPayload(DWORD instr) const
{
    return (instr & 0x00000FFF);
}

EmulatorResult Emulator::Halt(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    return EMU_FINISH;
}

EmulatorResult Emulator::SoftwareInterrupt(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    EmulatorResult result = EMU_ERROR;

    result = PushStack(status);
    CHECK_SUCCESS(result);

    result = PushStack(PC);
    CHECK_SUCCESS(result);    

    cause = MASK_INTERRUPT;
    status = status & (~0x01);
    PC = handle;

    return EMU_SUCCESS;
}

EmulatorResult Emulator::Call(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    DWORD newPC = regA + regB + payload;

    if (subcode == 0x00)
    {
        CHECK_PC_OVERFLOW(newPC);
    
        PC = newPC;
        return EMU_SUCCESS;
    }

    if(subcode == 0x01)
    {
        CHECK_PC_OVERFLOW(newPC);

        PC = m_memory[newPC];
        return EMU_SUCCESS;
    }
  
    return EMU_UNKNOWN_INSTRUCTION;
}

EmulatorResult Emulator::Jump(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    EmulatorResult result = PushStack(PC);
    CHECK_SUCCESS(result);

    bool useMemory = subcode & 0x8;

    DWORD newPC = m_GPR[regA] + payload;
    CHECK_PC_OVERFLOW(newPC);
    if (useMemory)
    {
        newPC = m_memory[newPC];
        CHECK_PC_OVERFLOW(newPC);
    }

    switch (subcode & 0x3)
    {
        case 0x0: // useMemory ? pc<=mem32[gpr[A]+D] :  pc<=gpr[A]+D
        {
            PC = newPC;
            break;
        }
        case 0x1: // useMemory ? if (gpr[B] == gpr[C]) pc<=mem32[gpr[A]+D] : if (gpr[B] == gpr[C]) pc<=gpr[A]+D
        {
            if (m_GPR[regB] == m_GPR[regC])
                PC = newPC;
            break;
        }
        case 0x2: // useMemory ? if (gpr[B] != gpr[C]) pc<=mem32[gpr[A]+D] : if (gpr[B] != gpr[C]) pc<=gpr[A]+D
        {
            if (m_GPR[regB] != m_GPR[regC])
                PC = newPC;
            break;
        }
        case 0x3: // useMemory ? if (gpr[B] signed> gpr[C]) pc<=mem32[gpr[A]+D] : if (gpr[B] signed> gpr[C]) pc<=gpr[A]+D
        {
            if ((int8_t)m_GPR[regB] > m_GPR[regC])
                PC = newPC;
            break;
        }
    }

    return EMU_SUCCESS;
}

EmulatorResult Emulator::AtomicExchange(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    DWORD temp = m_GPR[regB];
    m_GPR[regB] = m_GPR[regC];
    m_GPR[regC] = temp;

    return EMU_SUCCESS;
}

EmulatorResult Emulator::ArithmeticOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    DWORD newValue;
    switch(subcode)
    {
        case 0x0:
        {
            newValue = m_GPR[regB] + m_GPR[regC];
            break;
        }
        case 0x1:
        {
            newValue = m_GPR[regB] - m_GPR[regC];
            break;
        }
        case 0x2:
        {
            newValue = m_GPR[regB] * m_GPR[regC];
            break;
        }
        case 0x3:
        {
            newValue = m_GPR[regB] / m_GPR[regC];
            break;
        }
    }

    m_GPR[regA] = newValue;

    return EMU_SUCCESS;
}

EmulatorResult Emulator::LogicOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    DWORD newValue;
    switch(subcode)
    {
        case 0x0:
        {
            newValue = ~m_GPR[regB];
            break;
        }
        case 0x1:
        {
            newValue = m_GPR[regB] & m_GPR[regC];
            break;
        }
        case 0x2:
        {
            newValue = m_GPR[regB] | m_GPR[regC];
            break;
        }
        case 0x3:
        {
            newValue = m_GPR[regB] ^ m_GPR[regC];
            break;
        }
    }

    m_GPR[regA] = newValue;

    return EMU_SUCCESS;
}

EmulatorResult Emulator::MoveOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    if (subcode == 0x0)
    {
        m_GPR[regA] = m_GPR[regB] << m_GPR[regC];
        return EMU_SUCCESS;
    }

    if (subcode == 0x1)
    {
        m_GPR[regA] = m_GPR[regB] >> m_GPR[regC];
        return EMU_SUCCESS;
    }

    return EMU_UNKNOWN_INSTRUCTION;
}

EmulatorResult Emulator::DataStoreOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    if (subcode > 2 || subcode < 0)
        return EMU_UNKNOWN_INSTRUCTION;

    if (subcode == 0x1) // gpr[A]<=gpr[A]+D; mem32[gpr[A]]<=gpr[C];
    {
        m_GPR[regA] = m_GPR[regA] + payload;
        CHECK_PC_OVERFLOW(m_GPR[regA]);
        m_memory[m_GPR[regA]] = m_GPR[regC];

        return EMU_SUCCESS;
    }

    DWORD address = m_GPR[regA] + m_GPR[regB] + payload;
    CHECK_PC_OVERFLOW(address);
    if (subcode == 0x2) // mem32[mem32[gpr[A]+gpr[B]+D]]<=gpr[C]
        address = m_memory[address];        

    m_memory[address] = m_GPR[regC]; //  mem32[gpr[A]+gpr[B]+D]<=gpr[C]

    return EMU_SUCCESS;
}

EmulatorResult Emulator::DataLoadOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    bool useStatus = subcode & 0x04;
    if (useStatus)
        CHECK_CSR_OVERFLOW(regA);

    DWORD& target = useStatus ? m_CSR[regA] : m_GPR[regA];

    switch (subcode & 0x3)
    {
        case 0x0:
        {
            target = useStatus ? m_GPR[regB] : m_CSR[regB];
            break;
        }
        case 0x1:
        {
            target = useStatus ? m_GPR[regB] + payload : m_CSR[regB] | payload;
            break;
        }
        case 0x2:
        {
            DWORD address = m_GPR[regB] + m_GPR[regC] + payload;
            CHECK_PC_OVERFLOW(address);
            target = m_memory[address];
            break;
        }
        case 0x3:
        {
            CHECK_PC_OVERFLOW(m_GPR[regB]);
            target = m_memory[m_GPR[regB]];
            m_GPR[regB] = m_GPR[regB] + payload;
            break;
        }
    }

    return EMU_SUCCESS;
}

EmulatorResult Emulator::Placeholder(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload)
{
    std::cout << " PLACEHOLDER \n";
    return EMU_SUCCESS;
}
