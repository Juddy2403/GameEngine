#pragma once
#include <SDL_rect.h>
#include <glm/vec2.hpp>
#include "Component.h"

namespace GameEngine
{
    class CollisionComponent final : public Component
    {
    public:
        explicit CollisionComponent(GameObject* gameObj,SDL_Rect collisionRect);
        [[nodiscard]] const SDL_Rect& GetCollisionRect() const;
        bool IsColliding(CollisionComponent* other) const;
        void CollidedWith(CollisionComponent* other) const;
        virtual void Update() override;
    
        virtual ~CollisionComponent() override = default;
        CollisionComponent(const CollisionComponent& other) = delete;
        CollisionComponent(CollisionComponent&& other) = delete;
        CollisionComponent& operator=(const CollisionComponent& other) = delete;
        CollisionComponent& operator=(CollisionComponent&& other) = delete;
    private:
        SDL_Rect m_CollisionRect{};
        glm::ivec2 m_LastPosition{};
    };
}

