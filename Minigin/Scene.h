#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Managers/CollisionManager.h"

namespace GameEngine
{
	class CollisionManager;
	class IObserver;
	class GameObject;
	class Scene final
	{
	public:
		GameObject* AddObject(std::unique_ptr<GameObject>&& object);
		IObserver* AddObserver(int message,std::unique_ptr<IObserver>&& observer,GameObject* gameObj);
		void Remove(const std::unique_ptr<GameObject>& object);
		void RemoveAll();

		void Update();
		//void FixedUpdate();
		void Render() const;
		void RemoveDestroyedObjects();

		explicit Scene();
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) noexcept = delete;
		
	private:
		void AddGameObjectsToBeAdded();
		bool m_AreElemsToBeAdded = false;
		std::unique_ptr<CollisionManager> m_CollisionManager;
		std::vector<std::unique_ptr<IObserver>> m_Observers;
		std::vector<std::unique_ptr<GameObject>> m_GameObjects;
		std::vector<std::unique_ptr<GameObject>> m_GameObjectsToBeAdded;

	};

}
