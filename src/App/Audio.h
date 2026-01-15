#ifndef CHIP8_EMULATOR_AUDIO_H
#define CHIP8_EMULATOR_AUDIO_H

#include <raylib.h>

class Audio {
public:
    explicit Audio();

    ~Audio();

    Audio(const Audio &) = delete;

    Audio &operator=(const Audio &) = delete;

    void Update(bool shouldBeep);

private:
    void m_GenerateBeepSound();

    Sound m_BeepSound{};
    bool m_IsSoundPlaying{false};

    static constexpr int SAMPLE_RATE = 44100;
    static constexpr float BEEP_FREQUENCY = 800.0f;
    static constexpr float BEEP_DURATION = 0.5f;
};

#endif
