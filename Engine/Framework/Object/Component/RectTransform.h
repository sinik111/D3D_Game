#pragma once

#include "Framework/Object/Component/Transform.h"

namespace engine
{
	struct UIRect
	{
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		Vector2 Pos() const { return { x, y }; }
		Vector2 Size() const { return { w, h }; }
	};

	class RectTransform :
		public Transform
	{
	private:
		Vector2 m_anchoredPosition{ 0.0f, 0.0f };
		
		float m_width = 100.0f;
		float m_height = 100.0f;

		Vector2 m_pivot{ 0.5f, 0.5f };

		Vector2 m_anchorMin{ 0.5f, 0.5f };
		Vector2 m_anchorMax{ 0.5f, 0.5f };

		UIRect m_worldRect{};
		bool m_uiDirty = true;

	public:
		RectTransform() = default;
		~RectTransform() override = default;

	public:
		const Vector2& GetAnchoredPosition() const { return m_anchoredPosition; }
		//const Vector2& GetSizeDelta() const { return m_sizeDelta; }

	};

}