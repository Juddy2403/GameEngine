#include "DerivedSoundSystems.h"

#include <fstream>

#include "SDL_mixer.h"
#include <iostream>

using namespace GameEngine;

class SdlSoundSystem::SDLAudioClip final
{
public:
    SDLAudioClip(const SDLAudioClip& other) = delete;
    SDLAudioClip(SDLAudioClip&& other) noexcept = delete;
    SDLAudioClip& operator=(const SDLAudioClip& other) = delete;
    SDLAudioClip& operator=(SDLAudioClip&& other) noexcept = delete;
    explicit SDLAudioClip(const std::string& filePath) : m_FilePath(filePath) {}
    ~SDLAudioClip()
    {
        if (m_pSound != nullptr)
        {
            Mix_FreeChunk(m_pSound);
            m_pSound = nullptr;
        }
    }
    void Load()
    {
        m_pSound = Mix_LoadWAV(m_FilePath.c_str());
        if (m_pSound == nullptr)
        {
            std::cerr << "Failed to load sound: " << m_FilePath << '\n';
        }
    }
    [[nodiscard]] bool IsLoaded() const
    {
        return m_pSound != nullptr;
    }
    void SetVolume(const int volume) const
    {
        Mix_VolumeChunk(m_pSound, volume);
    }
    void Play() const
    {
        Mix_PlayChannel(-1, m_pSound, 0);
    }
    static void OpenAudio()
    {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            std::cerr << "Failed to initialize sound system.\n";
        }
        else
        {
            std::cout << "Sound system initialized.\n";
        }
    }
    static void CloseAudio()
    {
        Mix_CloseAudio();
    }
private:
    std::string m_FilePath{};
    Mix_Chunk* m_pSound{};
};

SdlSoundSystem::SdlSoundSystem() 
{
    SDLAudioClip::OpenAudio();
    m_WorkerThread = std::thread(&SdlSoundSystem::ProcessQueue, this);
}
SdlSoundSystem::~SdlSoundSystem()
{
    SDLAudioClip::CloseAudio();
    // Signal the worker thread to stop and wait for it to finish.
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_IsRunning = false;
    }
    m_ConditionVariable.notify_one();
    m_WorkerThread.join();
}
void SdlSoundSystem::FillSoundPaths(const std::string& fileSource)
{
    std::ifstream file(fileSource);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << fileSource << '\n';
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        m_SoundFilePaths.emplace_back(line);
    }
    file.close();
    for (const auto& path : m_SoundFilePaths)
    {
        m_AudioClips.emplace_back(std::make_unique<SDLAudioClip>(path));
    }
}

void SdlSoundSystem::PlaySound(const SoundId id, const int volume)
{
    if ((m_QueueTail + 1) % maxPending == m_QueueHead)
    {
        std::cerr << "Too many pending sounds\n";
        return;
    }
    for (int i = m_QueueHead; i != m_QueueTail; i = (i + 1) % maxPending)
    {
        if (m_PendingSounds[i].id == id)
        {
            // Use the larger of the two volumes.
            m_PendingSounds[i].volume = std::max(volume, m_PendingSounds[i].volume);

            // Don't need to enqueue.
            return;
        }
    }
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_PendingSounds[m_QueueTail] = { id,volume };
    m_QueueTail = (m_QueueTail + 1) % maxPending;
    m_ConditionVariable.notify_one();
}
int SdlSoundSystem::GetPending() const
{
    return abs(m_QueueHead - m_QueueTail);
}

void SdlSoundSystem::ProcessQueue()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if(!GetPending())
        m_ConditionVariable.wait(lock, [this] { return !m_IsRunning || GetPending(); });

        if (!m_IsRunning && GetPending() == 0)
        {
            break;
        }

        // Process queue
        if (m_QueueHead == m_QueueTail) return;
        auto audioClip = m_AudioClips[m_PendingSounds[m_QueueHead].id].get();
        if (!audioClip->IsLoaded())
            audioClip->Load();
        audioClip->SetVolume(m_PendingSounds[m_QueueHead].volume);
        audioClip->Play();
        m_QueueHead = (m_QueueHead + 1) % maxPending;
    }
}
void LoggingSoundSystem::PlaySound(const SoundId id, const int volume)
{
    m_RealSs->PlaySound(id, volume);
    std::cout << "Playing sound with id: " << id << " at volume " << volume << '\n';
}
void LoggingSoundSystem::FillSoundPaths(const std::string& fileSource)
{
    m_RealSs->FillSoundPaths(fileSource);
    std::cout << "Filling sound paths from " << fileSource << "\n";
}

