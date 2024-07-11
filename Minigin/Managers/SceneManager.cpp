#include "SceneManager.h"
#include "Minigin/Subjects/GameObject.h"

void GameEngine::SceneManager::SetCurrentScene(int sceneId)
{
    m_CurrentSceneId = sceneId;
}
void GameEngine::SceneManager::AddScene(int sceneId, std::unique_ptr<Scene>&& scene)
{
    m_Scenes[sceneId] = std::move(scene);
}
void GameEngine::SceneManager::RemoveScene(int sceneId)
{
    m_AreScenesToBeRemoved = true;
    m_SceneIdsToBeRemoved.emplace_back(sceneId);
}
void GameEngine::SceneManager::Update()
{
    if(m_CurrentSceneId != -1) m_Scenes[m_CurrentSceneId]->Update();
    if(m_AreScenesToBeRemoved)
    {
        m_AreScenesToBeRemoved = false;
        for(auto& sceneId : m_SceneIdsToBeRemoved)
        {
            m_Scenes.erase(sceneId);
        }
    }
}

void GameEngine::SceneManager::Render()
{
    if(m_CurrentSceneId != -1) m_Scenes[m_CurrentSceneId]->Render();
}

