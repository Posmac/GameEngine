#pragma once

#include "RefraEngine/Layer.h"

#include "RefraEngine/Events/ApplicationEvent.h"
#include "RefraEngine/Events/KeyEvent.h"
#include "RefraEngine/Events/MouseEvent.h"

namespace rfe
{
	class RF_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

