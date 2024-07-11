#include "ISoundSystem.h"
#include "ServiceLocator.h"

using namespace GameEngine;
std::unique_ptr<ISoundSystem> ServiceLocator::m_SsInstance{ std::make_unique<NullSoundSystem>() };
