#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Managers/Singleton.h"
namespace GameEngine
{
    enum class ControllerInputKey;
}
namespace GameEngine
{
    enum class KeyboardInputKey;
    class GameObject;
}
enum class SceneId;
enum class GameMode;
namespace GameEngine
{
    class Scene;
}
class Galaga final : public GameEngine::Singleton<Galaga>
{
public:
    ~Galaga() = default;
    Galaga(const Galaga& other) = delete;
    Galaga(Galaga&& other) noexcept = delete;
    Galaga& operator=(const Galaga& other) = delete;
    Galaga& operator=(Galaga&& other) noexcept = delete;

    void LoadStartScene();
    void LevelCleared();
    void SetPlayerName(const std::string& name);
    void GameLost();
    void ChooseName();
    void ChangeScene(SceneId sceneId, std::unique_ptr<GameEngine::Scene>&& scene);
    void SetGameMode(GameMode mode);
    GameMode GetGameMode() const { return m_CurrentGameMode; }
    static constexpr int baseVolume = 50;
    static int volume;
    GameEngine::GameObject* m_pPlayer;
private:
    friend class Singleton<Galaga>;
    Galaga() = default;

    std::string m_PlayerName{};
    bool m_HasGameModeBeenSet{ false };
    GameMode m_CurrentGameMode;
    SceneId m_CurrentScene;
    std::vector<GameEngine::KeyboardInputKey> m_KeyboardSceneKeys;
    std::vector<std::pair<GameEngine::ControllerInputKey, int>> m_ControllerSceneKeys;
    std::vector<GameEngine::KeyboardInputKey> m_PrevKeyboardSceneKeys;
    std::vector<std::pair<GameEngine::ControllerInputKey, int>> m_PrevControllerSceneKeys;
    std::unique_ptr<GameEngine::Scene> LoadLevel(const std::string& enemyInfoPath, const std::string& trajectoryInfoPath);
    std::unique_ptr<GameEngine::Scene> LoadStartScreen();
    std::unique_ptr<GameEngine::Scene> LoadGameOverScene();
    std::unique_ptr<GameEngine::Scene> LoadChooseNameScene();
};
