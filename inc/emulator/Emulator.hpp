#include <string>

#include "Util.hpp"
#include "EmulatorUtil.hpp"
#include "AssemblyUtil.hpp"

using namespace EmulatorUtil;

class Emulator;
typedef EmulatorResult (Emulator::* EmulatorMethod)(BYTE, BYTE, BYTE, BYTE, DWORD);

class Emulator
{
public:
    Emulator() = delete;
    Emulator(std::string file);
    EmulatorResult ReadInputProgram();
    EmulatorResult StartEmulating();
    void Print() const;
    ~Emulator();

private:
    void InitActionsMap();
/// Helper methods   
    EmulatorResult PushStack(BYTE value);
    EmulatorResult PopStack(BYTE& value);
    EmulatorResult PushStack(DWORD value);
    EmulatorResult PopStack(DWORD& value);

    const BYTE FetchInstructionCode(DWORD instr) const;
    const BYTE FetchInstructionSubCode(DWORD instr) const;
    const BYTE FetchInstructionRegisterA(DWORD instr) const;
    const BYTE FetchInstructionRegisterB(DWORD instr) const;
    const BYTE FetchInstructionRegisterC(DWORD instr) const;
    const DWORD FetchInstructionPayload(DWORD instr) const;

/// Main Emulator methods mapped into actions
    EmulatorResult Halt(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult SoftwareInterrupt(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult Call(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult Jump(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult AtomicExchange(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult ArithmeticOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult LogicOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult MoveOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult DataStoreOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
    EmulatorResult DataLoadOperation(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);

    EmulatorResult Placeholder(BYTE subcode, BYTE regA, BYTE regB, BYTE regC, DWORD payload);
private:
    std::map<const BYTE, EmulatorMethod> Actions = {};
    std::string m_inputFile;

    std::vector<DWORD> m_programData;

    DWORD m_stackIndex;
    DWORD m_pcIndex;
    
    std::vector<DWORD> m_GPR;
    std::vector<DWORD> m_CSR;
    BYTE* m_memory;
};



