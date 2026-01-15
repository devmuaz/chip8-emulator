#include "Audio.h"

Audio::Audio() {
    InitAudioDevice();
    m_GenerateBeepSound();
}

Audio::~Audio() {
    UnloadSound(m_BeepSound);
    CloseAudioDevice();
}

void Audio::Update(const bool shouldBeep) {
    if (shouldBeep) {
        if (!IsSoundPlaying(m_BeepSound)) {
            PlaySound(m_BeepSound);
            m_IsSoundPlaying = true;
        }
    } else {
        if (IsSoundPlaying(m_BeepSound)) {
            StopSound(m_BeepSound);
            m_IsSoundPlaying = false;
        }
    }
}

void Audio::m_GenerateBeepSound() {
    constexpr int sampleCount = static_cast<int>(SAMPLE_RATE * BEEP_DURATION);
    constexpr float period = static_cast<float>(SAMPLE_RATE) / BEEP_FREQUENCY;

    const auto samples = static_cast<short *>(MemAlloc(sampleCount * sizeof(short)));

    if (samples == nullptr) {
        TraceLog(LOG_WARNING, "Error, failed to allocate memory for beep sound!");
        return;
    }

    for (int i = 0; i < sampleCount; i++) {
        constexpr auto amplitude = 0.3f;
        const float sample = (static_cast<int>(i / period) % 2 == 0) ? amplitude : -amplitude;
        samples[i] = static_cast<short>(sample * 32767.0f);
    }

    const Wave wave = {
        .frameCount = sampleCount,
        .sampleRate = SAMPLE_RATE,
        .sampleSize = 16,
        .channels = 1,
        .data = samples
    };

    m_BeepSound = LoadSoundFromWave(wave);

    MemFree(samples);
    SetSoundVolume(m_BeepSound, 0.5f);
}
