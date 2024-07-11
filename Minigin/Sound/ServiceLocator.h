#pragma once
#include <memory>
#include "DerivedSoundSystems.h"

namespace GameEngine
{
    class ISoundSystem;
    class ServiceLocator final
    {
        static std::unique_ptr<ISoundSystem> m_SsInstance;
    public:
        static ISoundSystem& GetSoundSystem() { return *m_SsInstance; }
        static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
        {
            m_SsInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
        }
    };
    
}

