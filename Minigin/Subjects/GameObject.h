#pragma once
#include <memory>
#include "../Transform.h"
#include <vector>
#include <algorithm>
#include "../Components/Component.h"
#include "Subject.h"

namespace GameEngine
{
    template<typename T>
    concept ComponentType = std::is_base_of_v<Component, T>;
    class GameObject final: public Subject
    {
    private:
        std::vector<std::unique_ptr<Component>> m_Components{};
        int m_ID{};
        bool m_IsDestroyed{ false };

        GameObject* m_pParent{};
        std::vector<GameObject*> m_pChildren{};

        //Scene graph functions
        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);
        bool IsChild(GameObject* child);

        //Transform members
        Transform m_WorldTransform{};
        Transform m_LocalTransform{};
        bool m_IsPositionDirty{ true };
    public:
        void Update();
        void Render() const;
        [[nodiscard]] int GetID() const;

        [[nodiscard]] bool IsDestroyed() const;
        void SetDestroyedFlag();
        void RemoveDestroyedObjects();

        //Scene graph functions
        [[nodiscard]] GameObject* GetParent() const;
        void SetParent(GameObject* parent, bool keepWorldPosition = true);
        [[nodiscard]] int GetChildCount() const;
        [[nodiscard]] GameObject* GetChildAt(int index) const;

        //Transform functions
        void SetLocalTransform(const Transform& transform);
        Transform& GetLocalTransform();
        [[nodiscard]] const Transform& GetLocalTransform() const;
        Transform GetWorldTransform();
        void UpdateWorldTransform();
        void SetPositionIsDirty();

        void SetPosition(float x, float y, float z = 0);
        void SetPosition(const glm::vec3& pos);
        glm::ivec3 GetIntPosition();
        glm::vec3 GetPosition();

        explicit GameObject(int id);
        ~GameObject() override = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

#pragma region Component Handling

        template<ComponentType T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            m_Components.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
            return dynamic_cast<T*>(m_Components.back().get());
        }
        template<ComponentType T>
        T* AddComponent()
        {
            m_Components.push_back(std::make_unique<T>(this));
            return dynamic_cast<T*>(m_Components.back().get());
        }

        template<ComponentType T>
        T* GetComponent() const
        {
            for (const auto& componentPtr : m_Components)
            {
                if (auto ptr = dynamic_cast<T*>(componentPtr.get())) return ptr;
            }
            return nullptr; // Component not found
        }

        template<ComponentType T>
        void RemoveComponent()
        {
            const auto ret = std::ranges::remove_if(m_Components, [](const auto& elem) {
                return (dynamic_cast<T*>(elem.get()));
            });
            m_Components.erase(ret.begin(), ret.end());
        }

        template<ComponentType T>
        [[nodiscard]] bool CheckIfComponentExists() const
        {
            return std::ranges::find_if(m_Components, [](const auto& elem) {
                    return (dynamic_cast<T*>(elem.get()));
                }
            ) != m_Components.end();
        }
#pragma endregion
    };
}
