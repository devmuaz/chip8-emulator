#include <algorithm>
#include <iostream>
#include <vector>
#include <raylib.h>
#include "Application.h"

Application::Application() : m_Renderer(SCALE), m_UsingCustomFont(false), m_TimerAccumulator(0.0f),
                             m_SpeedMultiplier(1.0f) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHIP-8 Classic Emulator");
    SetTargetFPS(60);
    SetExitKey(0);
    m_LoadFont();
}

Application::~Application() {
    if (m_UsingCustomFont) {
        UnloadFont(m_Font);
    }
    CloseWindow();
}

void Application::Run() {
    bool isFirstMenu = true;

    while (!WindowShouldClose()) {
        ROMSelector selector;
        const std::string &romPath = selector.ShowMenu(!isFirstMenu, m_Font, m_UsingCustomFont);

        if (WindowShouldClose()) return;

        if (romPath.empty()) {
            if (isFirstMenu) return;
            continue;
        }

        isFirstMenu = false;

        m_Start(romPath);
    }
}

void Application::m_Start(const std::string_view rom) {
    m_Chip8.LoadChip8File(rom);

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

    if (m_UsingCustomFont) {
        DrawTextEx(m_Font, TextFormat("FPS: %d", GetFPS()), {10.0f, 10.0f}, 10.0f, 0.0f, GRAY);
        DrawTextEx(m_Font, TextFormat("Speed: %.1fx", m_SpeedMultiplier), {10.0f, 25.0f}, 10.0f, 0.0f, GRAY);
        DrawTextEx(m_Font, "[-] Decrease  [+] Increase  |  ESC: Menu", {10.0f, 40.0f}, 10.0f, 0.0f, DARKGRAY);
    } else {
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 10, GRAY);
        DrawText(TextFormat("Speed: %.1fx", m_SpeedMultiplier), 10, 25, 10, GRAY);
        DrawText("[-] Decrease  [+] Increase  |  ESC: Menu", 10, 40, 10, DARKGRAY);
    }

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

void Application::m_LoadFont() {
    constexpr std::string_view fontPath = "fonts/PressStart2P-Regular.ttf"; // Change this to your font path

    if (FileExists(fontPath.data())) {
        m_Font = LoadFont(fontPath.data());
        if (m_Font.texture.id != 0) {
            m_UsingCustomFont = true;
            std::cout << "Loaded custom font from: " << fontPath << std::endl;
            return;
        }
    }

    m_UsingCustomFont = false;
    std::cout << "Using default raylib font (no custom font file found)" << std::endl;
}
