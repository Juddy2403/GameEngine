#pragma once

namespace GameEngine
{
    class GameObject;
    struct EventData {};
    struct CollisionData : EventData
    {
        GameObject* pOtherCollider;
    };

}
