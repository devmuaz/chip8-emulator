#ifndef CHIP8_EMULATOR_ROM_SELECTOR_H
#define CHIP8_EMULATOR_ROM_SELECTOR_H

#include <filesystem>
#include <string>
#include <vector>

class ROMSelector {
public:
    explicit ROMSelector();

    [[nodiscard]] std::string ShowMenu(bool allowReturnToMenu = false) const;

private:
    void m_ScanROMsDirectory();

    void m_RenderMenu(int selectedIndex, int scrollOffset) const;

    std::vector<std::string> m_ROMFiles;

    static constexpr int FONT_SIZE = 20;
    static constexpr int LINE_HEIGHT = 30;
    static constexpr int MENU_START_Y = 100;
    static constexpr int MAX_VISIBLE_ITEMS = 15;
    
    static std::filesystem::path m_GetROMsPath();
};

#endif
