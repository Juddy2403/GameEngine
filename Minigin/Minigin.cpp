//#include <steam_api.h>
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"

#include "Managers/InputManager.h"
#include "Managers/SceneManager.h"
#include "Renderable/Renderer.h"
#include "Managers/ResourceManager.h"
#include "Sound/DerivedSoundSystems.h"
#include "Managers/TimeManager.h"

SDL_Window* g_window{};
constexpr float g_TargetFPS{1/160.f};

void PrintSDLVersion()
{
    SDL_version version{};
    SDL_VERSION(&version);
    printf("We compiled against SDL version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    SDL_GetVersion(&version);
    printf("We are linking against SDL version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_IMAGE_VERSION(&version);
    printf("We compiled against SDL_image version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *IMG_Linked_Version();
    printf("We are linking against SDL_image version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_TTF_VERSION(&version)
    printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *TTF_Linked_Version();
    printf("We are linking against SDL_ttf version %u.%u.%u.\n",
        version.major, version.minor, version.patch);
}

GameEngine::Minigin::Minigin(const std::string& dataPath)
{
    PrintSDLVersion();
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        g_WindowRect.x,
        g_WindowRect.y,
        g_WindowRect.w,
        g_WindowRect.h,
        SDL_WINDOW_OPENGL
    );
    if (g_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    Renderer::GetInstance().Init(g_window);

    ResourceManager::GetInstance().Init(dataPath);
}

GameEngine::Minigin::~Minigin()
{
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void GameEngine::Minigin::Run(const std::function<void()>& load)
{
    //SDL_GL_SetSwapInterval(1); //for a steady framerate of 160 using Vsync

    load();

    auto& renderer = Renderer::GetInstance();
    auto& sceneManager = SceneManager::GetInstance();
    auto& input = InputManager::GetInstance();
    auto& time = TimeManager::GetInstance();

    bool doContinue = true;
    while (doContinue)
    {
        float frameStartTime = SDL_GetTicks() / 1000.0f; // Get the current time in seconds
        
        time.Update();
        doContinue = input.ProcessInput();

        sceneManager.Update();
        renderer.Render();

        float frameEndTime = SDL_GetTicks() / 1000.0f; // Get the current time in seconds
        float frameTime = frameEndTime - frameStartTime; // Calculate the elapsed time 

        if (frameTime < g_TargetFPS)
        {
            SDL_Delay(static_cast<Uint32>((g_TargetFPS - frameTime) * 1000)); // Delay for the remaining time
        }
    }
}
