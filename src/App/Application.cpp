#include <algorithm>
#include <raylib.h>
#include "Application.h"

Application::Application() : m_Renderer(SCALE), m_TimerAccumulator(0.0f), m_SpeedMultiplier(1.0f) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHIP-8 Classic Emulator");
    SetTargetFPS(60);
    SetExitKey(0);
}

Application::~Application() {
    CloseWindow();
}

void Application::Run() {
    bool isFirstMenu = true;

    while (!WindowShouldClose()) {
        ROMSelector selector;
        const std::string romPath = selector.ShowMenu(!isFirstMenu);

        if (WindowShouldClose()) return;

        if (romPath.empty()) {
            if (isFirstMenu) return;
            continue;
        }

        isFirstMenu = false;

        m_Start(romPath);
    }
}

void Application::m_Start(std::string_view romPath) {
    m_Chip8.LoadChip8File(romPath);

    while (true) {
        if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) return;

        m_Update();
        m_Render();
    }
}

void Application::m_Update() {
    std::array<bool, 16> keys = {};
    m_Input.Update(keys);

    for (int i = 0; i < 16; i++) {
        m_Chip8.SetKey(i, keys[i]);
    }

    m_HandleSpeedControls();

    const int cyclesThisFrame = static_cast<int>(BASE_CYCLES_PER_FRAME * m_SpeedMultiplier);
    for (int i = 0; i < cyclesThisFrame; i++) {
        m_Chip8.Cycle();
    }

    m_TimerAccumulator += GetFrameTime();
    if (m_TimerAccumulator >= 1.0f / 60.0f) {
        m_Chip8.UpdateTimers();
        m_TimerAccumulator -= 1.0f / 60.0f;
    }

    m_Audio.Update(m_Chip8.ShouldBeep());
}

void Application::m_Render() const {
    BeginDrawing();
    m_Renderer.Clear();
    m_Renderer.Render(m_Chip8.GetDisplay());

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 10, GRAY);
    DrawText(TextFormat("Speed: %.1fx", m_SpeedMultiplier), 10, 25, 10, GRAY);
    DrawText("[-] Decrease  [+] Increase  |  ESC: Menu", 10, 40, 10, DARKGRAY);

    EndDrawing();
}

void Application::m_HandleSpeedControls() {
    if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        m_SpeedMultiplier = std::max(MIN_SPEED, m_SpeedMultiplier - SPEED_STEP);
    }

    if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
        m_SpeedMultiplier = std::min(MAX_SPEED, m_SpeedMultiplier + SPEED_STEP);
    }
}