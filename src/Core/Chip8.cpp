#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <random>
#include "Chip8.h"
#include "InstructionExecutor.h"

static const std::array<uint8_t, 80> FONT_SET = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

Chip8::Chip8() : m_InstructionExecutor(*this) {
    m_LoadFontSet();
    Reset();
}

void Chip8::LoadChip8File(const std::string_view filename) {
    Reset();

    std::ifstream file(filename.data(), std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Error, unable to open the file: " << filename << std::endl;
        return;
    }

    const std::streamsize &size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > (4096 - 0x200)) {
        std::cerr << "Error, file size is too large: " << size << std::endl;
        return;
    }

    file.read(reinterpret_cast<char *>(&m_Memory[0x200]), size);
    file.close();

    std::cout << "File (" << filename << ") was loaded successfully!" << std::endl;
    std::cout << "Size (" << size << " bytes)" << std::endl;
}

void Chip8::Reset() {
    std::fill(m_Memory.begin() + 0x050, m_Memory.end(), 0);
    m_Registers.fill(0);
    m_Stack.fill(0);
    m_Keys.fill(0);

    for (auto &row: m_Display) {
        row.fill(false);
    }

    m_I = 0;
    m_PC = 0x200;
    m_SP = 0;

    m_DelayTimer = 0;
    m_SoundTimer = 0;

    m_DrawFlag = false;
    m_WaitingForKey = false;
    m_WaitingForRegister = 0;
}


void Chip8::Cycle() {
    if (m_WaitingForKey) {
        for (uint8_t i = 0; i < 16; i++) {
            if (m_Keys[i]) {
                m_Registers[m_WaitingForRegister] = i;
                m_WaitingForKey = false;
                m_PC += 2;
                return;
            }
        }
        return;
    }

    const uint16_t opcode = (m_Memory[m_PC] << 8) | m_Memory[m_PC + 1];
    m_InstructionExecutor.Execute(opcode);
}

void Chip8::UpdateTimers() {
    if (m_DelayTimer > 0) {
        m_DelayTimer--;
    }

    if (m_SoundTimer > 0) {
        m_SoundTimer--;
    }
}

void Chip8::SetKey(const uint8_t &key, const bool state) {
    if (key < 16) {
        m_Keys[key] = state;
    }
}

const std::array<std::array<bool, 64>, 32> &Chip8::GetDisplay() const {
    return m_Display;
}

bool Chip8::ShouldDraw() const {
    return m_DrawFlag;
}

void Chip8::ClearDrawFlag() {
    m_DrawFlag = false;
}

bool Chip8::ShouldBeep() const {
    return m_SoundTimer > 0;
}

uint16_t Chip8::GetI() const {
    return m_I;
}

uint16_t Chip8::GetPC() const {
    return m_PC;
}

uint16_t Chip8::GetSP() const {
    return m_SP;
}

uint16_t Chip8::GetRegisterAt(const uint8_t &index) const {
    return m_Registers[index];
}

uint16_t Chip8::GetDelayTimer() const {
    return m_DelayTimer;
}

uint16_t Chip8::GetSoundTimer() const {
    return m_SoundTimer;
}

void Chip8::m_LoadFontSet() {
    std::ranges::copy(FONT_SET, m_Memory.begin());
}
