#pragma once
#include <string>

namespace GameEngine
{
    using SoundId = unsigned short;
    struct SoundInfo
    {
        SoundId id;
        int volume;
    };
    class ISoundSystem
    {
    public:
        ISoundSystem() = default;
        ISoundSystem(const ISoundSystem& other) = delete;
        ISoundSystem(ISoundSystem&& other) noexcept = delete;
        ISoundSystem& operator=(const ISoundSystem& other) = delete;
        ISoundSystem& operator=(ISoundSystem&& other) noexcept = delete;
        virtual ~ISoundSystem() = default;

        virtual void FillSoundPaths(const std::string& fileSource) = 0;
        virtual void PlaySound(const SoundId id, const int volume) = 0;

    };

}
