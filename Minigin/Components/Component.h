#pragma once

namespace GameEngine
{
    class GameObject;
    class Component
    {
    private:
        GameObject* m_pParent;
        bool m_IsDestroyed{ false };
    public:
        virtual void Update() {}
        virtual void Render() {}
        [[nodiscard]] GameObject* GetGameObjParent() const;
        [[nodiscard]] bool IsDestroyed() const;
        void SetDestroyedFlag();

        virtual ~Component() = default;
        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;
    protected:
        explicit Component(GameObject* gameObj);
    };

    

}
