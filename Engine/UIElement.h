#pragma once

#include "Framework/Object/Component/Component.h"

namespace engine
{
	class UIElement : public Component
	{
	private:
		RectTransform* GetRectTransform() const;
	};
}