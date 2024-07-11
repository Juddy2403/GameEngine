#include "GameObject.h"
#include <iostream>

using namespace GameEngine;

GameObject::GameObject(int id) :
    m_ID{ id }
{}

#pragma region Update stuff

void GameObject::Update()
{
    bool areElemsToErase = false;
    for (const auto& component : m_Components)
    {
        try {
            if (!component->IsDestroyed()) component->Update();
            else areElemsToErase = true;
        } catch (const std::runtime_error& e) {
            std::cerr << "Error updating component: " << e.what() << '\n';
        }
    }
    if (areElemsToErase) RemoveDestroyedObjects();
}

void GameObject::RemoveDestroyedObjects()
{
    const auto range = std::ranges::remove_if(m_Components,
        [](const auto& obj) {
            return obj->IsDestroyed();
        });

    // Erase the destroyed objects from the vector
    m_Components.erase(range.begin(), range.end());
}

void GameEngine::GameObject::Render() const
{
    for (const auto& component : m_Components)
    {
        component->Render();
    }
}
#pragma endregion

#pragma region Scene Graph hierarchy
void GameEngine::GameObject::AddChild(GameObject* child)
{
    m_pChildren.emplace_back(child);
}

void GameEngine::GameObject::RemoveChild(GameObject* child)
{
    m_pChildren.erase(std::ranges::remove(m_pChildren, child).begin());
}

bool GameEngine::GameObject::IsChild(GameObject* child)
{
    return std::ranges::find(m_pChildren, child) != m_pChildren.end();
}

void GameEngine::GameObject::SetPositionIsDirty()
{
    m_IsPositionDirty = true;
    for (auto& child : m_pChildren) child->SetPositionIsDirty();
}

GameObject* GameEngine::GameObject::GetParent() const
{
    return m_pParent;
}

void GameEngine::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    if (IsChild(parent) || parent == this || m_pParent == parent) return;
    if (parent == nullptr) SetLocalTransform(GetWorldTransform());
    else
    {
        if (keepWorldPosition)
            SetLocalTransform(GetWorldTransform() - parent->GetWorldTransform());
        //SetPositionIsDirty();
    }
    if (m_pParent) m_pParent->RemoveChild(this);
    m_pParent = parent;
    if (m_pParent) m_pParent->AddChild(this);
}

int GameEngine::GameObject::GetChildCount() const
{
    return static_cast<int>(m_pChildren.size());
}

GameObject* GameObject::GetChildAt(int index) const
{
    return m_pChildren[index];
}
#pragma endregion

int GameEngine::GameObject::GetID() const
{
    return m_ID;
}

bool GameEngine::GameObject::IsDestroyed() const
{
    return m_IsDestroyed;
}

void GameEngine::GameObject::SetDestroyedFlag()
{
    m_IsDestroyed = true;
    for(auto child : m_pChildren) child->SetDestroyedFlag();
}

void GameObject::SetPosition(float x, float y, float z)
{
    m_LocalTransform.SetPosition(x, y, z);
    SetPositionIsDirty();
}

void GameEngine::GameObject::SetPosition(const glm::vec3& pos)
{
    SetPosition(pos.x, pos.y, pos.z);
}

glm::ivec3 GameObject::GetIntPosition()
{
    return glm::round(GetWorldTransform().GetPosition());
}
glm::vec3 GameObject::GetPosition()
{
    return GetWorldTransform().GetPosition();
}

void GameEngine::GameObject::SetLocalTransform(const Transform& transform)
{
    m_LocalTransform = transform;
    SetPositionIsDirty();
}

Transform& GameEngine::GameObject::GetLocalTransform()
{
    SetPositionIsDirty();
    return m_LocalTransform;
}

const Transform& GameEngine::GameObject::GetLocalTransform() const
{
    return m_LocalTransform;
}

void GameEngine::GameObject::UpdateWorldTransform()
{
    if (m_IsPositionDirty)
    {
        m_LocalTransform.Update();
        if (m_pParent == nullptr) m_WorldTransform = m_LocalTransform;
        else m_WorldTransform = m_pParent->GetWorldTransform() + m_LocalTransform;
    }
    m_IsPositionDirty = false;
}

Transform GameEngine::GameObject::GetWorldTransform()
{
    if (m_IsPositionDirty) UpdateWorldTransform();
    return m_WorldTransform;
}
