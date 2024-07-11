#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "ISoundSystem.h"

namespace GameEngine
{
    class SdlSoundSystem final : public ISoundSystem
    {
    public:
        SdlSoundSystem();
        SdlSoundSystem(const SdlSoundSystem& other) = delete;
        SdlSoundSystem(SdlSoundSystem&& other) noexcept = delete;
        SdlSoundSystem& operator=(const SdlSoundSystem& other) = delete;
        SdlSoundSystem& operator=(SdlSoundSystem&& other) noexcept = delete;
        virtual ~SdlSoundSystem() override;

        virtual void FillSoundPaths(const std::string& fileSource) override;
        virtual void PlaySound(const SoundId id, const int volume) override;
    private:
        int GetPending() const;

        class SDLAudioClip;
        std::vector<std::unique_ptr<SDLAudioClip>> m_AudioClips;
        static constexpr int maxPending = 16;
        SoundInfo m_PendingSounds[maxPending]{};
        int m_QueueHead{};
        int m_QueueTail{};
        std::vector<std::string> m_SoundFilePaths;
        std::mutex m_Mutex;
        std::condition_variable m_ConditionVariable;
        std::thread m_WorkerThread;
        bool m_IsRunning{true};
        void ProcessQueue();
    };
    class LoggingSoundSystem final : public ISoundSystem
    {
        std::unique_ptr<ISoundSystem> m_RealSs;
    public:
        LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& ss) : m_RealSs(std::move(ss)) {}
        LoggingSoundSystem(const LoggingSoundSystem& other) = delete;
        LoggingSoundSystem(LoggingSoundSystem&& other) noexcept = delete;
        LoggingSoundSystem& operator=(const LoggingSoundSystem& other) = delete;
        LoggingSoundSystem& operator=(LoggingSoundSystem&& other) noexcept = delete;
        virtual ~LoggingSoundSystem() override = default;

        void PlaySound(const SoundId id, const int volume) override;
        virtual void FillSoundPaths(const std::string& fileSource) override;
    };
    class NullSoundSystem final : public ISoundSystem
    {
    public:
        virtual void PlaySound(const SoundId id, const int volume) override {id;volume;}
        virtual void FillSoundPaths(const std::string& fileSource) override {fileSource;}
    };

}
