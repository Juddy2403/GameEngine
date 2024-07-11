#pragma once
#include <map>
#include <memory>
#include "Singleton.h"
#include "../Scene.h"

namespace GameEngine
{

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void SetCurrentScene(int sceneId);
		void AddScene(int sceneId, std::unique_ptr<Scene>&& scene);
		void RemoveScene(int sceneId);

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<int, std::unique_ptr<Scene>> m_Scenes;
		bool m_AreScenesToBeRemoved = false;
		std::vector<int> m_SceneIdsToBeRemoved;
		int m_CurrentSceneId = -1;
	};
}
