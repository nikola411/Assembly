#pragma once

#include <inttypes.h>

namespace EmulatorUtil
{
    enum EmulatorResult
    {
        EMU_SUCCESS = 0x00,
        EMU_ERROR = 0x01,

        EMU_MEMORY_OVERFLOW = 0x02,
        EMU_STACK_OVERFLOW = 0x03,
        EMU_UNKNOWN_INSTRUCTION = 0x04,
        EMU_UNKNOWN_REGISTER = 0x05,
        EMU_INVALID_STATUS_REGISTER = 0x06,

        EMU_FINISH = 0xFF,
    };

    enum StatusRegisterMasks
    {
        MASK_TIMER_INTERRUPT = 0x01,
        MASK_TERMINAL_INTERRUPT = 0x02,
        MASK_INTERRUPT = 0x04
    };

    enum CauseRegisterInterrupts
    {
        INTERRUPT_BAD_INSTRUCTION = 0x01,
        INTERRUPT_TIMER = 0x02,
        INTERRUPT_TERMINAL = 0x03,
        INTERRUPT_SOFTWARE = 0x04
    };

    enum MemoryWriteDirection
    {
        MEMORY_INCREMENT = 0x01,
        MEMORY_DECREMENT = 0x02,
    };

    typedef uint32_t ADDRESS;
    typedef uint32_t DWORD;
    typedef uint16_t WORD;
    typedef uint8_t  BYTE;

    static constexpr ADDRESS ProgramStart = 0x40000000;
    static constexpr ADDRESS MemoryMappedRegisters = 0xFFFFFF00;
    static constexpr ADDRESS StackStart = 0x3FFFFFFF;

    static constexpr DWORD ByteSize = 8;
    static constexpr DWORD MemorySize = (1 << 31);
    
    static constexpr DWORD StackChunkSize = (1 << 8);
    static constexpr DWORD MemoryChunkSize = (1 << 16);

    static constexpr DWORD MaxProgramSize = MemoryMappedRegisters - ProgramStart;
    static constexpr DWORD MemoryChunkCount = MaxProgramSize / MemoryChunkSize;

    static constexpr DWORD MaxStackSize = StackStart - 0;
    static constexpr DWORD StackChunkCount = MaxStackSize / StackChunkSize;

    void Dump(BYTE* memory);
}