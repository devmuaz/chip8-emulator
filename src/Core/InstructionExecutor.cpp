#include <random>
#include "InstructionExecutor.h"
#include "Chip8.h"

InstructionExecutor::InstructionExecutor(Chip8 &chip8) : m_Chip8(chip8) {
}

void InstructionExecutor::Execute(const uint16_t opcode) const {
    m_CurrentInstruction = opcode;
    const uint8_t x = (opcode & 0x0F00) >> 8;
    const uint8_t y = (opcode & 0x00F0) >> 4;
    const uint8_t n = opcode & 0x000F;
    const uint8_t nn = opcode & 0x00FF;
    const uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000) {
        case 0x0000:
            m_Handle0xxx(opcode, x, y, n, nn, nnn);
            break;
        case 0x1000:
            m_Handle1xxx(nnn);
            break;
        case 0x2000:
            m_Handle2xxx(nnn);
            break;
        case 0x3000:
            m_Handle3xxx(x, nn);
            break;
        case 0x4000:
            m_Handle4xxx(x, nn);
            break;
        case 0x5000:
            m_Handle5xxx(x, y, n);
            break;
        case 0x6000:
            m_Handle6xxx(x, nn);
            break;
        case 0x7000:
            m_Handle7xxx(x, nn);
            break;
        case 0x8000:
            m_Handle8xxx(x, y, n);
            break;
        case 0x9000:
            m_Handle9xxx(x, y, n);
            break;
        case 0xA000:
            m_HandleAxxx(nnn);
            break;
        case 0xC000:
            m_HandleCxxx(x, nn);
            break;
        case 0xD000:
            m_HandleDxxx(x, y, n);
            break;
        case 0xE000:
            m_HandleExxx(opcode, x);
            break;
        case 0xF000:
            m_HandleFxxx(x, nn);
            break;
        default:
            std::printf("Unimplemented opcode: 0x%04X\n", opcode);
            m_Chip8.m_PC += 2;
            break;
    }
}

void InstructionExecutor::m_Handle0xxx(const uint16_t opcode, uint8_t x, uint8_t y, uint8_t n, uint8_t nn,
                                       uint16_t nnn) const {
    if (opcode == 0x00E0) {
        for (auto &row: m_Chip8.m_Display) {
            row.fill(false);
        }
        m_Chip8.m_DrawFlag = true;
        m_Chip8.m_PC += 2;
    } else if (opcode == 0x00EE) {
        m_Chip8.m_SP--;
        m_Chip8.m_PC = m_Chip8.m_Stack[m_Chip8.m_SP];
        m_Chip8.m_PC += 2;
    } else {
        std::printf("Unknown opcode: 0x%04X\n", opcode);
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_Handle1xxx(const uint16_t nnn) const {
    m_Chip8.m_PC = nnn;
}

void InstructionExecutor::m_Handle2xxx(const uint16_t nnn) const {
    m_Chip8.m_Stack[m_Chip8.m_SP] = m_Chip8.m_PC;
    m_Chip8.m_SP++;
    m_Chip8.m_PC = nnn;
}

void InstructionExecutor::m_Handle3xxx(const uint8_t x, const uint8_t nn) const {
    if (m_Chip8.m_Registers[x] == nn) {
        m_Chip8.m_PC += 4;
    } else {
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_Handle4xxx(const uint8_t x, const uint8_t nn) const {
    if (m_Chip8.m_Registers[x] != nn) {
        m_Chip8.m_PC += 4;
    } else {
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_Handle5xxx(const uint8_t x, const uint8_t y, const uint8_t n) const {
    if (n == 0) {
        if (m_Chip8.m_Registers[x] == m_Chip8.m_Registers[y]) {
            m_Chip8.m_PC += 4;
        } else {
            m_Chip8.m_PC += 2;
        }
    } else {
        std::printf("Unknown opcode: 0x%04X\n", (0x5000 | (x << 8) | (y << 4) | n));
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_Handle6xxx(const uint8_t x, const uint8_t nn) const {
    m_Chip8.m_Registers[x] = nn;
    m_Chip8.m_PC += 2;
}

void InstructionExecutor::m_Handle7xxx(const uint8_t x, const uint8_t nn) const {
    m_Chip8.m_Registers[x] += nn;
    m_Chip8.m_PC += 2;
}

void InstructionExecutor::m_Handle8xxx(const uint8_t x, const uint8_t y, const uint8_t n) const {
    switch (n) {
        case 0x0:
            m_Chip8.m_Registers[x] = m_Chip8.m_Registers[y];
            m_Chip8.m_PC += 2;
            break;

        case 0x1:
            m_Chip8.m_Registers[x] |= m_Chip8.m_Registers[y];
            m_Chip8.m_PC += 2;
            break;

        case 0x2:
            m_Chip8.m_Registers[x] &= m_Chip8.m_Registers[y];
            m_Chip8.m_PC += 2;
            break;

        case 0x3:
            m_Chip8.m_Registers[x] ^= m_Chip8.m_Registers[y];
            m_Chip8.m_PC += 2;
            break;

        case 0x4: {
            const uint16_t sum = m_Chip8.m_Registers[x] + m_Chip8.m_Registers[y];
            m_Chip8.m_Registers[0xF] = (sum > 255) ? 1 : 0;
            m_Chip8.m_Registers[x] = sum & 0xFF;
            m_Chip8.m_PC += 2;
        }
        break;

        case 0x5:
            m_Chip8.m_Registers[0xF] = (m_Chip8.m_Registers[x] >= m_Chip8.m_Registers[y]) ? 1 : 0;
            m_Chip8.m_Registers[x] -= m_Chip8.m_Registers[y];
            m_Chip8.m_PC += 2;
            break;

        case 0x6:
            m_Chip8.m_Registers[0xF] = m_Chip8.m_Registers[x] & 0x01;
            m_Chip8.m_Registers[x] >>= 1;
            m_Chip8.m_PC += 2;
            break;

        case 0x7:
            m_Chip8.m_Registers[0xF] = (m_Chip8.m_Registers[y] >= m_Chip8.m_Registers[x]) ? 1 : 0;
            m_Chip8.m_Registers[x] = m_Chip8.m_Registers[y] - m_Chip8.m_Registers[x];
            m_Chip8.m_PC += 2;
            break;

        case 0xE:
            m_Chip8.m_Registers[0xF] = (m_Chip8.m_Registers[x] & 0x80) >> 7;
            m_Chip8.m_Registers[x] <<= 1;
            m_Chip8.m_Registers[x] &= 0xFF;
            m_Chip8.m_PC += 2;
            break;

        default:
            std::printf("Unknown opcode: 0x%04X\n", (0x8000 | (x << 8) | (y << 4) | n));
            m_Chip8.m_PC += 2;
            break;
    }
}

void InstructionExecutor::m_Handle9xxx(const uint8_t x, const uint8_t y, const uint8_t n) const {
    if (n == 0) {
        if (m_Chip8.m_Registers[x] != m_Chip8.m_Registers[y]) {
            m_Chip8.m_PC += 4;
        } else {
            m_Chip8.m_PC += 2;
        }
    } else {
        std::printf("Unknown opcode: 0x%04X\n", (0x9000 | (x << 8) | (y << 4) | n));
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_HandleAxxx(const uint16_t nnn) const {
    m_Chip8.m_I = nnn;
    m_Chip8.m_PC += 2;
}

void InstructionExecutor::m_HandleCxxx(const uint8_t x, const uint8_t nn) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);
    m_Chip8.m_Registers[x] = dis(gen) & nn;
    m_Chip8.m_PC += 2;
}

void InstructionExecutor::m_HandleDxxx(const uint8_t x, const uint8_t y, const uint8_t n) const {
    const uint8_t xPos = m_Chip8.m_Registers[x] % 64;
    const uint8_t yPos = m_Chip8.m_Registers[y] % 32;
    const uint8_t height = n;

    m_Chip8.m_Registers[0xF] = 0;

    for (uint8_t row = 0; row < height; row++) {
        const uint8_t spriteByte = m_Chip8.m_Memory[m_Chip8.m_I + row];

        for (uint8_t col = 0; col < 8; col++) {
            if ((spriteByte & (0x80 >> col)) != 0) {
                const uint8_t pixelX = (xPos + col) % 64;
                const uint8_t pixelY = (yPos + row) % 32;

                if (m_Chip8.m_Display[pixelY][pixelX]) {
                    m_Chip8.m_Registers[0xF] = 1;
                }

                m_Chip8.m_Display[pixelY][pixelX] ^= true;
            }
        }
    }

    m_Chip8.m_DrawFlag = true;
    m_Chip8.m_PC += 2;
}

void InstructionExecutor::m_HandleExxx(const uint16_t opcode, const uint8_t x) const {
    if ((opcode & 0x00FF) == 0x009E) {
        if (m_Chip8.m_Keys[m_Chip8.m_Registers[x]]) {
            m_Chip8.m_PC += 4;
        } else {
            m_Chip8.m_PC += 2;
        }
    } else if ((opcode & 0x00FF) == 0x00A1) {
        if (!m_Chip8.m_Keys[m_Chip8.m_Registers[x]]) {
            m_Chip8.m_PC += 4;
        } else {
            m_Chip8.m_PC += 2;
        }
    } else {
        std::printf("Unknown opcode: 0x%04X\n", opcode);
        m_Chip8.m_PC += 2;
    }
}

void InstructionExecutor::m_HandleFxxx(const uint8_t x, const uint8_t nn) const {
    switch (nn) {
        case 0x07:
            m_Chip8.m_Registers[x] = m_Chip8.m_DelayTimer;
            m_Chip8.m_PC += 2;
            break;

        case 0x0A:
            m_Chip8.m_WaitingForKey = true;
            m_Chip8.m_WaitingForRegister = x;
            break;

        case 0x15:
            m_Chip8.m_DelayTimer = m_Chip8.m_Registers[x];
            m_Chip8.m_PC += 2;
            break;

        case 0x18:
            m_Chip8.m_SoundTimer = m_Chip8.m_Registers[x];
            m_Chip8.m_PC += 2;
            break;

        case 0x1E:
            m_Chip8.m_I += m_Chip8.m_Registers[x];
            m_Chip8.m_PC += 2;
            break;

        case 0x29:
            m_Chip8.m_I = m_Chip8.m_Registers[x] * 5;
            m_Chip8.m_PC += 2;
            break;

        case 0x33: {
            const uint8_t value = m_Chip8.m_Registers[x];
            m_Chip8.m_Memory[m_Chip8.m_I] = value / 100;
            m_Chip8.m_Memory[m_Chip8.m_I + 1] = (value / 10) % 10;
            m_Chip8.m_Memory[m_Chip8.m_I + 2] = value % 10;
            m_Chip8.m_PC += 2;
        }
        break;

        case 0x55:
            for (uint8_t i = 0; i <= x; i++) {
                m_Chip8.m_Memory[m_Chip8.m_I + i] = m_Chip8.m_Registers[i];
            }
            m_Chip8.m_PC += 2;
            break;

        case 0x65:
            for (uint8_t i = 0; i <= x; i++) {
                m_Chip8.m_Registers[i] = m_Chip8.m_Memory[m_Chip8.m_I + i];
            }
            m_Chip8.m_PC += 2;
            break;

        default:
            std::printf("Unknown opcode: 0x%04X\n", (0xF000 | (x << 8) | nn));
            m_Chip8.m_PC += 2;
            break;
    }
}
