#pragma once
#include <vector>

namespace GameEngine
{
    class CollisionComponent;
    class CollisionManager final
    {
    public:
        CollisionManager() = default;
        CollisionManager(const CollisionManager& other) = delete;
        CollisionManager(CollisionManager&& other) noexcept = delete;
        CollisionManager& operator=(const CollisionManager& other) = delete;
        CollisionManager& operator=(CollisionManager&& other) noexcept = delete;
        
        void AddCollisionComponent(GameEngine::CollisionComponent* collisionComponent);
        void RemoveCollisionComponent(GameEngine::CollisionComponent* collisionComponent);
        void CheckCollisions() const;
        void RenderCollisionRects() const;
        
        ~CollisionManager() = default;
    private:
        std::vector<GameEngine::CollisionComponent*> m_CollisionComponents{};
    };
}
