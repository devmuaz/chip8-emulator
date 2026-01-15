#ifndef CHIP8_EMULATOR_RENDERER_H
#define CHIP8_EMULATOR_RENDERER_H

#include <array>
#include <raylib.h>

class Renderer {
public:
    explicit Renderer(const uint8_t &scale);
    explicit Renderer(const uint8_t &scale, const Color &enabledColor, const Color &disabledColor);

    void Render(const std::array<std::array<bool, 64>, 32> &display) const;

    void SetEnabledColor(const Color &color);

    void SetDisabledColor(const Color &color);

    void Clear() const;

private:
    uint8_t m_DisplayScale;
    Color m_EnabledColor;
    Color m_DisabledColor;
};

#endif
