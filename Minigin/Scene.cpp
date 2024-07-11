#include "Scene.h"
#include "Subjects/GameObject.h"
#include "IObserver.h"
#include <algorithm>
#include "Managers/CollisionManager.h"
#include "Components/CollisionComponent.h"
#include "Managers/TimeManager.h"

using namespace GameEngine;

//#define CHECK_COLLISION_RECTS

Scene::Scene() : m_CollisionManager(std::make_unique<CollisionManager>())
{}

Scene::~Scene() = default;

void Scene::AddGameObjectsToBeAdded()
{
    std::ranges::move(m_GameObjectsToBeAdded, std::back_inserter(m_GameObjects));
    m_GameObjectsToBeAdded.clear();
    m_AreElemsToBeAdded = false;
}

GameObject* Scene::AddObject(std::unique_ptr<GameObject>&& object)
{
    if (object->CheckIfComponentExists<CollisionComponent>())
    {
        m_CollisionManager->AddCollisionComponent(object->GetComponent<CollisionComponent>());
    }
    m_GameObjectsToBeAdded.emplace_back(std::move(object));
    m_AreElemsToBeAdded = true;
    return m_GameObjectsToBeAdded.back().get();
}

IObserver* GameEngine::Scene::AddObserver(int message, std::unique_ptr<IObserver>&& observer, GameObject* gameObj)
{
    if (gameObj != nullptr)
        gameObj->AddObserver(message, m_Observers.emplace_back(std::move(observer)).get());
    else m_Observers.emplace_back(std::move(observer));
    return m_Observers.back().get();
}

void Scene::Remove(const std::unique_ptr<GameObject>& object)
{
    if (object->CheckIfComponentExists<CollisionComponent>())
    {
        m_CollisionManager->RemoveCollisionComponent(object->GetComponent<CollisionComponent>());
    }
    std::erase(m_GameObjects, object);
}

void Scene::RemoveAll()
{
    m_GameObjects.clear();
}

void Scene::Update()
{
    if (m_AreElemsToBeAdded) AddGameObjectsToBeAdded();
    bool areElemsToErase = false;
    for (const auto& object : m_GameObjects)
    {
        if (!object->IsDestroyed()) object->Update();
        else areElemsToErase = true;
    }
    if (areElemsToErase) RemoveDestroyedObjects();
    m_CollisionManager->CheckCollisions();
}

void Scene::Render() const
{
    for (const auto& object : m_GameObjects)
    {
        object->Render();
    }
    #ifdef CHECK_COLLISION_RECTS
    m_CollisionManager->RenderCollisionRects();
    #endif
}

void Scene::RemoveDestroyedObjects()
{
    std::erase_if(m_GameObjects, [&](const auto& obj) {
        if (obj->IsDestroyed() && obj->template CheckIfComponentExists<CollisionComponent>())
            m_CollisionManager->RemoveCollisionComponent(obj->template GetComponent<CollisionComponent>());
        return obj->IsDestroyed();
    });
}
