#include "Renderer.h"

Renderer::Renderer(const uint8_t &scale) : m_DisplayScale(scale), m_EnabledColor(GREEN), m_DisabledColor(BLACK) {
}

Renderer::Renderer(const uint8_t &scale, const Color &enabledColor, const Color &disabledColor)
    : m_DisplayScale(scale), m_EnabledColor(enabledColor), m_DisabledColor(disabledColor) {
}

void Renderer::Render(const std::array<std::array<bool, 64>, 32> &display) const {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (display[y][x]) {
                DrawRectangle(
                    x * m_DisplayScale,
                    y * m_DisplayScale,
                    m_DisplayScale,
                    m_DisplayScale,
                    m_EnabledColor
                );
            }
        }
    }
}

void Renderer::SetEnabledColor(const Color &color) {
    m_EnabledColor = color;
}

void Renderer::SetDisabledColor(const Color &color) {
    m_DisabledColor = color;
}

void Renderer::Clear() const {
    ClearBackground(m_DisabledColor);
}
