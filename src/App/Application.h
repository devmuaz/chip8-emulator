#ifndef CHIP8_EMULATOR_APPLICATION_H
#define CHIP8_EMULATOR_APPLICATION_H

#include "Audio.h"
#include "Chip8.h"
#include "Input.h"
#include "ROMSelector.h"
#include "Renderer.h"

class Application {
public:
    explicit Application();

    ~Application();

    void Run();

private:
    void m_Start(std::string_view romPath);

    void m_Update();

    void m_Render() const;

    void m_HandleSpeedControls();

    Chip8 m_Chip8;
    Renderer m_Renderer;
    Input m_Input;
    Audio m_Audio;

    float m_TimerAccumulator;
    float m_SpeedMultiplier;

    static constexpr int CHIP8_WIDTH = 64;
    static constexpr int CHIP8_HEIGHT = 32;
    static constexpr int SCALE = 20;
    static constexpr int WINDOW_WIDTH = CHIP8_WIDTH * SCALE;
    static constexpr int WINDOW_HEIGHT = CHIP8_HEIGHT * SCALE;

    static constexpr float BASE_CYCLES_PER_FRAME = 10.0f;
    static constexpr float MIN_SPEED = 0.5f;
    static constexpr float MAX_SPEED = 10.0f;
    static constexpr float SPEED_STEP = 0.5f;
};

#endif
