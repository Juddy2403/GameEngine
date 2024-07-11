#pragma once
#include <glm/glm.hpp>

namespace GameEngine
{
	class Transform final
	{
	public:
		[[nodiscard]] glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void Translate(float x, float y);
		void Translate(const glm::vec2& vec);
		void SetRotation(float angle);
		void SetScale(float xScale, float yScale);

		void Update();

		Transform() = default;
		Transform(const Transform& other) = default;
		Transform& operator=(const Transform& other) = default;
		Transform(Transform&& other) noexcept = default;
		Transform& operator=(Transform&& other) noexcept = default;
		Transform operator+(const Transform& other);
		Transform operator-(const Transform& other);
	private:
		glm::vec3 m_Position{};
		glm::mat4 m_TranslateMatrix{glm::mat4(1)};
		glm::mat4 m_RotateMatrix{ glm::mat4(1) };
		glm::mat4 m_ScaleMatrix{ glm::mat4(1) };
	};
}
