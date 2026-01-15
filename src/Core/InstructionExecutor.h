#ifndef CHIP8_EMULATOR_INSTRUCTION_EXECUTOR_H
#define CHIP8_EMULATOR_INSTRUCTION_EXECUTOR_H

#include <cstdint>

class Chip8;

class InstructionExecutor {
public:
    explicit InstructionExecutor(Chip8 &chip8);

    void Execute(uint16_t opcode) const;

private:
    Chip8 &m_Chip8;
    mutable uint16_t m_CurrentInstruction{};

    void m_Handle0xxx(uint16_t opcode, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn) const;

    void m_Handle1xxx(uint16_t nnn) const;

    void m_Handle2xxx(uint16_t nnn) const;

    void m_Handle3xxx(uint8_t x, uint8_t nn) const;

    void m_Handle4xxx(uint8_t x, uint8_t nn) const;

    void m_Handle5xxx(uint8_t x, uint8_t y, uint8_t n) const;

    void m_Handle6xxx(uint8_t x, uint8_t nn) const;

    void m_Handle7xxx(uint8_t x, uint8_t nn) const;

    void m_Handle8xxx(uint8_t x, uint8_t y, uint8_t n) const;

    void m_Handle9xxx(uint8_t x, uint8_t y, uint8_t n) const;

    void m_HandleAxxx(uint16_t nnn) const;

    void m_HandleCxxx(uint8_t x, uint8_t nn) const;

    void m_HandleDxxx(uint8_t x, uint8_t y, uint8_t n) const;

    void m_HandleExxx(uint16_t opcode, uint8_t x) const;

    void m_HandleFxxx(uint8_t x, uint8_t nn) const;
};

#endif
