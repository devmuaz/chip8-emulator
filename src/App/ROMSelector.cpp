#include <algorithm>
#include <filesystem>
#include <iostream>
#include <raylib.h>
#include "ROMSelector.h"

ROMSelector::ROMSelector() {
    m_ScanROMsDirectory();
}

std::string ROMSelector::ShowMenu(const bool allowReturnToMenu) const {
    if (m_ROMFiles.empty()) {
        return "";
    }

    int selectedIndex = 0;
    int scrollOffset = 0;
    bool selectionMade = false;
    std::string selectedROM;

    while (!WindowShouldClose() && !selectionMade) {
        if (IsKeyPressed(KEY_UP)) {
            selectedIndex = (selectedIndex - 1 + static_cast<int>(m_ROMFiles.size()))
                            % static_cast<int>(m_ROMFiles.size());

            if (selectedIndex < scrollOffset) {
                scrollOffset = selectedIndex;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedIndex = (selectedIndex + 1) % static_cast<int>(m_ROMFiles.size());

            if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
                scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
            }
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            // Find the ROMs directory path (cross-platform)
            std::filesystem::path romsPath = m_GetROMsPath();
            selectedROM = (romsPath / m_ROMFiles[selectedIndex]).string();
            selectionMade = true;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (allowReturnToMenu) {
                int waitFrames = 0;
                while ((IsKeyDown(KEY_ESCAPE) || IsKeyPressed(KEY_ESCAPE)) && waitFrames < 30) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    EndDrawing();
                    waitFrames++;
                }
                return "";
            } else {
                return "";
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const auto title = "Select ROM (.ch8)";
        const int titleWidth = MeasureText(title, FONT_SIZE + 10);
        DrawText(title, (GetScreenWidth() - titleWidth) / 2, 30, FONT_SIZE + 10, GREEN);

        const auto instructions = "Arrow Keys: Navigate  |  Enter/Space: Select  |  ESC: Cancel";
        const int instructionsWidth = MeasureText(instructions, FONT_SIZE - 5);
        DrawText(instructions, (GetScreenWidth() - instructionsWidth) / 2, 70, FONT_SIZE - 5, GRAY);

        m_RenderMenu(selectedIndex, scrollOffset);

        EndDrawing();
    }

    return selectedROM;
}

void ROMSelector::m_ScanROMsDirectory() {
    m_ROMFiles.clear();

    std::vector<std::filesystem::path> possiblePaths = {
        "roms",
        "../roms",
        "../../roms",
    };

#ifdef _WIN32
    possiblePaths.emplace_back("cmake-build-release/roms");
    possiblePaths.emplace_back("cmake-build-debug/roms");
    possiblePaths.emplace_back("build/roms");
#else
    possiblePaths.emplace_back("cmake-build-release/roms");
    possiblePaths.emplace_back("cmake-build-debug/roms");
    possiblePaths.emplace_back("build/roms");
#endif

    std::filesystem::path romsPath;
    bool found = false;

    for (const auto &path: possiblePaths) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            romsPath = path;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Warning: \"roms\" directory was not found in any expected location!" << std::endl;
        return;
    }

    std::cout << "Loading ROMs from: " << romsPath.string() << std::endl;

    for (const auto &entry: std::filesystem::directory_iterator(romsPath)) {
        if (entry.is_regular_file()) {
            const std::string &filename = entry.path().filename().string();
            if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".ch8") {
                m_ROMFiles.push_back(filename);
            }
        }
    }

    std::ranges::sort(m_ROMFiles);
}

void ROMSelector::m_RenderMenu(const int selectedIndex, const int scrollOffset) const {
    const int startX = GetScreenWidth() / 2 - 200;
    int y = MENU_START_Y;

    const int totalItems = static_cast<int>(m_ROMFiles.size());
    const int visibleStart = scrollOffset;
    const int visibleEnd = std::min(scrollOffset + MAX_VISIBLE_ITEMS, totalItems);
    const int visibleCount = visibleEnd - visibleStart;

    if (totalItems > MAX_VISIBLE_ITEMS) {
        if (scrollOffset > 0) {
            DrawText("UP", startX - 100, MENU_START_Y - 5, FONT_SIZE, GRAY);
        }

        if (scrollOffset + MAX_VISIBLE_ITEMS < totalItems) {
            constexpr int bottomY = MENU_START_Y + (MAX_VISIBLE_ITEMS * LINE_HEIGHT);
            DrawText("DOWN", startX - 100, bottomY, FONT_SIZE, GRAY);
        }
    }

    for (int i = visibleStart; i < visibleEnd; i++) {
        const bool isSelected = (i == selectedIndex);
        const Color textColor = isSelected ? BLACK : GREEN;
        const Color bgColor = isSelected ? GREEN : BLACK;

        if (isSelected) {
            DrawRectangle(startX - 10, y - 5, 420, LINE_HEIGHT, bgColor);
        }

        const auto prefix = isSelected ? "> " : "  ";
        const auto postfix = isSelected ? " <" : "";
        DrawText(TextFormat("%s%s%s", prefix, m_ROMFiles[i].c_str(), postfix), startX, y, FONT_SIZE, textColor);

        y += LINE_HEIGHT;
    }

    if (!m_ROMFiles.empty()) {
        const int bottomY = MENU_START_Y + (visibleCount * LINE_HEIGHT) + 10;

        const auto countText = TextFormat("%zu ROM(s) found", m_ROMFiles.size());
        DrawText(countText, startX, bottomY, FONT_SIZE - 5, GRAY);

        if (totalItems > MAX_VISIBLE_ITEMS) {
            const auto positionText = TextFormat("Showing %d-%d of %d", visibleStart + 1, visibleEnd, totalItems);
            DrawText(positionText, startX, bottomY + 20, FONT_SIZE - 5, DARKGRAY);
        }
    }
}

std::filesystem::path ROMSelector::m_GetROMsPath() {
    std::vector<std::filesystem::path> possiblePaths = {
        "roms",
        "../roms",
        "../../roms",
    };


#ifdef _WIN32
    possiblePaths.emplace_back("cmake-build-release/roms");
    possiblePaths.emplace_back("cmake-build-debug/roms");
    possiblePaths.emplace_back("build/roms");
#else
    possiblePaths.emplace_back("cmake-build-release/roms");
    possiblePaths.emplace_back("cmake-build-debug/roms");
    possiblePaths.emplace_back("build/roms");
#endif

    for (const auto &path: possiblePaths) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            return path;
        }
    }

    return "roms";
}
