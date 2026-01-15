#ifndef CHIP8_EMULATOR_INPUT_H
#define CHIP8_EMULATOR_INPUT_H

#include <array>
#include <raylib.h>

class Input {
public:
    explicit Input();

    void Update(std::array<bool, 16> &chipKeys) const;

private:
    std::array<KeyboardKey, 16> m_keyMap{};
};

#endif
