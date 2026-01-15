#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <array>
#include <string>
#include "InstructionExecutor.h"

class Chip8 {
    friend class InstructionExecutor;

public:
    explicit Chip8();

    void LoadChip8File(std::string_view filename);

    void Reset();

    void Cycle();

    void UpdateTimers();

    void SetKey(const uint8_t &key, bool state);

    [[nodiscard]] const std::array<std::array<bool, 64>, 32> &GetDisplay() const;

    [[nodiscard]] bool ShouldDraw() const;

    void ClearDrawFlag();

    [[nodiscard]] bool ShouldBeep() const;

    [[nodiscard]] uint16_t GetI() const;

    [[nodiscard]] uint16_t GetPC() const;

    [[nodiscard]] uint16_t GetSP() const;

    [[nodiscard]] uint16_t GetRegisterAt(const uint8_t &index) const;

    [[nodiscard]] uint16_t GetDelayTimer() const;

    [[nodiscard]] uint16_t GetSoundTimer() const;

private:
    void m_LoadFontSet();

    std::array<uint8_t, 16> m_Registers{};
    std::array<uint8_t, 4096> m_Memory{};

    uint16_t m_I{};
    uint16_t m_PC{};
    uint16_t m_SP{};
    std::array<uint16_t, 16> m_Stack{};

    uint8_t m_DelayTimer{};
    uint8_t m_SoundTimer{};

    std::array<uint8_t, 16> m_Keys{};
    std::array<std::array<bool, 64>, 32> m_Display{};
    bool m_DrawFlag{};

    bool m_WaitingForKey{};
    uint8_t m_WaitingForRegister{};

    InstructionExecutor m_InstructionExecutor;
};

#endif
