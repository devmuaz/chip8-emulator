#include "Input.h"

Input::Input() {
    // CHIP-8 keypad:     Keyboard Layout:
    // 1 2 3 C            1 2 3 4
    // 4 5 6 D             Q W E R
    // 7 8 9 E              A S D F
    // A 0 B F               Z X C V

    m_keyMap[0x1] = KEY_ONE;
    m_keyMap[0x2] = KEY_TWO;
    m_keyMap[0x3] = KEY_THREE;
    m_keyMap[0xC] = KEY_FOUR;

    m_keyMap[0x4] = KEY_Q;
    m_keyMap[0x5] = KEY_W;
    m_keyMap[0x6] = KEY_E;
    m_keyMap[0xD] = KEY_R;

    m_keyMap[0x7] = KEY_A;
    m_keyMap[0x8] = KEY_S;
    m_keyMap[0x9] = KEY_D;
    m_keyMap[0xE] = KEY_F;

    m_keyMap[0xA] = KEY_Z;
    m_keyMap[0x0] = KEY_X;
    m_keyMap[0xB] = KEY_C;
    m_keyMap[0xF] = KEY_V;
}

void Input::Update(std::array<bool, 16> &chipKeys) const {
    for (int i = 0; i < 16; i++) {
        chipKeys[i] = IsKeyDown(m_keyMap[i]);
    }
}
